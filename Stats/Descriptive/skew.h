



#pragma once

#include "TCReduce.h"
#include "Descriptive\devsq.h"

namespace Descriptive
{
    template <typename Workset>
    struct SkewReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            Workset::in_t deviation = in - ws.Param0Element(ind); 
            Workset::in_t deviationSq = deviation * deviation;
            ws.Param1Element(ind) += deviationSq;                 
            ws.Param2Element(ind) += deviationSq * deviation;     
        }
    };

    template <typename Workset>
    struct SkewCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param1Element(ind) += ws_local.Param1Element(ind);
            ws.Param2Element(ind) += ws_local.Param2Element(ind);
        }
    };

    template <typename Workset>
    struct SkewFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                Workset::in_t n = static_cast<Workset::in_t> (ws.Count(ind));
                Workset::in_t sumSq = ws.Param1Element(ind);
                ws.Param2Element(ind) *= ((n * Utils::sqrt(n - 1)) / ((n - 2) * Utils::sqrt(sumSq * sumSq * sumSq)));
            }
        }
    };

    template <typename T, typename Workset>
    struct SkewOps
    {
        typedef AverageOperator<Workset>::type AveOp;
        typedef ReductionOperator<Workset, SkewReduce, SkewFinal, NoCount, SkewCombine> SkewOp;
        typedef TwoPassReduction<AveOp, SkewOp> ops;
    };

    template <typename T>
    TCErrorCode stats_reddim_skew(const TCArray* numbers, const TCSize_t dim, TCArray* skewness)
    {
        if (dim <= 0 || dim > numbers->m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }

        
        
        if (numbers->m_dims[dim-1] < 3)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef Workset2<T> Workset;
        Workset ws(0, 0, 0);
        return reduce<SkewOps<T, Workset>::ops>(ws, numbers, dim, skewness);
    }

    template <typename T>
    TCErrorCode stats_redall_skew(const TCArray* numbers, const TCSize_t narrays, T* skewness)
    {
        
        TCSize_t nTotalElem = 0;
        for (TCSize_t arrayId = 0; arrayId < narrays; arrayId ++)
        {
            nTotalElem += numbers[arrayId].m_numelt;
        }

        
        if (nTotalElem < 3)
        {
            *skewness = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef Workset2<T> Workset;
        Workset ws(0, 0, 0);
        return reduce<SkewOps<T, Workset>::ops>(ws, numbers, narrays, skewness);
    }
}
