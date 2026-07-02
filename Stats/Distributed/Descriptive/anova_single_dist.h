



#pragma once

#include "OlympCoreDistUtils.h"
#include "..\..\Descriptive\anova_single.h"
#include <vector>

using namespace DistUtils;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_single_dist(const TCDistArray* data, 
    T alpha, 
    TCBool bycol,
    T* f, 
    T* p, 
    T* f_crit, 
    T* ssb, 
    T* ssw, 
    T* sst, 
    TCUInt64* dfb, 
    TCUInt64* dfw, 
    TCUInt64* dft, 
    T* msb, 
    T* msw, 
    TCDistArray* s_cnt, 
    TCDistArray* s_sum, 
    TCDistArray* s_avg, 
    TCDistArray* s_var)
    {
        try
        {
            return AnovaSingleDist<T>::ComputeAnovaSingleDist(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
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
    class AnovaSingleDist
    {
    public:
        static TCErrorCode ComputeAnovaSingleDist(const TCDistArray* data, 
        T alpha, 
        TCBool bycol,
        T* f, 
        T* p, 
        T* f_crit, 
        T* ssb, 
        T* ssw, 
        T* sst, 
        TCUInt64* dfb, 
        TCUInt64* dfw, 
        TCUInt64* dft, 
        T* msb, 
        T* msw, 
        TCDistArray* s_cnt, 
        TCDistArray* s_sum, 
        TCDistArray* s_avg, 
        TCDistArray* s_var)
        {
            if (!(data->m_Layout.m_GlobalSize) 
                || (data->m_Layout.m_GlobalDims != 2) 
                || (data->m_Layout.m_GlobalShape[0] < 2 && !bycol) 
                || (data->m_Layout.m_GlobalShape[1] < 2 && bycol)) 
            {
                return tcerror_code_new(TCError_IllegalSize, 1);
            }

            TCInt32 rank;
            MPI_Comm comm_tmp = data->m_Layout.m_Comm;
            MPI_Comm_rank(comm_tmp, &rank);

            const TCArray *localData = &data->m_LocalArray;

            
            MPI_Comm comm; 
            int color = (localData->m_numelt != 0 ? 0 : MPI_UNDEFINED); 
            MPI_Comm_split(comm_tmp, color, rank, &comm);
            if (MPI_COMM_NULL == comm) return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);

            bool badDistribution = (data->m_Layout.m_DistDim == 0 && bycol) || (data->m_Layout.m_DistDim == 1 && !bycol);

            T avg = 0;
            T sum = 0;
            TCUInt64 cnt = 0;
            TCSize_t dim = (bycol ? 2 : 1);
            TCSize_t outputSize = localData->m_dims[dim - 1];
            T maxVal = 0;

            TCUInt64 *s_cnt_local_data = (TCUInt64*)s_cnt->m_LocalArray.m_data;
            T *s_sum_local_data = (T*)s_sum->m_LocalArray.m_data;
            T *s_avg_local_data = (T*)s_avg->m_LocalArray.m_data;
            T *s_var_local_data = (T*)s_var->m_LocalArray.m_data;
            
            if (!badDistribution)
            {
                AnovaSingle<T>::ComputeSumCount(localData, dim, s_cnt_local_data, s_sum_local_data, &sum, &cnt, &maxVal);

                
                getGlobalSumCount(comm, &sum, &cnt, &maxVal);

                avg = sum / cnt;

                AnovaSingle<T>::ComputeAvgVarSS(localData, dim, s_cnt_local_data, s_sum_local_data, s_avg_local_data, s_var_local_data, avg, ssb, ssw);

                GetGlobalSum(comm, ssb);
            } 
            else
            {
                std::vector<TCUInt64> s_cnt_local_tmp(outputSize, 0);
                std::vector<T> s_sum_local_tmp(outputSize, 0);
                std::vector<T> s_var_local_tmp(outputSize, 0);

                std::vector<TCUInt64> s_cnt_global(outputSize, 0);
                std::vector<T> s_sum_global(outputSize, 0);
                std::vector<T> s_avg_global(outputSize, 0);

                AnovaSingle<T>::ComputeSumCount(localData, dim, &s_cnt_local_tmp[0], &s_sum_local_tmp[0], &sum, &cnt, &maxVal);

                
                MPI_Allreduce(&s_cnt_local_tmp[0], &s_cnt_global[0], outputSize, MPI_LONG_LONG_INT, MPI_SUM, comm);
                MPI_Allreduce(&s_sum_local_tmp[0], &s_sum_global[0], outputSize, MPI_Type<T>::mpi_type(), MPI_SUM, comm);

                
                getGlobalSumCount(comm, &sum, &cnt, &maxVal);

                avg = sum / cnt;

                AnovaSingle<T>::ComputeAvgVarSS(localData, dim, &s_cnt_global[0], &s_sum_global[0], &s_avg_global[0], &s_var_local_tmp[0], avg, ssb, ssw, badDistribution);

                
                MPI_Reduce(&s_var_local_tmp[0], s_var_local_data, outputSize, MPI_Type<T>::mpi_type(), MPI_SUM, MASTER_RANK, comm);
                   
                if (MASTER_RANK == rank)
                {
                    for (TCSize_t i = 0; i < outputSize; ++i)
                    {
                        s_cnt_local_data[i] = s_cnt_global[i];
                        s_sum_local_data[i] = s_sum_global[i];
                        s_avg_local_data[i] = s_avg_global[i];
                        s_var_local_data[i] /= (s_cnt_local_data[i] - 1);
                    }
                }
            }

            GetGlobalSum(comm, ssw);

            if (MASTER_RANK == rank)
            {
                *dfb = data->m_Layout.m_GlobalShape[dim - 1] - 1;
                *dfw = cnt - data->m_Layout.m_GlobalShape[dim - 1];

                AnovaSingle<T>::ComputeF(alpha, *ssb, *ssw, *dfb, *dfw, maxVal, f, p, f_crit, sst, dft, msb, msw);
            }

            MPI_Comm_free(&comm);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:

        template <typename T>
        static void getGlobalSumCount(MPI_Comm comm, T *sum, TCUInt64 *cnt, T *maxVal)
        {
            TCUInt64 cntGlobal;
            T sumGlobal;
            T maxValGlobal;

            
            MPI_Allreduce(cnt, &cntGlobal, 1, MPI_LONG_LONG_INT, MPI_SUM, comm);
            MPI_Allreduce(sum, &sumGlobal, 1, MPI_Type<T>::mpi_type(), MPI_SUM, comm);
            MPI_Allreduce(maxVal, &maxValGlobal, 1, MPI_Type<T>::mpi_type(), MPI_MAX, comm);

            *cnt = cntGlobal;
            *sum = sumGlobal;
            *maxVal = maxValGlobal;
        }
    };
}