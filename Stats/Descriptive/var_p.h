//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCReduce.h"
#include "Descriptive\devsq.h"

namespace Descriptive
{
    template <typename Workset>
    struct VarPFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) /= ws.Count(ind);
            }
        }
    };

    template <typename T, typename Workset>
    struct VarPOps
    {
        typedef AverageOperator<Workset>::type AveOp;
        typedef ReductionOperator<Workset, DevsqReduce, VarPFinal, NoCount, DevsqCombine> VarPOp;
        typedef TwoPassReduction<AveOp, VarPOp> ops;
    };

    template <typename T>
    TCErrorCode stats_reddim_var_p(const TCArray* numbers, const TCSize_t dim, TCArray* variance)
    {
        assert(dim > 0 && dim <= numbers->m_ndims);

        // Check if the input array is length zero in the direction to be
        // reduced. If it is, return a TCError_IllegalSize error.
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef Workset1<T> Workset;
        Workset ws(0, 0);
        return reduce<VarPOps<T, Workset>::ops>(ws, numbers, dim, variance);
    }

    template <typename T>
    TCErrorCode stats_redall_var_p(const TCArray* numbers, const TCSize_t narrays, T* variance)
    {
        // Check if the input arrays are all zero length. If they are, return
        // a TCError_IllegalSize error.
        bool noInput = true;
        TCSize_t arrayId = 0;
        while (noInput && arrayId < narrays)
        {
            if (numbers[arrayId].m_numelt > 0)
            {
                noInput = false;
            }
            ++arrayId;
        }
        if (noInput)
        {
            *variance = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef Workset1<T> Workset;
        Workset ws(0, 0);
        return reduce<VarPOps<T, Workset>::ops>(ws, numbers, narrays, variance);
    }
}