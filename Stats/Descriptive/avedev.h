//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCErrorCommon.h"
#include "OlympCoreUtils.h"
#include "TCReduce.h"
#include "Descriptive\average.h"

using namespace Utils;

namespace Descriptive
{
    template <typename Workset>
    struct AvedevReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
        {
            Workset::in_t value = Utils::abs(in - ws.Param0Element(ind));
            ws.Param1Element(ind) += value;
        }
    };

    template <typename Workset>
    struct AvedevCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.BlockElement(ind) += ws_local.BlockElement(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct AvedevFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) /= ws.Count(ind);
            }
        }
    };

    template <typename T>
    TCErrorCode stats_redall_avedev(const TCArray* numbers, const TCSize_t narrays, T* ave_dev)
    {
        // Check empty input arrays and return a TCError_IllegalSize error when it applies.
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
            *ave_dev = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef AverageOperator<Workset1<T> >::type AveOp;
        typedef ReductionOperator<Workset1<T>, AvedevReduce, AvedevFinal, NoCount, AvedevCombine> AvedevOp;
        typedef TwoPassReduction<AveOp, AvedevOp> avedevOps;

        Workset1<T> ws(0, 0);
        return reduce<avedevOps>(ws, numbers, narrays, ave_dev);
    }

    template <typename T>
    TCErrorCode stats_reddim_avedev(const TCArray* numbers, const TCSize_t dim, TCArray* ave_dev)
    {
        
        assert(dim > 0 && dim <= numbers->m_ndims);
        
        // Check empty input array in the reduction direction and return an error when it applies.
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef AverageOperator<Workset1<T> >::type AveOp;
        typedef ReductionOperator<Workset1<T>, AvedevReduce, AvedevFinal, NoCount, AvedevCombine> AvedevOp;
        typedef TwoPassReduction<AveOp, AvedevOp> avedevOps;

        Workset1<T> ws(0, 0);
        return reduce<avedevOps>(ws, numbers, dim, ave_dev);
    }
}