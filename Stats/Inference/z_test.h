//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "OlympCoreUtils.h"
#include "norm.h"
#include "TCReduce.h"
#include "norm_s.h"
#include "Descriptive\average.h"

using namespace Descriptive;

template <typename Workset>
struct STDReduce
{
    static void Reduce(Workset& ws, const TCSize_t ind, const Workset::in_t in)
    {
        Workset::in_t value = in - ws.Param0Element(ind);
        ws.Param1Element(ind) += value * value;
    }
};

template <typename Workset>
struct STDCombine
{
    static void Combine(Workset & ws, Workset & ws_local, const TCSize_t ind)
    {
        ws.BlockElement(ind) += ws_local.BlockElement(ind);
        ws.Count(ind) += ws_local.Count(ind);
    }
};


template <typename Workset>
struct STDFinal
{
    static void Final(Workset & ws)
    {
        for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
        {
            ws.Param1Element(ind) /= (ws.Count(ind) - 1);
            ws.Param1Element(ind) = Utils::sqrt(ws.Param1Element(ind));
        }
    }
};


template <typename Workset>
struct ZTestFinal
{
    static void Final(Workset& ws) 
    {
        for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
        {
            const Workset::in_t std = (ws.Param1Element(ind));

            if (std == static_cast<Workset::in_t>(0))
            {
                ws.Param2Element(ind) = (ws.Param0Element(ind) <= ws.Mean0()) ? static_cast<Workset::in_t>(1) : static_cast<Workset::in_t>(0);
            }
            else
            {
                ws.Param1Element(ind) /= (ws.Count(ind) - 1);
                ws.Param1Element(ind) = Utils::sqrt(ws.Param1Element(ind));
                Workset::in_t sqrtN = Utils::sqrt((Workset::in_t)ws.Count(ind));
                Workset::in_t zscore = sqrtN * (ws.Param0Element(ind) - ws.Mean0()) / (ws.Param1Element(ind));

                // complement of normal distribution
                ws.Param2Element(ind) = Utils::erfc(zscore / Utils::sqrt(static_cast<Workset::in_t>(2))) / (static_cast<Workset::in_t>(2));
            }
        }
    }
};

template <typename T>
class ZTestWorkset : public Workset2<T>
{
public:
    ZTestWorkset(const T mean0, TCInt64 tail)
        : Workset2<T>(0, 0, 0), _mean0(mean0), _tail(tail)
    {
    }

    ZTestWorkset(const ZTestWorkset& source, const TCSize_t nblocks, T* const blocks)
        : Workset2<T>(source, nblocks, blocks), _mean0(source._mean0), _tail(source._tail)
    {
    }

    ZTestWorkset(const ZTestWorkset& source, const TCSize_t nblocks)
        : Workset2<T>(source, nblocks), _mean0(source._mean0), _tail(source._tail)
    {
    }

    inline T Mean0()
    {
        return _mean0;
    }

protected:    
    const T _mean0;
    const TCInt64 _tail;
};



template <typename T>
class ZTestStdWorkset : public Workset1<T>
{
public:
    ZTestStdWorkset(T mean, T std, TCInt64 tail)
        : Workset1<T>(0, 0) , _mean0(mean), _std(std), _tail(tail) 
    {
    }

    ZTestStdWorkset(const ZTestStdWorkset& source, const TCSize_t nblocks, T* const blocks)
        : Workset1<T>(source, nblocks, blocks), _mean0(source._mean0), _std(source._std), _tail(source._tail)
    {
    }

    ZTestStdWorkset(const ZTestStdWorkset& source, const TCSize_t nblocks)
        : Workset1<T>(source, nblocks), _mean0(source._mean0), _std(source._std), _tail(source._tail)
    {
    }

    inline T STD()
    {
        return _std;
    }

    inline T Mean0()
    {
        return _mean0;
    }

protected:    
    const T _mean0;
    const T _std;
    const TCInt64 _tail;
};

template <typename Workset>
struct ZTestStdFinal
{
    static void Final(Workset& ws) 
    {
        for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
        {

            ws.Param0Element(ind) /= ws.Count(ind);

            if (ws.STD() == static_cast<Workset::in_t>(0))
            {
                ws.Param1Element(ind) = (ws.Param0Element(ind) <= ws.Mean0()) ? static_cast<Workset::in_t>(1) : static_cast<Workset::in_t>(0);
            }
            else
            {
                Workset::in_t sqrtN = Utils::sqrt((Workset::in_t)ws.Count(ind));
                Workset::in_t zscore = sqrtN * (ws.Param0Element(ind) - ws.Mean0()) / ws.STD();

                // complement of normal distribution
                ws.Param1Element(ind) = Utils::erfc(zscore / Utils::sqrt(static_cast<Workset::in_t>(2))) / (static_cast<Workset::in_t>(2));
            }
        }
    }
};


namespace Inference 
{   
    template <typename T>
    TCErrorCode stats_inference_z_test(const TCArray* sample, T mean, T standard_deviation, T* p_value)
    {
        if (standard_deviation < 0.0)
        {
            return tcerror_code_new(TCError_IllegalInput, 3);
        }

        const TCSize_t ndims = sample->m_ndims;

        if ((ndims == 0) || (ndims > 2))
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        const TCSize_t dim0 = sample->m_dims[0];

        if (dim0 == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef ReductionOperator<ZTestStdWorkset<T>, AveReduce, ZTestStdFinal, DoCount, AveCombine, OneInputReduce> AveOp;
        ZTestStdWorkset<T> ztest_stdws(mean, standard_deviation, 0);

        return reduce<OnePassReduction<AveOp> >(ztest_stdws, sample, 1, p_value);
    }

    template <typename T>
    TCErrorCode stats_inference_z_test(const TCArray* sample, T mean, T* p_value)
    {
        const TCSize_t ndims = sample->m_ndims;

        if ((ndims == 0) || (ndims > 2))
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        const TCSize_t dim0 = sample->m_dims[0];

        if (dim0 == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        typedef AverageOperator<ZTestWorkset<T> >::type AveOp;
        typedef ReductionOperator<ZTestWorkset<T>, STDReduce, ZTestFinal, NoCount, STDCombine, OneInputReduce> StdOp;
        typedef TwoPassReduction<AveOp, StdOp> ZtestOps;
        ZTestWorkset<T> ztest_ws(mean, 0);

        return reduce<ZtestOps>(ztest_ws, sample, 1, p_value);
    }
}