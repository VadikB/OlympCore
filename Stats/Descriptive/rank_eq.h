//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCErrorCommon.h"
#include "TCReduce.h"

namespace Descriptive
{
    template <typename Workset>
    struct RankEqReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            if (in == ws.number())
            {
                ws.Param0Element(ind)++;
            }
            else if (ws.ascending())
            {
                if (in < ws.number())
                {
                    ws.Param1Element(ind)++;
                }
            }
            else
            {
                if (in > ws.number())
                {
                    ws.Param1Element(ind)++;
                }
            }
        }
    };

    template <typename Workset>
    struct RankEqCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param0Element(ind) += ws_local.Param0Element(ind);
            ws.Param1Element(ind) += ws_local.Param1Element(ind);
        }
    };

    template <typename Workset>
    struct RankEqFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                if (ws.Param0Element(ind) == 0)
                {
                    ws.Param1Element(ind) = TCError::NewErrorImpl<Workset::in_t>::run(tcerror_code_new(TCError_IllegalInput, 1));
                }
                else
                {
                    ws.Param1Element(ind) += 1;
                }
            }
        }
    };

    template <typename Workset>
    struct RankEqOperator
    {
        typedef ReductionOperator<Workset, RankEqReduce, RankEqFinal, NoCount, RankEqCombine, OneInputReduce> type;
    };

    template <typename T>
    class WorksetRankEq : public Workset1<T>
    {
    public:
        WorksetRankEq(const T num, TCBool order)
            : Workset1<T>(0, 0), _number(num), _order(order) {}

        WorksetRankEq(const WorksetRankEq& source, const TCSize_t nblocks, T* const blocks)
            : Workset1<T>(source, nblocks, blocks), _number(source._number), _order(source._order) {}

        WorksetRankEq(const WorksetRankEq& source, const TCSize_t nblocks)
            : Workset1<T>(source, nblocks), _number(source._number), _order(source._order) {} 

        T number() const { return _number; }
        TCBool ascending() const { return _order; }

    private:
        T _number;
        TCBool _order;
    };

    template <typename T>
    TCErrorCode stats_redall_rank_eq(const T number,  const TCArray* ref, TCBool order, T* rank_eq)
    {
        // Check empty input arrays and return a TCError_IllegalSize error when it applies.
        bool noInput = true;
        TCSize_t arrayId = 0;
        while (noInput && arrayId < 1)
        {
            if (ref[arrayId].m_numelt > 0)
            {
                noInput = false;
            }
            ++arrayId;
        }
        if (noInput)
        {
            *rank_eq = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 2));
            return TCError_NoError;
        }

        typedef RankEqOperator<WorksetRankEq<T> >::type RankEqOp;
        WorksetRankEq<T> ws(number, order);
        return reduce<OnePassReduction<RankEqOp> >(ws, ref, 1, rank_eq);
    }

    template <typename T>
    TCErrorCode stats_reddim_rank_eq(const T number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_eq)
    {
        assert(dim > 0 && dim <= ref->m_ndims);
        
        // Check empty input array in the reduction direction and return an error when it applies.
        if (ref->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 2);
        }

        typedef RankEqOperator<WorksetRankEq<T> >::type RankEqOp;
        WorksetRankEq<T> ws(number, order);
        return reduce<OnePassReduction<RankEqOp> >(ws, ref, dim, rank_eq);
    }
}
