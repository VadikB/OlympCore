



#pragma once

#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "TCErrorCommon.h"

namespace TimeSeriesUtils
{
    typedef enum _TimeSeriesParallelMode
    {
        sequential,
    } TimeSeriesParallelMode;

    static inline bool TCArrayIs1D(TCArray* ts)
    {
        return (ts->m_ndims == 1);
    }

    template <typename T>
    static inline bool TCArray1DContainsMetaValues(TCArray* x)
    {
        TCSize_t n = x->m_numelt;
        T* xData = (T*)x->m_data;
        TCSSize_t stride = x->m_strides[0];
        using namespace TCError;

        for (TCSize_t i = 0; i < n; i++)
        {
            T val = xData[i*stride];
            if (IsMissing(&val) || IsErrorImpl<T>::run(&val) || isinf<T>(val))
                return true;
        }
        return false;
    }

    template <typename T>
    TCTypeTag argTypeTag()
    {
        static_assert("argTypeTag specialized by wrong type");
    }
    template <>
    TCTypeTag argTypeTag<TCFloat>()
    {
        return TC_FLOAT;
    }
    template <>
    TCTypeTag argTypeTag<TCDouble>()
    {
        return TC_DOUBLE;
    }
}
