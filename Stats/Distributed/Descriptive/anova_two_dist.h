//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "OlympCoreDistUtils.h"
#include "..\..\Descriptive\anova_two.h"
#include <vector>

using namespace DistUtils;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_two_dist(const TCDistArray* data,
    T alpha, 
    T* f_row, 
    T* f_col, 
    T* p_row, 
    T* p_col, 
    T* f_crit_row, 
    T* f_crit_col, 
    T* ss_row, 
    T* ss_col, 
    T* ss_err, 
    T* ss_tot, 
    TCUInt64* df_row, 
    TCUInt64* df_col, 
    TCUInt64* df_err, 
    TCUInt64* df_tot, 
    T* ms_row, 
    T* ms_col, 
    T* ms_err, 
    TCDistArray* s_cnt_row, 
    TCDistArray* s_sum_row, 
    TCDistArray* s_avg_row, 
    TCDistArray* s_var_row, 
    TCDistArray* s_cnt_col, 
    TCDistArray* s_sum_col, 
    TCDistArray* s_avg_col, 
    TCDistArray* s_var_col)
    {
        try
        {
            return AnovaTwoDist<T>::ComputeAnovaTwoDist(data, alpha, 
                                                f_row, f_col, 
                                                p_row, p_col, 
                                                f_crit_row, f_crit_col, 
                                                ss_row, ss_col, ss_err, ss_tot, 
                                                df_row, df_col, df_err, df_tot, 
                                                ms_row, ms_col, ms_err, 
                                                s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
                                                s_cnt_col, s_sum_col, s_avg_col, s_var_col);
        }
        catch(std::bad_alloc &ba)
        {
            ba.what();
            return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
        }
        catch(...)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
    }

    template <typename T>
    class AnovaTwoDist
    {
    public:
        static TCErrorCode ComputeAnovaTwoDist(const TCDistArray* data, 
        T alpha, 
        T* f_row, 
        T* f_col, 
        T* p_row, 
        T* p_col, 
        T* f_crit_row, 
        T* f_crit_col, 
        T* ss_row, 
        T* ss_col, 
        T* ss_err, 
        T* ss_tot, 
        TCUInt64* df_row, 
        TCUInt64* df_col, 
        TCUInt64* df_err, 
        TCUInt64* df_tot, 
        T* ms_row, 
        T* ms_col, 
        T* ms_err, 
        TCDistArray* s_cnt_row, 
        TCDistArray* s_sum_row, 
        TCDistArray* s_avg_row, 
        TCDistArray* s_var_row, 
        TCDistArray* s_cnt_col, 
        TCDistArray* s_sum_col, 
        TCDistArray* s_avg_col, 
        TCDistArray* s_var_col)
        {
            if (!(data->m_Layout.m_GlobalSize) 
                || (data->m_Layout.m_GlobalDims != 2)) 
            {
                return tcerror_code_new(TCError_IllegalSize, 1);
            }

            TCInt32 rank;
            MPI_Comm comm_old = data->m_Layout.m_Comm;
            MPI_Comm_rank(comm_old, &rank);

            const TCArray *localData = &data->m_LocalArray;

            //create new comm
            MPI_Comm comm; //new communicator
            int color = (localData->m_numelt != 0 ? 0 : MPI_UNDEFINED); //include ranks with data only
            MPI_Comm_split(comm_old, color, rank, &comm);
           
            TCSize_t rows_global = data->m_Layout.m_GlobalShape[0];
            TCSize_t cols_global = data->m_Layout.m_GlobalShape[1];

            TCUInt64 *s_cnt_dim_local_data[] = {(TCUInt64*)s_cnt_row->m_LocalArray.m_data, (TCUInt64*)s_cnt_col->m_LocalArray.m_data};
            T *s_sum_dim_local_data[] = {(T*)s_sum_row->m_LocalArray.m_data, (T*)s_sum_col->m_LocalArray.m_data};
            T *s_avg_dim_local_data[] = {(T*)s_avg_row->m_LocalArray.m_data, (T*)s_avg_col->m_LocalArray.m_data};
            T *s_var_dim_local_data[] = {(T*)s_var_row->m_LocalArray.m_data, (T*)s_var_col->m_LocalArray.m_data};

            T *ss_dim[] = {ss_row, ss_col};

            T maxVal = 0;

            *ss_tot = 0;

            TCUInt32 errorsCountLocal = 0;
            TCUInt32 errorsCountGlobal = 0;

            TCErrorCode ec = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);

            if (MPI_COMM_NULL != comm)
            {
                ec = ComputeGlobal(data, s_cnt_dim_local_data, s_sum_dim_local_data, s_avg_dim_local_data, s_var_dim_local_data, ss_dim, ss_tot, &maxVal, comm, rank);
            }

            if (tcerror_code_get_errorid(ec) != TCError_NoError) errorsCountLocal = 1;

            //check for missings on all ranks
            MPI_Allreduce(&errorsCountLocal, &errorsCountGlobal, 1, MPI_UNSIGNED, MPI_SUM, comm_old);
            if (errorsCountGlobal) 
            {
                if (comm != MPI_COMM_NULL) 
                {
                    MPI_Comm_free(&comm);
                }

                return tcerror_code_new(TCError_IllegalInput, 1);
            }

            if (MPI_COMM_NULL == comm) 
            {
                return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
            }

            GetGlobalSum(comm, ss_tot);

            if (MASTER_RANK == rank)
            {
                TCSize_t cnt_global = data->m_Layout.m_GlobalSize;

                *df_row = rows_global - 1;
                *df_col = cols_global - 1;
                *df_tot = cnt_global - 1;

                AnovaTwo<T>::ComputeF(alpha, *ss_row, *ss_col, *ss_tot, *df_row, *df_col, *df_tot, maxVal, 
                                      f_row, f_col, p_row, p_col, f_crit_row, f_crit_col, ss_err, df_err, ms_row, ms_col, ms_err);
            }

            MPI_Comm_free(&comm);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:
        template <typename T>
        static TCErrorCode ComputeGlobal(const TCDistArray* data, 
                                        TCUInt64* s_cnt_local_data[], 
                                        T* s_sum_local_data[], 
                                        T* s_avg_local_data[], 
                                        T* s_var_local_data[], 
                                        T* ss[], T* ss_tot, 
                                        T* maxVal, MPI_Comm comm, TCInt32 rank)
        {
            TCSize_t dim1 = data->m_Layout.m_DistDim;
            TCSize_t dim2 = (dim1 + 1) % 2;

            const TCArray *localData = &data->m_LocalArray;

            TCSize_t cnt_global = data->m_Layout.m_GlobalSize;
            
            TCSize_t cnt_dim1_global = data->m_Layout.m_GlobalShape[dim1];
            TCSize_t cnt_dim2_global = data->m_Layout.m_GlobalShape[dim2];

            T sum_global = 0;
            T avg_global;

            std::vector<TCUInt64> s_cnt_dim2_global(cnt_dim2_global, 0);
            std::vector<T> s_sum_dim2_global(cnt_dim2_global, 0);
            std::vector<T> s_avg_dim2_global(cnt_dim2_global, 0);

            std::vector<T> s_sum_dim2_local(cnt_dim2_global, 0);
            std::vector<T> s_var_dim2_local(cnt_dim2_global, 0);

            memset(s_sum_local_data[dim1], 0, localData->m_dims[dim1] * sizeof(T));

            T* s_sum_dim[] = {s_sum_local_data[dim1], &s_sum_dim2_local[0]};
                        
            TCErrorCode ec = AnovaTwo<T>::ComputeSum(localData, s_sum_dim[dim1], s_sum_dim[dim2], &sum_global, maxVal);

            if (tcerror_code_get_errorid(ec) != TCError_NoError) 
            {
                return ec;
            }

            MPI_Allreduce(&s_sum_dim2_local[0], &s_sum_dim2_global[0], cnt_dim2_global, MPI_Type<T>::mpi_type(), MPI_SUM, comm);

            getGlobalSumMaxVal(comm, &sum_global, maxVal);

            avg_global = sum_global / cnt_global;

            AnovaTwo<T>::ComputeAvgVarSS<T, false>(localData, dim1, avg_global, cnt_dim2_global, s_sum_local_data[dim1], 
                                                   s_cnt_local_data[dim1], s_avg_local_data[dim1], s_var_local_data[dim1], ss[dim1], ss_tot);

            AnovaTwo<T>::ComputeAvgVarSS<T, true>(localData, dim2, avg_global, cnt_dim1_global, &s_sum_dim2_global[0], 
                                                  &s_cnt_dim2_global[0], &s_avg_dim2_global[0], &s_var_dim2_local[0], ss[dim2], ss_tot, true);

            MPI_Reduce(&s_var_dim2_local[0], s_var_local_data[dim2], cnt_dim2_global, MPI_Type<T>::mpi_type(), MPI_SUM, MASTER_RANK, comm);

            if (MASTER_RANK == rank)
            {
                for (TCSize_t i = 0; i < cnt_dim2_global; ++i)
                {
                    s_cnt_local_data[dim2][i] = cnt_dim1_global;
                    s_sum_local_data[dim2][i] = s_sum_dim2_global[i];
                    s_avg_local_data[dim2][i] = s_avg_dim2_global[i];
                    s_var_local_data[dim2][i] /= cnt_dim1_global - 1;
                }
            }

            GetGlobalSum(comm, ss[dim1]);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        template <typename T>
        static void getGlobalSumMaxVal(MPI_Comm comm, T *sum, T *maxVal)
        {
            T sumGlobal;
            T maxValGlobal;

            MPI_Allreduce(sum, &sumGlobal, 1, MPI_Type<T>::mpi_type(), MPI_SUM, comm);
            MPI_Allreduce(maxVal, &maxValGlobal, 1, MPI_Type<T>::mpi_type(), MPI_MAX, comm);

            *sum = sumGlobal;
            *maxVal = maxValGlobal;
        }
    };
}