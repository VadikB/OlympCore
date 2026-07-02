



#pragma once

#include "TCErrorCommon.h"
#include "OlympCoreUtils.h"
#include "TCReduce.h"

using namespace Utils;

namespace Descriptive
{
    template <typename Workset>
    struct MomentReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
        {
            Workset::in_t value = Utils::pow((in - ws.center()), ws.order());
            ws.Param1Element(ind) += value;
        }
    };

    template <typename Workset>
    struct MomentCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.BlockElement(ind) += ws_local.BlockElement(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct MomentFinal
    {
        static void Final(Workset& ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) /= ws.Count(ind);
            }
        }
    };

    template <typename Workset>
    struct MomentOperator
    {
        typedef ReductionOperator<Workset, MomentReduce, MomentFinal, DoCount, MomentCombine, OneInputReduce> type;
    };

    template <typename T>
    class WorksetMoment : public Workset1<T>
    {
    public:
        WorksetMoment(const T center, const TCSize_t order)
            : Workset1<T>(0, 0), _center(center), _order(order) {}

        WorksetMoment(const WorksetMoment& source, const TCSize_t nblocks, T* const blocks)
            : Workset1<T>(source, nblocks, blocks), _center(source._center), _order(source._order) {}

        WorksetMoment(const WorksetMoment& source, const TCSize_t nblocks)
            : Workset1<T>(source, nblocks), _center(source._center), _order(source._order) {} 

        TCSize_t order() const { return _order; }
        T center() const { return _center; }

    private:
        T _center;
        TCSize_t _order;
    };

    template <typename T>
    TCErrorCode stats_redall_moment(const TCArray* numbers, const TCSize_t narrays, const T center, const TCSize_t order, T* mu)
    {
        
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
            *mu = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef ReductionOperator<WorksetMoment<T>, MomentReduce, MomentFinal, DoCount, MomentCombine> MomentOp;
        typedef OnePassReduction<MomentOp> momentOps;

        WorksetMoment<T> ws(center, order);
        return reduce<momentOps>(ws, numbers, narrays, mu);
    }

    template <typename T>
    TCErrorCode stats_reddim_moment(const TCArray* numbers, const T center, const TCSize_t order, const TCSize_t dim, TCArray* mu)
    {
        
        assert(dim > 0 && dim <= numbers->m_ndims);
        
        
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef ReductionOperator<WorksetMoment<T>, MomentReduce, MomentFinal, DoCount, MomentCombine> MomentOp;
        typedef OnePassReduction<MomentOp> momentOps;

        WorksetMoment<T> ws(center, order);
        return reduce<momentOps>(ws, numbers, dim, mu);
    }
}