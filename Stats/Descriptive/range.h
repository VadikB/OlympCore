//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCReduce.h"
#include "OlympCoreUtils.h"

using namespace Utils;

namespace Descriptive
{
    template <typename Workset>
    struct RangeReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            //Boolean expression (a<b) evaluates to false if a=NaN or b=NaN. Thus use !(a>b) to propagate NaN 
            if ( ! (ws.Param0Element(ind) > in) ) ws.Param0Element(ind) = in;
            if ( ! (ws.Param1Element(ind) < in) ) ws.Param1Element(ind) = in;

        }
    };

    template <typename Workset>
    struct RangeCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            //Boolean expression (a<b) evaluates to false if a=NaN or b=NaN. Thus use !(a>b) to propagate NaN 
            if ( ! (ws.Param0Element(ind) > ws_local.Param0Element(ind))) ws.Param0Element(ind) = ws_local.Param0Element(ind);
            if ( ! (ws.Param1Element(ind) < ws_local.Param1Element(ind))) ws.Param1Element(ind) = ws_local.Param1Element(ind);
        }
    };

    template <typename Workset>
    struct RangeFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) = ws.Param0Element(ind) - ws.Param1Element(ind);
            }
        }
    };

    template <typename T>
    TCErrorCode stats_redall_range(const TCArray* numbers, const TCSize_t narrays, T* range)
    {
        // Check if the input arrays are all zero length. If they are, return
        // a TCError_IllegalSize error.
        bool bHasInput = false;
        for (TCSize_t arrayId = 0; arrayId < narrays; arrayId ++)
        {
            if (numbers[arrayId].m_numelt > 0)
            {
                bHasInput = true;
                break;
            }
        }

        if (false == bHasInput)
        {
            *range = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef ReductionOperator<Workset1<T>, RangeReduce, RangeFinal, NoCount, RangeCombine> RangeOp;
        Workset1<T> ws(Utils::infinity<T>(),-Utils::infinity<T>());
        return reduce<OnePassReduction<RangeOp> >(ws, numbers, narrays, range);
    }
   
    template <typename T>
    TCErrorCode stats_reddim_range(const TCArray* numbers, const TCSize_t dim, TCArray* range)
    {
        if (dim <= 0 || dim > numbers->m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }

        // Check if the input array is length zero in the direction to be
        // reduced. If it is, return a TCError_IllegalSize error.
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        Workset1<T> ws(Utils::infinity<T>(),-Utils::infinity<T>());
        typedef ReductionOperator<Workset1<T>, RangeReduce, RangeFinal, NoCount, RangeCombine> RangeOp;
        return reduce<OnePassReduction<RangeOp> >(ws, numbers, dim, range);
    }
}
