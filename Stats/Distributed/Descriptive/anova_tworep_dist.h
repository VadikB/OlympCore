//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "OlympCoreDistUtils.h"
#include "..\..\Descriptive\anova_tworep.h"
#include <vector>

using namespace DistUtils;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_tworep_dist(const TCDistArray* data,
    TCUInt64  cnt_row_sample,
    T   alpha,
    T*  f_sample,
    T*  f_col,
    T*  f_int,
    T*  p_sample,
    T*  p_col,
    T*  p_int,
    T*  f_crit_sample,
    T*  f_crit_col,
    T*  f_crit_int,
    T*  ss_sample,
    T*  ss_col,
    T*  ss_int,
    T*  ss_err,
    T*  ss_tot,
    TCUInt64* df_sample,
    TCUInt64* df_col,
    TCUInt64* df_int,
    TCUInt64* df_err,
    TCUInt64* df_tot,
    T*  ms_sample,
    T*  ms_col,
    T*  ms_int,
    T*  ms_err,
    TCDistArray* s_cnt_sample,
    TCDistArray* s_sum_sample,
    TCDistArray* s_avg_sample,
    TCDistArray* s_var_sample,
    TCDistArray* s_cnt_col,
    TCDistArray* s_sum_col,
    TCDistArray* s_avg_col,
    TCDistArray* s_var_col,
    TCDistArray* s_cnt_col_sample,
    TCDistArray* s_sum_col_sample,
    TCDistArray* s_avg_col_sample,
    TCDistArray* s_var_col_sample)
    {
        try
        {
            return AnovaTwoRepDist<T>::ComputeAnovaTwoRepDist(data, cnt_row_sample, alpha, 
                                                f_sample, f_col, f_int, 
                                                p_sample, p_col, p_int,
                                                f_crit_sample, f_crit_col, f_crit_int,
                                                ss_sample, ss_col, ss_int, ss_err, ss_tot, 
                                                df_sample, df_col, df_int, df_err, df_tot, 
                                                ms_sample, ms_col, ms_int, ms_err, 
                                                s_cnt_sample, s_sum_sample, s_avg_sample, s_var_sample, 
                                                s_cnt_col, s_sum_col, s_avg_col, s_var_col,
                                                s_cnt_col_sample, s_sum_col_sample, s_avg_col_sample, s_var_col_sample);
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
    class AnovaTwoRepDist
    {
    public:
        static TCErrorCode ComputeAnovaTwoRepDist(const TCDistArray* data, 
        TCUInt64 cnt_row_sample,
        T   alpha,
        T*  f_sample, 
        T*  f_col, 
        T*  f_int, 
        T*  p_sample, 
        T*  p_col, 
        T*  p_int, 
        T*  f_crit_sample, 
        T*  f_crit_col, 
        T*  f_crit_int, 
        T*  ss_sample, 
        T*  ss_col, 
        T*  ss_int, 
        T*  ss_err, 
        T*  ss_tot, 
        TCUInt64* df_sample, 
        TCUInt64* df_col, 
        TCUInt64* df_int, 
        TCUInt64* df_err, 
        TCUInt64* df_tot, 
        T*  ms_sample, 
        T*  ms_col, 
        T*  ms_int, 
        T*  ms_err, 
        TCDistArray* s_cnt_sample, 
        TCDistArray* s_sum_sample, 
        TCDistArray* s_avg_sample, 
        TCDistArray* s_var_sample, 
        TCDistArray* s_cnt_col, 
        TCDistArray* s_sum_col,
        TCDistArray* s_avg_col, 
        TCDistArray* s_var_col,
        TCDistArray* s_cnt_col_sample,
        TCDistArray* s_sum_col_sample,
        TCDistArray* s_avg_col_sample,
        TCDistArray* s_var_col_sample)
        {
            if (!(data->m_Layout.m_GlobalSize)
                || (data->m_Layout.m_GlobalDims != 2)
                || (data->m_Layout.m_GlobalShape[0] % cnt_row_sample)) return tcerror_code_new(TCError_IllegalSize, 1);

            if ((data->m_Layout.m_GlobalShape[0] == 1)
                || (data->m_Layout.m_GlobalShape[1] == 1)) return tcerror_code_new(TCError_IllegalInput, 1);


            TCInt32 rank;
            MPI_Comm comm_old = data->m_Layout.m_Comm;
            MPI_Comm_rank(comm_old, &rank);

            const TCArray *localData = &data->m_LocalArray;

            //create new comm
            MPI_Comm comm; //new communicator
            int color = (localData->m_numelt != 0 ? 0 : MPI_UNDEFINED); //include ranks with data only
            MPI_Comm_split(comm_old, color, rank, &comm);

            T maxVal = 0;

            TCSize_t cnt_global = data->m_Layout.m_GlobalSize;

            TCSize_t cols_global = data->m_Layout.m_GlobalShape[1];
            TCSize_t samples_global = data->m_Layout.m_GlobalShape[0] / cnt_row_sample;

            T* ss[] = {ss_sample, ss_col};

            TCUInt32 errorsCountLocal = 0;
            TCUInt32 errorsCountGlobal = 0;

            TCErrorCode ec = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);

            if (MPI_COMM_NULL != comm)
            {
                ec = ComputeGlobal(data, cnt_row_sample, s_sum_sample->m_LocalArray.m_numelt,
                                        (TCUInt64*)s_cnt_sample->m_LocalArray.m_data, 
                                        (T*)s_sum_sample->m_LocalArray.m_data, 
                                        (T*)s_avg_sample->m_LocalArray.m_data, 
                                        (T*)s_var_sample->m_LocalArray.m_data, 
                                        (TCUInt64*)s_cnt_col->m_LocalArray.m_data, 
                                        (T*)s_sum_col->m_LocalArray.m_data, 
                                        (T*)s_avg_col->m_LocalArray.m_data, 
                                        (T*)s_var_col->m_LocalArray.m_data,
                                        (TCUInt64*)s_cnt_col_sample->m_LocalArray.m_data, 
                                        (T*)s_sum_col_sample->m_LocalArray.m_data, 
                                        (T*)s_avg_col_sample->m_LocalArray.m_data, 
                                        (T*)s_var_col_sample->m_LocalArray.m_data,
                                        ss, ss_int, ss_err, 
                                        &maxVal, comm, rank);
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

            if (MASTER_RANK == rank)
            {

                *df_sample = samples_global - 1;
                *df_col = cols_global - 1;
                *df_int = *df_sample * (*df_col);
                *df_tot = cnt_global - 1;

                AnovaTwoRep<T>::ComputeF(alpha, 
                             *ss_sample, *ss_col, *ss_int, *ss_err, 
                             *df_sample, *df_col, *df_int, *df_tot, 
                             maxVal, 
                             f_sample, f_col, f_int, 
                             p_sample, p_col, p_int, 
                             f_crit_sample, f_crit_col, f_crit_int, 
                             ss_tot, 
                             df_err, 
                             ms_sample, ms_col, ms_int, ms_err);
            }

            MPI_Comm_free(&comm);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:
        template <typename T>
        static T& GetData(const TCArray* data, TCSize_t index)
        {
            return (static_cast<T*>(data->m_data))[index];
        }

        template <typename T>
        static TCErrorCode ComputeSumDist(const TCArray* data, const TCUInt64 cnt_row_sample, 
                                        const TCSize_t offset, const TCSize_t n_samples,
                                        T* s_sum_sample, 
                                        T* s_sum_col,
                                        T* s_sum_col_sample,
                                        T* sum_tot, T* maxVal)
        {
            *sum_tot = (T)0;

            for (TCSize_t index_arr = 0; index_arr < data->m_numelt; ++index_arr)
            {
                TCSize_t index_sample = (index_arr % data->m_dims[0] + (cnt_row_sample - offset) % cnt_row_sample) / cnt_row_sample;
                TCSize_t index_col = index_arr / data->m_dims[0];
                TCSize_t index_col_sample = index_sample + index_col * n_samples;

                T x = GetData<T>(data, index_arr);

                if (TCError::IsErrorOrMissingImpl<T>::run(&x))
                {
                    return tcerror_code_new(TCError_IllegalInput, 1);
                }

                s_sum_sample[index_sample] += x;
                s_sum_col[index_col] += x;
                s_sum_col_sample[index_col_sample] += x;

                *sum_tot += x;

                *maxVal = std::max<T>(*maxVal, fabs(x));
            }

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        template <typename T>
        static TCErrorCode ComputeGlobal(const TCDistArray* data, const TCUInt64 cnt_row_sample, const TCSize_t n_samples_output,
                                        TCUInt64* s_cnt_sample_local_data, 
                                        T* s_sum_sample_local_data, 
                                        T* s_avg_sample_local_data, 
                                        T* s_var_sample_local_data, 
                                        TCUInt64* s_cnt_col_local_data, 
                                        T* s_sum_col_local_data, 
                                        T* s_avg_col_local_data, 
                                        T* s_var_col_local_data, 
                                        TCUInt64* s_cnt_col_sample_local_data, 
                                        T* s_sum_col_sample_local_data, 
                                        T* s_avg_col_sample_local_data, 
                                        T* s_var_col_sample_local_data, 
                                        T* ss[], T* ss_int, T* ss_err, 
                                        T* maxVal, MPI_Comm comm, TCInt32 rank)
        {
            const TCArray *localData = &data->m_LocalArray;

            T sum_global = 0;
            T avg_global;

            TCSize_t dim1 = data->m_Layout.m_DistDim;
            TCSize_t dim2 = (dim1 + 1) % 2;

            TCSize_t cnt_global = data->m_Layout.m_GlobalSize;

            TCSize_t rows_local = localData->m_dims[0];
            TCSize_t cols_local = localData->m_dims[1];

            TCSize_t endIdx = data->m_Layout.m_Spans[rank].EndIdx;
            TCSize_t startIdx = data->m_Layout.m_Spans[rank].StartIdx;

            TCSize_t samples_local = rows_local / cnt_row_sample; //number of "whole" samples on current rank
            TCSize_t offset_start = 0;
            TCSize_t offset_end = 0;

            TCBool samplesSplitted = (rows_local % cnt_row_sample) ? 1 : 0;

            TCSize_t samples_local_total = samples_local;

            if (dim1 == 0)
            {
                offset_start = (cnt_row_sample - startIdx % cnt_row_sample) % cnt_row_sample;
                offset_end = (endIdx + 1) % cnt_row_sample;
            
                samples_local_total = (endIdx + 1) / cnt_row_sample - startIdx / cnt_row_sample;
                if ((endIdx + 1) % cnt_row_sample != 0) ++samples_local_total;
            }

            TCSize_t samples_global = data->m_Layout.m_GlobalShape[0] / cnt_row_sample;
            TCSize_t dims_global[] = {samples_global, data->m_Layout.m_GlobalShape[1]}; //samples, cols

            std::vector<T> s_sum_sample_local(samples_local_total, 0);

            //needed temp arrays for cols only if dim1 == 0 (distributed by rows)!!!
            std::vector<T> s_sum_col_local(cols_local, 0);

            std::vector<T> s_sum_col_sample_local(samples_local_total * cols_local, 0);

            std::vector<TCUInt64> s_cnt_dim2_global(dims_global[dim2], 0);
            std::vector<T> s_sum_dim2_global(dims_global[dim2], 0);
            std::vector<T> s_avg_dim2_global(dims_global[dim2], 0);
            std::vector<T> s_var_dim2_global(dims_global[dim2], 0);

            T* s_sum_sample_local_ptr = &s_sum_sample_local[0];
            T* s_sum_col_local_ptr = s_sum_col_local_data; //s_sum_col->m_localArray.m_data;
            T* s_sum_col_sample_local_ptr = &s_sum_col_sample_local[0];

            T* s_sum_dim2_local_ptr = s_sum_sample_local_ptr;

            if (dim1 == 0) //distributed by rows (samples)
            {
                s_sum_col_local_ptr = &s_sum_col_local[0];

                s_sum_dim2_local_ptr = s_sum_col_local_ptr;
            }
            else
            {
                memset(s_sum_col_local_ptr, 0, cols_local * sizeof(T));
            }

            TCErrorCode ec = ComputeSumDist(localData, cnt_row_sample, offset_start, samples_local_total, 
                                            s_sum_sample_local_ptr, s_sum_col_local_ptr, s_sum_col_sample_local_ptr, &sum_global, maxVal);

            if (tcerror_code_get_errorid(ec) != TCError_NoError) 
            {
                return ec;
            }

            MPI_Allreduce(s_sum_dim2_local_ptr, &s_sum_dim2_global[0], dims_global[dim2], MPI_Type<T>::mpi_type(), MPI_SUM, comm);

            getGlobalSumMaxVal(comm, &sum_global, maxVal);

            avg_global = sum_global / cnt_global;

            TCUInt32 threshold = rows_local;
            MPI_Status status;

            MPI_Bcast(&threshold, 1, MPI_UNSIGNED_LONG, 0, comm);

             //=============================== merge sums for samples ==========================// 
            //merge sample arrays if dim1 == 0 (distributed by rows) and samples splitted between ranks
            if (dim1 == 0 && samplesSplitted)
            {
                mergeSampleArrays(cnt_row_sample, offset_start, offset_end, samples_local_total, cols_local, 
                                  threshold, s_sum_sample_local_ptr, &s_sum_col_sample_local[0], comm, rank);
            }

            TCSize_t dims_local[] = {samples_local_total, cols_local}; //samples, cols
            
            std::vector<TCUInt64> s_cnt_sample_local(samples_local_total, 0);
            std::vector<T> s_avg_sample_local(samples_local_total, 0);
            std::vector<T> s_var_sample_local(samples_local_total, 0);

            std::vector<TCUInt64> s_cnt_col_sample_local(samples_local_total * cols_local, 0);
            std::vector<T> s_avg_col_sample_local(samples_local_total * cols_local, 0);
            std::vector<T> s_var_col_sample_local(samples_local_total * cols_local, 0);

            TCUInt64* s_cnt_local_data_result[] = {&s_cnt_sample_local[0], s_cnt_col_local_data};
            T* s_sum_local_data_result[] = {&s_sum_sample_local[0], s_sum_col_local_data};
            T* s_avg_local_data_result[] = {&s_avg_sample_local[0], s_avg_col_local_data};
            T* s_var_local_data_result[] = {&s_var_sample_local[0], s_var_col_local_data};
                        
            *(ss[0]) = (T)0;
            *(ss[1]) = (T)0;

            //if dim1 == 1 || dim1 == 0 && rows % cnt_row_samples == 0 then we can use AnovaTwoRep<T>::ComputeAvgSSFactor as more effective
            ComputeAvgSSFactorDist(cnt_row_sample, dims_local[dim1], cnt_row_sample * dims_global[dim2], dims_global[dim2], 
                                   s_cnt_local_data_result[dim1], s_sum_local_data_result[dim1], s_avg_local_data_result[dim1], ss[dim1], avg_global, offset_start);
            
            ComputeAvgSSFactorDist(cnt_row_sample, dims_global[dim2], cnt_row_sample * dims_global[dim1], dims_global[dim1], 
                                   &s_cnt_dim2_global[0], &s_sum_dim2_global[0], &s_avg_dim2_global[0], ss[dim2], avg_global, 0);

            GetGlobalSum(comm, ss[dim1]);


            std::vector<T> s_var_col_local(cols_local, 0);

            T* s_avg_dim[] = {s_avg_local_data_result[dim1], &s_avg_dim2_global[0]};
            T* s_var_dim[] = {&s_var_sample_local[0], &s_var_col_local[0]};

            ComputeSSIntDist(cnt_row_sample, offset_start,
                                    samples_local_total, cols_local, 
                                    &s_cnt_col_sample_local[0], s_sum_col_sample_local_ptr, &s_avg_col_sample_local[0],
                                    s_avg_dim[dim1], s_avg_dim[dim2],
                                    avg_global, ss_int);

            ComputeVarSSErrDist(localData, cnt_row_sample, offset_start,
                                    samples_local_total, s_sum_col_sample_local_ptr,
                                    s_avg_dim[dim1], s_avg_dim[dim2], 
                                    s_var_dim[0], s_var_dim[1], &s_var_col_sample_local[0],
                                    ss_err);
            
            //compute global var before dividing
            MPI_Reduce(s_var_dim[dim2], &s_var_dim2_global[0], dims_global[dim2], MPI_Type<T>::mpi_type(), MPI_SUM, MASTER_RANK, comm);

            GetGlobalSum(comm, ss_int);
            GetGlobalSum(comm, ss_err);


            if (dim1 == 0 && samplesSplitted)
            {
                mergeSampleArrays(cnt_row_sample, offset_start, offset_end, samples_local_total, cols_local, 
                                  threshold, &s_var_sample_local[0], &s_var_col_sample_local[0], comm, rank);
            }

            //divide vars for col_sample and dim1
            if (dim1 == 0) //distributed by rows
            {
                for (TCSize_t i = 0; i < samples_local_total; ++i)
                {
                    s_var_local_data_result[dim1][i] /= (cnt_row_sample * cols_local - 1);

                    for (TCSize_t j = 0; j < cols_local; ++j)
                    {
                        s_var_col_sample_local[j * samples_local_total + i] /= cnt_row_sample - 1;
                    }
                }
            }
            else //distributed by cols
            {
                for (TCSize_t i = 0; i < cols_local; ++i)
                {
                    s_var_local_data_result[dim1][i] = s_var_col_local[i] / (cnt_row_sample * samples_local - 1);

                    for (TCSize_t j = 0; j < samples_local; ++j)
                    {
                        s_var_col_sample_local[i * samples_local + j] /= cnt_row_sample - 1;
                    }
                }
            }

            if (MASTER_RANK == rank)
            {
                if (dim1 == 0)
                {
                    for (TCSize_t k = 0; k < dims_global[dim2]; ++k)
                    {
                        s_cnt_local_data_result[dim2][k] = s_cnt_dim2_global[k];
                        s_sum_local_data_result[dim2][k] = s_sum_dim2_global[k];
                        s_avg_local_data_result[dim2][k] = s_avg_dim2_global[k];
                        s_var_local_data_result[dim2][k] = s_var_dim2_global[k] / (s_cnt_dim2_global[k] - 1);
                    }
                }
                else
                {
                    for (TCSize_t k = 0; k < samples_global; ++k)
                    {
                        s_cnt_sample_local_data[k] = s_cnt_dim2_global[k];
                        s_sum_sample_local_data[k] = s_sum_dim2_global[k];
                        s_avg_sample_local_data[k] = s_avg_dim2_global[k];
                        s_var_sample_local_data[k] = s_var_dim2_global[k] / (s_cnt_dim2_global[k] - 1);
                    }
                }
            }

            //if dim1 == 0 - gather samples and col_samples data on ranks with output arrays!!!
            if (dim1 == 0)
            {
                TCSize_t sample_index_start = startIdx / cnt_row_sample;

                TCUInt32 destRank = sample_index_start / threshold;

                for (TCSize_t i = 0; i < samples_local_total; ++i)
                {
                    if (rank == 0)
                    {
                        s_cnt_sample_local_data[i] = s_cnt_sample_local[i];
                        s_sum_sample_local_data[i] = s_sum_sample_local[i];
                        s_avg_sample_local_data[i] = s_avg_sample_local[i];
                        s_var_sample_local_data[i] = s_var_sample_local[i];

                        for (TCSize_t j = 0; j < cols_local; ++j)
                        {
                            s_cnt_col_sample_local_data[j * n_samples_output + i] = s_cnt_col_sample_local[j * samples_local_total + i];
                            s_sum_col_sample_local_data[j * n_samples_output + i] = s_sum_col_sample_local[j * samples_local_total + i];
                            s_avg_col_sample_local_data[j * n_samples_output + i] = s_avg_col_sample_local[j * samples_local_total + i];
                            s_var_col_sample_local_data[j * n_samples_output + i] = s_var_col_sample_local[j * samples_local_total + i];
                        }
                    }
                    else if (i || !offset_start)
                    {
                        //send
                        TCUInt32 tag1 = i + sample_index_start % threshold;
                        TCUInt32 tag2 = i + sample_index_start % threshold + threshold;
                        TCUInt32 tag3 = i + sample_index_start % threshold + 2 * threshold;
                        TCUInt32 tag4 = i + sample_index_start % threshold + 3 * threshold;

                        MPI_Send(&s_sum_sample_local[i], 1, MPI_Type<T>::mpi_type(), destRank, tag1, comm);
                        MPI_Send(&s_var_sample_local[i], 1, MPI_Type<T>::mpi_type(), destRank, tag2, comm);

                        std::vector<T> s_sum_col_sample_tmp(cols_local, 0);
                        std::vector<T> s_var_col_sample_tmp(cols_local, 0);

                        for (TCSize_t j = 0; j < cols_local; ++j)
                        {
                            s_sum_col_sample_tmp[j] = s_sum_col_sample_local[j * samples_local_total + i];
                            s_var_col_sample_tmp[j] = s_var_col_sample_local[j * samples_local_total + i];
                        }

                        MPI_Send(&s_sum_col_sample_tmp[0], cols_local, MPI_Type<T>::mpi_type(), destRank, tag3, comm);
                        MPI_Send(&s_var_col_sample_tmp[0], cols_local, MPI_Type<T>::mpi_type(), destRank, tag4, comm);
                    }
                }

                TCInt32 n_ranks = dims_global[0] / threshold; //number of ranks with output
                if (dims_global[0] % threshold) ++n_ranks;

                if (rank < n_ranks)
                {
                    TCSize_t start_index = 0;
                    if (rank == 0) start_index = samples_local_total;

                    for (TCSize_t i = start_index; i < n_samples_output; ++i)
                    {
                        TCUInt32 srcRank = ((rank * threshold + i) * cnt_row_sample) / threshold;

                        TCUInt32 tag1 = i;
                        TCUInt32 tag2 = i + threshold;
                        TCUInt32 tag3 = i + 2 * threshold;
                        TCUInt32 tag4 = i + 3 * threshold;

                        MPI_Recv(&s_sum_sample_local_data[i], 1, MPI_Type<T>::mpi_type(), srcRank, tag1, comm, &status);
                        MPI_Recv(&s_var_sample_local_data[i], 1, MPI_Type<T>::mpi_type(), srcRank, tag2, comm, &status);

                        s_cnt_sample_local_data[i] = cnt_row_sample * cols_local;
                        s_avg_sample_local_data[i] = s_sum_sample_local_data[i] / s_cnt_sample_local_data[i];

                        std::vector<T> s_sum_col_sample_tmp(cols_local, 0);
                        std::vector<T> s_var_col_sample_tmp(cols_local, 0);

                        MPI_Recv(&s_sum_col_sample_tmp[0], cols_local, MPI_Type<T>::mpi_type(), srcRank, tag3, comm, &status);
                        MPI_Recv(&s_var_col_sample_tmp[0], cols_local, MPI_Type<T>::mpi_type(), srcRank, tag4, comm, &status);

                        for (TCSize_t j = 0; j < cols_local; ++j)
                        {
                            s_cnt_col_sample_local_data[j * n_samples_output + i] = cnt_row_sample;
                            s_sum_col_sample_local_data[j * n_samples_output + i] = s_sum_col_sample_tmp[j];
                            s_avg_col_sample_local_data[j * n_samples_output + i] = s_sum_col_sample_tmp[j] / cnt_row_sample;
                            s_var_col_sample_local_data[j * n_samples_output + i] = s_var_col_sample_tmp[j];
                        }
                    }
                }
            }
            else
            {
                for (TCSize_t k = 0; k < samples_global * cols_local; ++k)
                {
                    s_cnt_col_sample_local_data[k] = s_cnt_col_sample_local[k];
                    s_sum_col_sample_local_data[k] = s_sum_col_sample_local[k];
                    s_avg_col_sample_local_data[k] = s_avg_col_sample_local[k];
                    s_var_col_sample_local_data[k] = s_var_col_sample_local[k];
                }
            }

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        template <typename T>
        static void ComputeAvgSSFactorDist(const TCUInt64 cnt_row_sample, const TCSize_t nelements, const TCSize_t cnt, const TCSize_t cnt_opp, 
                                           TCUInt64* s_cnt, T* s_sum, T* s_avg, T* ss, const T avg, const TCSize_t offset_start = 0)
        {
            for (TCSize_t i = 0; i < nelements; ++i)
            {
                T avg_dim = s_sum[i] / cnt;

                if (i || !offset_start)
                {
                    *ss += (avg_dim - avg) * (avg_dim - avg);
                }
                
                s_cnt[i] = cnt;
                s_avg[i] = avg_dim;
            }

            *ss *= cnt_row_sample * cnt_opp;
        }

        template <typename T>
        static void ComputeSSIntDist(const TCUInt64 cnt_row_sample, const TCSize_t offset_samples_start,
                                    const TCSize_t samples_local, const TCSize_t cols_local,
                                    TCUInt64* s_cnt_col_sample_data, T* s_sum_col_sample_data, T* s_avg_col_sample_data,
                                    T* s_avg_sample_data, T* s_avg_col_data,
                                    const T avg_global, T* ss_int)
        {
            *ss_int = (T)0;

            TCSize_t start_index = 0;
            if (offset_samples_start) ++start_index;

            for (TCSize_t j = start_index; j < samples_local; ++j)
            {
                T avg_sample = s_avg_sample_data[j];

                for (TCSize_t k = 0; k < cols_local; ++k)
                {
                    TCSize_t index_col_sample = j + k * samples_local;

                    s_cnt_col_sample_data[index_col_sample] = cnt_row_sample;
                    T avg_col_sample = s_sum_col_sample_data[index_col_sample] / cnt_row_sample;
                        
                    T avg_col = s_avg_col_data[k];

                    *ss_int += (avg_col_sample - avg_sample - avg_col + avg_global) * (avg_col_sample - avg_sample - avg_col + avg_global);

                    s_avg_col_sample_data[index_col_sample] = avg_col_sample;
                }
            }

            *ss_int *= cnt_row_sample;
        }

        template <typename T>
        static void ComputeVarSSErrDist(const TCArray* data, const TCUInt64 cnt_row_sample, const TCSize_t offset_samples_start,
                                    const TCSize_t samples_local,
                                    T* s_sum_col_sample_data,
                                    T* s_avg_sample_data, T* s_avg_col_data,
                                    T* s_var_sample_data, T* s_var_col_data, T* s_var_col_sample_data,
                                    T* ss_err)
        {
            *ss_err = (T)0;

            for (TCSize_t index_arr = 0; index_arr < data->m_numelt; ++index_arr)
            {
                TCSize_t index_sample = (index_arr % data->m_dims[0] + (cnt_row_sample - offset_samples_start) % cnt_row_sample) / cnt_row_sample;
                TCSize_t index_col = index_arr / data->m_dims[0];
                TCSize_t index_col_sample = index_sample + index_col * samples_local;

                T avg_sample = s_avg_sample_data[index_sample];
                T avg_col = s_avg_col_data[index_col];
                T avg_col_sample = s_sum_col_sample_data[index_col_sample] / cnt_row_sample;

                T x = GetData<T>(data, index_arr);

                *ss_err += (x - avg_col_sample) * (x - avg_col_sample);

                s_var_sample_data[index_sample] += (x - avg_sample) * (x - avg_sample);
                s_var_col_data[index_col] += (x - avg_col) * (x - avg_col);
                s_var_col_sample_data[index_col_sample] += (x - avg_col_sample) * (x - avg_col_sample);
            }
        }


        template <typename T>
        static void mergeSampleArrays(const TCUInt64 cnt_row_sample, const TCSize_t offset_start, const TCSize_t offset_end, 
                                      const TCSize_t samples_local_total, const TCSize_t cols_local, 
                                      const TCUInt32 threshold, T* s_arr_sample_local, T* s_arr_col_sample_local, 
                                      MPI_Comm comm, const TCInt32 rank)
        {
            if (offset_start)
            {
                //need to send
                TCUInt32 destRankOffset = (cnt_row_sample - offset_start) / threshold;
                if ((cnt_row_sample - offset_start) % threshold) ++destRankOffset;

                TCUInt32 destRank = rank - destRankOffset;

                MPI_Send(s_arr_sample_local, 1, MPI_Type<T>::mpi_type(), destRank, 0, comm);

                std::vector<T> s_arr_col_sample_local_tmp(cols_local, 0);

                for (TCSize_t j = 0; j < cols_local; ++j)
                {
                    s_arr_col_sample_local_tmp[j] = s_arr_col_sample_local[j * samples_local_total];
                }

                MPI_Send(&s_arr_col_sample_local_tmp[0], cols_local, MPI_Type<T>::mpi_type(), destRank, 1, comm);


                //need to recv
                MPI_Status status;

                MPI_Recv(s_arr_sample_local, 1, MPI_Type<T>::mpi_type(), destRank, 0, comm, &status);

                MPI_Recv(&s_arr_col_sample_local_tmp[0], cols_local, MPI_Type<T>::mpi_type(), destRank, 1, comm, &status);

                for (TCSize_t j = 0; j < cols_local; ++j)
                {
                    s_arr_col_sample_local[j * samples_local_total] = s_arr_col_sample_local_tmp[j];
                }
            }

            if (offset_end && offset_start < threshold)
            {
                //need to recv
                TCUInt32 srcRankOffset = (cnt_row_sample - offset_end) / threshold;
                if ((cnt_row_sample - offset_end) % threshold) ++srcRankOffset;

                std::vector<T> s_arr_col_sample_local_tmp(cols_local, 0);

                for (TCSize_t i = 0; i < srcRankOffset; ++i)
                {
                    MPI_Status status;

                    T sum = 0;
                    MPI_Recv(&sum, 1, MPI_Type<T>::mpi_type(), rank + i + 1, 0, comm, &status);

                    s_arr_sample_local[samples_local_total - 1] += sum;

                    MPI_Recv(&s_arr_col_sample_local_tmp[0], cols_local, MPI_Type<T>::mpi_type(), rank + i + 1, 1, comm, &status);

                    for (TCSize_t j = 0; j < cols_local; ++j)
                    {
                        s_arr_col_sample_local[j * samples_local_total + samples_local_total - 1] += s_arr_col_sample_local_tmp[j];
                    }
                }

                for (TCSize_t j = 0; j < cols_local; ++j)
                {
                    s_arr_col_sample_local_tmp[j] = s_arr_col_sample_local[j * samples_local_total + samples_local_total - 1];
                }

                for (TCSize_t i = 0; i < srcRankOffset; ++i)
                {
                    MPI_Send(&s_arr_sample_local[samples_local_total - 1], 1, MPI_Type<T>::mpi_type(), rank + i + 1, 0, comm);

                    MPI_Send(&s_arr_col_sample_local_tmp[0], cols_local, MPI_Type<T>::mpi_type(), rank + i + 1, 1, comm);
                }
            }
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