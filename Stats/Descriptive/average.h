



#pragma once

#include "TCReduce.h"

namespace Descriptive
{
    template <typename Workset>
    struct AveReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, typename const Workset::in_t in)
        {
            ws.Param0Element(ind) += in;
        }
    };

    template <typename Workset>
    struct AveCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.Param0Element(ind) += ws_local.Param0Element(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct AveFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param0Element(ind) /= ws.Count(ind);
            }
        }
    };

    template <typename Workset>
    struct AverageOperator
    {
        typedef ReductionOperator<Workset, AveReduce, AveFinal, DoCount, AveCombine, OneInputReduce> type;
    };

    template <typename T>
    TCErrorCode stats_reddim_average(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
    {
        typedef AverageOperator<Workset0<T> >::type AveOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<AveOp> >(ws, numbers, dim, mean);
    }

    template <typename T>
    TCErrorCode stats_redall_average(const TCArray* numbers, const TCSize_t narrays, T* mean)
    {
        typedef AverageOperator<Workset0<T> >::type AveOp;
        Workset0<T> ws(0);
        return reduce<OnePassReduction<AveOp> >(ws, numbers, narrays, mean);
    }
}