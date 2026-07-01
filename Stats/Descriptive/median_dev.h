//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCCommon.h"
#include "TCArray.h"
#include "TCMath.h"

#include "median.h"

namespace Descriptive
{

template <typename T>
struct median_dev_t : public Sort::operator_temp_copy_t<T>
{
    inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
    {
        if (nelem == 0)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
            result = TCError::NewErrorImpl<T>::run(err);
            return err;
        }
        const T Inf = Utils::infinity<T>();
        T* data = state.data();
        // MEDIAN ( abs( X - MEDIAN(X) ) )
        T median_value = median<T>(data, nelem);
        if (TCError::IsErrorImpl<T>::run(&median_value))
        {
            result = median_value;
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        }
        else if (median_value == Inf)
        {
            result = TCError::quiet_NaN<T>();
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        }
        else if (median_value == -Inf)
        {
            result = TCError::quiet_NaN<T>();
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        }
        else
        {
            // median(x) is finite
            for (TCSize_t i = 0;  i < nelem; i++ )
            {
                T v = data[i];
                if (v == Inf || v == -Inf)
                {
                    data[i] = Inf;
                }
                else
                {
                    v = v - median_value;
                    if (v < 0)
                        v = -v;
                    data[i] = v;
                }
            }
        }
        // data contains abs deviations from median
        result = median<T>(data, nelem);
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}