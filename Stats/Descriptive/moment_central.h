



#pragma once

#include "TCErrorCommon.h"
#include "OlympCoreUtils.h"
#include "TCReduce.h"
#include "Descriptive\average.h"

using namespace Utils;

namespace Descriptive
{
    template <typename Workset>
    struct MomentCentralReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
        {
            Workset::in_t value = Utils::pow((in - ws.Param0Element(ind)), ws.order());
            ws.Param1Element(ind) += value;
        }
    };

    template <typename Workset>
    struct MomentCentralCombine
    {
        static void Combine(Workset& ws, Workset& ws_local, const TCSize_t ind)
        {
            ws.BlockElement(ind) += ws_local.BlockElement(ind);
            ws.Count(ind) += ws_local.Count(ind);
        }
    };

    template <typename Workset>
    struct MomentCentralFinal
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
    struct MomentCentralOperator
    {
        typedef ReductionOperator<Workset, MomentCentralReduce, MomentCentralFinal, NoCount, MomentCentralCombine, OneInputReduce> type;
    };

    template <typename T>
    class WorksetMomentCentral : public Workset1<T>
    {
    public:
        WorksetMomentCentral(const TCSize_t order)
            : Workset1<T>(0, 0), _order(order) {}

        WorksetMomentCentral(const WorksetMomentCentral& source, const TCSize_t nblocks, T* const blocks)
            : Workset1<T>(source, nblocks, blocks), _order(source._order) {}

        WorksetMomentCentral(const WorksetMomentCentral& source, const TCSize_t nblocks)
            : Workset1<T>(source, nblocks), _order(source._order) {} 

        TCSize_t order() const { return _order; }

    private:
        TCSize_t _order;
    };

    template <typename T>
    TCErrorCode stats_redall_moment_central(const TCArray* numbers, const TCSize_t narrays, const TCSize_t order, T* mu_central)
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
            *mu_central = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef AverageOperator<WorksetMomentCentral<T> >::type AveOp;
        typedef ReductionOperator<WorksetMomentCentral<T>, MomentCentralReduce, MomentCentralFinal, NoCount, MomentCentralCombine> MomentCentralOp;
        typedef TwoPassReduction<AveOp, MomentCentralOp> momentcentralOps;

        WorksetMomentCentral<T> ws(order);
        return reduce<momentcentralOps>(ws, numbers, narrays, mu_central);
    }

    template <typename T>
    TCErrorCode stats_reddim_moment_central(const TCArray* numbers, const TCSize_t order, const TCSize_t dim, TCArray* mu_central)
    {
        
        assert(dim > 0 && dim <= numbers->m_ndims);
        
        
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef AverageOperator<WorksetMomentCentral<T> >::type AveOp;
        typedef ReductionOperator<WorksetMomentCentral<T>, MomentCentralReduce, MomentCentralFinal, NoCount, MomentCentralCombine> MomentCentralOp;
        typedef TwoPassReduction<AveOp, MomentCentralOp> momentcentralOps;

        WorksetMomentCentral<T> ws(order);
        return reduce<momentcentralOps>(ws, numbers, dim, mu_central);
    }
}