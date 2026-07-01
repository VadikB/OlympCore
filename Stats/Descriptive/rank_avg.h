//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCErrorCommon.h"
#include "TCReduce.h"
#include "rank_eq.h"

namespace Descriptive
{
    template <typename Workset>
    struct RankAvgReduce : public RankEqReduce<Workset> {};

    template <typename Workset>
    struct RankAvgCombine : public RankEqReduce<Workset> {};

    template <typename Workset>
    struct RankAvgFinal
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
                    if (ws.Param0Element(ind) > 1)
                    {
                        ws.Param1Element(ind) += (ws.Param0Element(ind) - 1) / 2.0;
                    }
                }
            }
        }
    };

    template <typename Workset>
    struct RankAvgOperator
    {
        typedef ReductionOperator<Workset, RankAvgReduce, RankAvgFinal, NoCount, RankAvgCombine, OneInputReduce> type;
    };

    template <typename T>
    class WorksetRankAvg : public Workset1<T>
    {
    public:
        WorksetRankAvg(const T num, TCBool order)
            : Workset1<T>(0, 0), _number(num), _order(order) {}

        WorksetRankAvg(const WorksetRankAvg& source, const TCSize_t nblocks, T* const blocks)
            : Workset1<T>(source, nblocks, blocks), _number(source._number), _order(source._order) {}

        WorksetRankAvg(const WorksetRankAvg& source, const TCSize_t nblocks)
            : Workset1<T>(source, nblocks), _number(source._number), _order(source._order) {} 

        T number() const { return _number; }
        TCBool ascending() const { return _order; }

    private:
        T _number;
        TCBool _order;
    };

    template <typename T>
    TCErrorCode stats_redall_rank_avg(const T number,  const TCArray* ref, TCBool order, T* rank_avg)
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
            *rank_avg = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 2));
            return TCError_NoError;
        }

        typedef RankAvgOperator<WorksetRankAvg<T> >::type RankAvgOp;
        WorksetRankAvg<T> ws(number, order);
        return reduce<OnePassReduction<RankAvgOp> >(ws, ref, 1, rank_avg);
    }

    template <typename T>
    TCErrorCode stats_reddim_rank_avg(const T number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_avg)
    {
        assert(dim > 0 && dim <= ref->m_ndims);
        
        // Check empty input array in the reduction direction and return an error when it applies.
        if (ref->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 2);
        }

        typedef RankAvgOperator<WorksetRankAvg<T> >::type RankAvgOp;
        WorksetRankAvg<T> ws(number, order);
        return reduce<OnePassReduction<RankAvgOp> >(ws, ref, dim, rank_avg);
    }
}
