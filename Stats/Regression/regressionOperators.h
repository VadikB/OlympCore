//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "TCReduce.h"
#include "TCTypes.h"
#include "TCArray.h"
#include <vector>

#pragma once

template<typename T> 
void reshape(TCSize_t numElements, const T *orig, T *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone);

void SetRowsCols(TCSize_t &rows, TCSize_t &cols, const TCArray *known);

template <typename T>
class AverageSlopeOperator : public reduction_operator_t<T>
{
public:
    AverageSlopeOperator(T zero)
        : reduction_operator_t<T>(static_cast<T>(zero)), _count(0), _kahan()
    {
    }

    AverageSlopeOperator(const AverageSlopeOperator& source, const TCSize_t nblocks, T* const blocks)
        : reduction_operator_t<T>(source, nblocks, blocks), _count(0), _kahan()
    {
    }

    AverageSlopeOperator(const AverageSlopeOperator& source, const TCSize_t nblocks)
        : reduction_operator_t<T>(source, nblocks), _count(0), _kahan()
    {
    }

    inline void Reduce(const T& x, const T& y, T , T )
    {
        if (!is_missing(x) && !is_missing(y))
        {    
            _kahan.DoKahanOperation(x, BlockElement(0));
            _count++;
        }
    }

    inline void Final()
    {
        T tmp = tc_static_cast<T>(0);
        tmp = tmp + tc_static_cast<T>(_count);

        for (TCSize_t ind = 0; ind < NumBlocks(); ++ind)
        {
            BlockElement(ind) = BlockElement(ind) / tmp;
        }
    }
private:
    TCSize_t _count;
    kahan_operator_t<T> _kahan;
};

template <typename T>
class SumOperator : public reduction_operator_t<T>
{
public:
    SumOperator(T zero)
        : reduction_operator_t<T>(static_cast<T>(zero)), _kahan()
    {
    }

    SumOperator(const SumOperator& source, const TCSize_t nblocks, T* const blocks)
        : reduction_operator_t<T>(source, nblocks, blocks), _kahan()
    {
    }

    SumOperator(const SumOperator& source, const TCSize_t nblocks)
        : reduction_operator_t<T>(source, nblocks), _kahan()
    {
    }

    inline void Reduce(const T& x, const T& y, T , T )
    {
        if (!is_missing(x) && !is_missing(y))
        {    
            _kahan.DoKahanOperation(x, BlockElement(0));
        }
    }

    inline void Final()
    {
    }
private:
    kahan_operator_t<T> _kahan;
};

template <typename T>
class SumSqSlopeOperator : public reduction_operator_t<T>
{
public:
    SumSqSlopeOperator(T zero)
        : reduction_operator_t<T>(static_cast<T>(zero)), _kahan()
    {
    }

    SumSqSlopeOperator(const SumSqSlopeOperator& source, const TCSize_t nblocks, T* const blocks)
        : reduction_operator_t<T>(source, nblocks, blocks), _kahan()
    {
    }

    SumSqSlopeOperator(const SumSqSlopeOperator& source, const TCSize_t nblocks)
        : reduction_operator_t<T>(source, nblocks), _kahan()
    {
    }

    inline void Reduce(const T& x, const T& y, T valuex, T )
    {
        if (!is_missing(x) && !is_missing(y))
        {    
            const T sq = (x - valuex) * (x - valuex);
            _kahan.DoKahanOperation(sq, BlockElement(0));
        }
    }

    inline void Final()
    {
    }
private:
    kahan_operator_t<T> _kahan;
};

template <typename T>
class SumMultSlopeOperator : public reduction_operator_t<T>
{
public:
    SumMultSlopeOperator(T zero)
        : reduction_operator_t<T>(static_cast<T>(zero)), _kahan()
    {
    }

    SumMultSlopeOperator(const SumMultSlopeOperator& source, const TCSize_t nblocks, T* const blocks)
        : reduction_operator_t<T>(source, nblocks, blocks), _kahan()
    {
    }

    SumMultSlopeOperator(const SumMultSlopeOperator& source, const TCSize_t nblocks)
        : reduction_operator_t<T>(source, nblocks), _kahan()
    {
    }

    inline void Reduce(const T& x, const T& y, T valuex, T valuey)
    {
        if (!is_missing(x) && !is_missing(y))
        {    
            const T sq = (x - valuex) * (y - valuey);
            _kahan.DoKahanOperation(sq, BlockElement(0));
        }
    }

    inline void Final()
    {
    }
private:
    kahan_operator_t<T> _kahan;
};

template <typename T>
class CountOp : public reduction_operator_t<T>
{
public:
    CountOp(T zero)
        : reduction_operator_t<T>(static_cast<T>(zero))
    {
    }

    CountOp(const CountOp& source, const TCSize_t nblocks, T* const blocks)
        : reduction_operator_t<T>(source, nblocks, blocks)
    {
    }

    CountOp(const CountOp& source, const TCSize_t nblocks)
        : reduction_operator_t<T>(source, nblocks)
    {
    }

    inline void Final()
    {
    }

    inline void Reduce(const T& x, const T& y, T , T )
    {
        const T one = tc_static_cast<T>(1);

        if (!is_missing(x) && !is_missing(y))
        {    
            BlockElement(0) += one;
        }
    }
};