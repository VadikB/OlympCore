



#pragma once

#include "TCErrorCommon.h"
#include "TCReduce.h"
#include "Descriptive\average.h"

namespace Descriptive
{
    template <typename Workset>
    struct DevsqReduce
    {
        static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
        {
            Workset::in_t value = in - ws.Param0Element(ind);
            ws.Param1Element(ind) += value * value;
        }
    };

    template<class Workset>
    struct DevsqCombine
    {
        static void Combine(Workset & ws, Workset & ws_local, const TCSize_t ind)
        {
            ws.BlockElement(ind) += ws_local.BlockElement(ind);
        }
    };

    template <typename T>
    TCErrorCode stats_redall_devsq(const TCArray* numbers, const TCSize_t narrays, T* dev_sq)
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
            *dev_sq = TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalSize, 1));
            return TCError_NoError;
        }

        typedef AverageOperator<Workset1<T> >::type AveOp;
        typedef ReductionOperator<Workset1<T>, DevsqReduce, NoFinal, NoCount, DevsqCombine> DevsqOp;
        typedef TwoPassReduction<AveOp, DevsqOp> devsqOps;
        Workset1<T> ws(0, 0);
        return reduce<devsqOps>(ws, numbers, narrays, dev_sq);
    }

    template <typename T>
    TCErrorCode stats_reddim_devsq(const TCArray* numbers, const TCSize_t dim, TCArray* dev_sq)
    {
        assert(dim > 0 && dim <= numbers->m_ndims);

        
        if (numbers->m_dims[dim-1] == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef AverageOperator<Workset1<T> >::type AveOp;
        typedef ReductionOperator<Workset1<T>, DevsqReduce, NoFinal, NoCount, DevsqCombine> DevsqOp;
        typedef TwoPassReduction<AveOp, DevsqOp> devsqOps;
        Workset1<T> ws(0, 0);
        return reduce<devsqOps>(ws, numbers, dim, dev_sq);
    }
}