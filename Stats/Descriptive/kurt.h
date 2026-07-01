//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCReduce.h"
#include "Descriptive\devsq.h"

namespace Descriptive
{
    template <typename Workset>
    struct KurtReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            Workset::in_t deviation = in - ws.Param0Element(ind); // x[i] - ave(x)
            Workset::in_t deviationSq = deviation * deviation;
            ws.Param1Element(ind) += deviationSq;                 // sum( (x[i] - ave(x)) ^ 2 )
            ws.Param2Element(ind) += deviationSq * deviationSq;   // sum( (x[i] - ave(x)) ^ 4 )
        }
    };

    template <typename Workset>
    struct KurtCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param1Element(ind) += ws_local.Param1Element(ind);
            ws.Param2Element(ind) += ws_local.Param2Element(ind);
        }
    };

    template <typename Workset>
    struct KurtFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                Workset::in_t n = static_cast<Workset::in_t> (ws.Count(ind));
                Workset::in_t sumSq = ws.Param1Element(ind);
                ws.Param2Element(ind) = (n * (n + 1) * (n - 1)) / ((n - 2) * (n - 3))
                    * (ws.Param2Element(ind) / (sumSq * sumSq)) 
                    - (3 * (n - 1) * (n - 1)) / ((n - 2) * (n - 3));
            }
        }
    };

    template <typename T, typename Workset>
    struct KurtOps
    {
        typedef AverageOperator<Workset>::type AveOp;
        typedef ReductionOperator<Workset, KurtReduce, KurtFinal, NoCount, KurtCombine> KurtOp;
        typedef TwoPassReduction<AveOp, KurtOp> ops;
    };

    template <typename T>
    TCErrorCode stats_reddim_kurt(const TCArray* numbers, const TCSize_t dim, TCArray* kurtosis)
    {
        if (dim <= 0 || dim > numbers->m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }

        // Kurt requires the input array has length>=4 in the direction to be
        // reduced. Othrwise, return a TCError_IllegalSize error.
        if (numbers->m_dims[dim-1] < 4)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef Workset2<T> Workset;
        Workset ws(0, 0, 0);
        return reduce<KurtOps<T, Workset>::ops>(ws, numbers, dim, kurtosis);
    }

    template <typename T>
    TCErrorCode stats_redall_kurt(const TCArray* numbers, const TCSize_t narrays, T* kurtosis)
    {
        //Compute the total number of elements
        TCSize_t nTotalElem = 0;
        for (TCSize_t arrayId = 0; arrayId < narrays; arrayId ++)
        {
            nTotalElem += numbers[arrayId].m_numelt;
        }

        // Kurt requires the input array has length>=4. Othrwise, return a TCError_IllegalSize error.
        if (nTotalElem < 4)
        {
            *kurtosis = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }


        typedef Workset2<T> Workset;
        Workset ws(0, 0, 0);
        return reduce<KurtOps<T, Workset>::ops>(ws, numbers, narrays, kurtosis);
    }
}
