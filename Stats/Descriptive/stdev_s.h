//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCReduce.h"
#include "OlympCoreUtils.h"
#include "Descriptive\devsq.h"

using namespace Utils;

namespace Descriptive
{
    template <typename Workset>
    struct StdevSFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) = Utils::sqrt(ws.Param1Element(ind) / (ws.Count(ind)-1));
            }
        }
    };

    template <typename T, typename Workset>
    struct StdevSOps
    {
        typedef AverageOperator<Workset>::type AveOp;
        typedef ReductionOperator<Workset, DevsqReduce, StdevSFinal, NoCount, DevsqCombine> StdevSOp;
        typedef TwoPassReduction<AveOp, StdevSOp> ops;
    };

    template <typename T>
    TCErrorCode stats_reddim_stdev_s(const TCArray* numbers, const TCSize_t dim, TCArray* stdev)
    {
        assert(dim > 0 && dim <= numbers->m_ndims);

        // Check if the input array is length zero or one in the direction to
        // be reduced. If it is, return a TCError_IllegalSize error.
        if (numbers->m_dims[dim-1] < 2)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef Workset1<T> Workset;
        Workset ws(0, 0);
        return reduce<StdevSOps<T, Workset>::ops>(ws, numbers, dim, stdev);
    }

    template <typename T>
    TCErrorCode stats_redall_stdev_s(const TCArray* numbers, const TCSize_t narrays, T* stdev)
    {
        // Check if the sum of the input arrays is zero or one. If it is,
        // return a TCError_IllegalSize error.
        TCSize_t inputLength = 0;
        TCSize_t arrayId = 0;
        while (inputLength < 2 && arrayId < narrays)
        {
            inputLength += numbers[arrayId].m_numelt;
            ++arrayId;
        }
        if (inputLength < 2)
        {
            *stdev = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef Workset1<T> Workset;
        Workset ws(0, 0);
        return reduce<StdevSOps<T, Workset>::ops>(ws, numbers, narrays, stdev);
    }
}