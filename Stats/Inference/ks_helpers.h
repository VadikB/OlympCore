//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include <TCMath.h>

namespace Inference
{
    template <typename T>
    static TCErrorCode math_sort_sequential(const TCArray* in_array, TCBool ascend,
        TCSize_t dim, TCArray* sorted_array)
    {
        static_assert("math_sort_sequential is specialized by wrong type.");
    }
    template <>
    static TCErrorCode math_sort_sequential<TCFloat>(const TCArray* in_array, TCBool ascend,
        TCSize_t dim, TCArray* sorted_array)
    {
        return math_sort_f_e(in_array, ascend, dim, sorted_array);
    }
    template <>
    static TCErrorCode math_sort_sequential<TCDouble>(const TCArray* in_array, TCBool ascend,
        TCSize_t dim, TCArray* sorted_array)
    {
        return math_sort_d_e(in_array, ascend, dim, sorted_array);
    }

    template <typename T>
    static bool skip_ties(T* data, TCSize_t& n)
    {
        bool res = false;
        T p = data[0];
        TCSize_t len = n;
        for (TCSize_t i = 1, j = 1; i < n; i++)
        {
            if (data[i] != p) {
                p = data[i];
                data[j++] = p;
            }
            else
            {
                len--;
            }
        }
        if (n != len)
            res = true;
        n = len;

        return res;
    }

    template <typename T>
    static TCErrorCode skip_missings(T* data, TCSize_t& n, TCArgumentID id)
    {
        // We need to check first element for -Inf,
        // and last for Inf and NaN.
        if (Utils::isinf(data[0]))
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }
        while(n > 0 && TCError::IsMissingImpl<T>::run(&data[n - 1]))
        {
            n--;
        }
        if (n > 0 && (Utils::isinf(data[n - 1]) || Utils::isnan(data[n - 1])))
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(id));
        }
        if (n == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(id));
        }

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
}