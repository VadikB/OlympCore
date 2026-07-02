



#pragma once

#include "TCReduceDist.h"
#include "OlympCoreDist.h"
#include "TCMathDist.h"
#include "OlympCoreUtils.h"

#include <iostream>

template <typename T>
T inline norm_cdf_complement(T x)
{
    const T two = static_cast<T>(2);
    return Utils::erfc(x / Utils::sqrt(two)) / two;
}

template <typename Workset>
struct ZTestDevsqReduce
{
    static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
    {
        Workset::in_t value = in - ws.Mean0();
        ws.Param0Element(ind) += value * value;
    }
};

template<class Workset>
struct ZTestDevsqCombine
{
    static void Combine(Workset & ws, Workset & ws_local, const TCSize_t ind)
    {
        ws.BlockElement(ind) += ws_local.BlockElement(ind);
    }
};

template<class Workset>
struct ZTestStdevSFinal
{
    static void Final(Workset& ws)
    {
        for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
        {
            ws.Param0Element(ind) = Utils::sqrt(ws.Param0Element(ind) / (ws.Count(ind)-1));
        }
    }
};

SPECIALIZE_MPI_OP_TRAIT (ZTestDevsqReduce, MPI_SUM);

namespace Inference 
{   

    template <typename T>
    struct SumAll
    {
        static TCErrorCode call(const TCDistArray *numbers, TCSize_t narrays, TCDouble* y)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
    };

    template <>
    struct SumAll<TCDouble>
    {
        static TCErrorCode call(const TCDistArray *numbers, TCDouble* y)
        {
            return math_redall_sum_d_e_dist(numbers, 1, y);
        }
    };

    template <>
    struct SumAll<TCFloat>
    {
        static TCErrorCode call(const TCDistArray *numbers, TCFloat* y)
        {
            return math_redall_sum_f_e_dist(numbers, 1, y);
        }
    };

    template <typename T>
    class ZTestDistWorkset : public Workset0<T>
    {
    public:
        ZTestDistWorkset(const T mean0)
            : Workset0<T>(0), _mean0(mean0)
        {
        }

        ZTestDistWorkset(const ZTestDistWorkset& source, const TCSize_t nblocks, T* const blocks)
            : Workset0<T>(source, nblocks, blocks), _mean0(source._mean0)
        {
        }

        ZTestDistWorkset(const ZTestDistWorkset& source, const TCSize_t nblocks)
            : Workset0<T>(source, nblocks), _mean0(source._mean0)
        {
        }

        inline T Mean0()
        {
            return _mean0;
        }

    protected:    
        const T _mean0;
    };

    template <typename T>
    TCErrorCode stats_inference_z_test_std_dist(const TCDistArray* sample, T mean0, T std, T* p_value)
    {
        try
        {
            int master_rank = (int)TCDistArray_Get_MasterRank(*sample);
            int rank = TCDistArray_Get_Rank(*sample);

            T smean;
            TCErrorCode result = SumAll<T>::call(sample, &smean);

            if (rank == master_rank)
            {
                if (IS_TCERROR(result))
                {
                    return result;
                }
                else
                {
                    TCSize_t n_elements =  TCDistArray_Get_Size(*sample);
                    T count = static_cast<T>(n_elements);
                    smean /= count;
                    T sqrtN = Utils::sqrt(count);
                    T zscore = sqrtN * (smean - mean0) / std;
                    *p_value = norm_cdf_complement(zscore);
                }
            }
        }
        catch(...)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }

    template <typename T>
    TCErrorCode stats_inference_z_test_dist(const TCDistArray* sample, T mean0, T* p_value)
    {
        try
        {
            TCInt32 comm = sample->m_Layout.m_Comm;

            int master_rank = (int)TCDistArray_Get_MasterRank(*sample);
            int rank = TCDistArray_Get_Rank(*sample);

            T smean;
            TCSize_t n_elements =  TCDistArray_Get_Size(*sample);
            T count = static_cast<T>(n_elements);

            TCErrorCode result = SumAll<T>::call(sample, &smean);

            if(rank == master_rank)
            {
                if (IS_TCERROR(result))
                {
                    return result;
                }
                else
                {
                    smean /= count;
                }
            }

            MPI_Bcast(&smean, 1, mpi_traits<T>::mpi_type(), master_rank, comm);

            typedef ReductionOperator<ZTestDistWorkset<T>, ZTestDevsqReduce, NoFinal, NoCount, ZTestDevsqCombine> ZTestStdevSOp;

            ZTestDistWorkset<T> ws_std(smean);
            T std;

            result = reduce<OnePassReduction<ZTestStdevSOp> >(ws_std, sample, 1, &std, master_rank);

            if (rank == master_rank)
            {
                if (IS_TCERROR(result))
                {
                    return result;
                }
                else
                {
                    std /= (count - 1);
                    std = Utils::sqrt(std);
                    T sqrtN = Utils::sqrt(count);
                    T zscore = sqrtN * (smean - mean0) / std;
                    *p_value = norm_cdf_complement(zscore);
                }
            }

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }
        catch(...)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
    }
} 