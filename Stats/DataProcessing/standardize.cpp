//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

template <typename T>
static T stats_data_standardize(const T x, const T mean, const T standard_dev)
{
    // Compute for valid inputs
    if (standard_dev > 0 && !Utils::isinf(mean) && !Utils::isinf(standard_dev)) // propagate x == NaN
    {
        T result = (x - mean)/standard_dev;
        // Alternative formulation to deal with spurious infinities
        if (Utils::isinf(result))
        {
            T betterResult = x / standard_dev - mean / standard_dev;
            if (!Utils::isnan(betterResult))
            {
                return betterResult;
            }
        }
        return result;
    }

    // Check for nan inputs
    if (Utils::isnan(mean))         return mean;
    if (Utils::isnan(standard_dev)) return standard_dev;
    if (Utils::isnan(x))            return x;

    // Check of out-of-domain inputs
    if (Utils::isinf(mean))
    {
        return TCError::NewError<T>(mean,  TCFuncId_STANDARDIZE, TCArgPosition_2, TCError_IllegalInput);
    }

    assert(standard_dev <= (T)0 || Utils::isinf(standard_dev)); 
    return TCError::NewError<T>(standard_dev,  TCFuncId_STANDARDIZE, TCArgPosition_3, TCError_IllegalInput);
}

TCFloat stats_data_standardize_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev)
{
    return stats_data_standardize<TCFloat>(x, mean, standard_dev);
}

TCDouble stats_data_standardize_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev)
{
    return stats_data_standardize<TCDouble>(x, mean, standard_dev);
}


