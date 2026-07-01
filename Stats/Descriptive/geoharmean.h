//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCReduce.h"

namespace Descriptive
{
#pragma region geomean
    template <typename Workset>
    struct GeomeanReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            if (in <= 0)
            {
                ws.Param0Element(ind) = TCError::NewErrorImpl<Workset::in_t>::run(tcerror_code_new(TCError_IllegalInput, 1));
            }
            else
            {
                ws.Param0Element(ind) += Utils::log(in);
            }
        }
    };

    template <typename Workset>
    struct GeomeanCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param0Element(ind) += ws_local.Param0Element(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct GeomeanFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param0Element(ind) /= ws.Count(ind);
                ws.Param0Element(ind) = Utils::exp(ws.Param0Element(ind));
            }
        }
    };

    template <typename Workset>
    struct geomeanOperator
    {
        typedef ReductionOperator<Workset, GeomeanReduce, GeomeanFinal, DoCount, GeomeanCombine, OneInputReduce> type;
    };

    template <typename T>
    TCErrorCode stats_reddim_geomean(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
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

        typedef geomeanOperator<Workset0<T> >::type GeomeanOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<GeomeanOp> >(ws, numbers, dim, mean);
    }

    template <typename T>
    TCErrorCode stats_redall_geomean(const TCArray* numbers, const TCSize_t narrays, T* mean)
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
            *mean = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef geomeanOperator<Workset0<T> >::type GeomeanOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<GeomeanOp> >(ws, numbers, narrays, mean);
    }
#pragma endregion geomean

#pragma region harmean
    template <typename Workset>
    struct HarmeanReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            if (in <= 0)
            {
                ws.Param0Element(ind) = TCError::NewErrorImpl<Workset::in_t>::run(tcerror_code_new(TCError_IllegalInput, 1));
            }
            else
            {
                ws.Param0Element(ind) += 1/in;
            }
        }
    };

    template <typename Workset>
    struct HarmeanCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param0Element(ind) += ws_local.Param0Element(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct HarmeanFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param0Element(ind) = ws.Count(ind)/ws.Param0Element(ind);
            }
        }
    };

    template <typename Workset>
    struct harmeanOperator
    {
        typedef ReductionOperator<Workset, HarmeanReduce, HarmeanFinal, DoCount, HarmeanCombine, OneInputReduce> type;
    };

    template <typename T>
    TCErrorCode stats_reddim_harmean(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
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

        typedef harmeanOperator<Workset0<T> >::type HarmeanOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<HarmeanOp> >(ws, numbers, dim, mean);
    }

    template <typename T>
    TCErrorCode stats_redall_harmean(const TCArray* numbers, const TCSize_t narrays, T* mean)
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
            *mean = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef harmeanOperator<Workset0<T> >::type HarmeanOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<HarmeanOp> >(ws, numbers, narrays, mean);
    }
#pragma endregion harmean

}