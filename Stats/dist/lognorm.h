//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>
#pragma once


#include "OlympCoreUtils.h"

namespace Dist
{
template <typename T>
class Lognorm
{
public:
    static T PDF(const T x, const T mean, const T standard_dev) 
    {
        // Compute for valid inputs
        if (mean == mean && !Utils::isinf(mean)
            && standard_dev > 0 && !Utils::isinf(standard_dev))                 // propagate x == NaN
        {
            if (x <= 0)                                                         // handle special inputs
            {
                return 0;
            }
            const T factor = (Utils::log(x) - mean) / standard_dev;
            const T w = T(0.5) * factor * factor;
            T probability;
            if (w < T(80))                                                      // check for exp(-w) underflow
            {
                probability= Utils::exp(-w) / (x * standard_dev * Utils::sqrt(T(2) * T(Utils::pi())));
            }
            else
            {
                const T z = -w - Utils::log(x) - Utils::log(standard_dev) - T(0.5) * Utils::log(T(2) * T(Utils::pi()));
                probability = Utils::exp(z);
            }
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(mean))             return mean;
        if (Utils::isnan(standard_dev))     return standard_dev;
        if (Utils::isnan(x))                return x;
        
        // Check for Out-of-domain inputs   
        if (Utils::isinf(mean))
        {
            return TCError::NewError<T>(mean,  TCFuncId_LOGNORM_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(standard_dev <= 0 || Utils::isinf(standard_dev));    // standard_dev <= T(0) || Utils::isinf(standard_dev)
        return TCError::NewError<T>(standard_dev,  TCFuncId_LOGNORM_DIST, TCArgPosition_3, TCError_IllegalInput);
    }

    static T CDF(const T x, const T mean, const T standard_dev) 
    {
        // Compute for valid inputs
        if (mean == mean && !Utils::isinf(mean)
            && standard_dev > 0 && !Utils::isinf(standard_dev))                   // propagate x == NaN
        {
            if (x <= 0)                         // handle special inputs
            {
                return 0;
            }
            const T factor = (Utils::log(x) - mean) / (standard_dev * Utils::sqrt(T(2)));
            const T probability = T(0.5) * Utils::erfc(-factor);
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(mean))             return mean;
        if (Utils::isnan(standard_dev))     return standard_dev;
        if (Utils::isnan(x))                return x;
        
        // Check for Out-of-domain inputs   
        if (Utils::isinf(mean))
        {
            return TCError::NewError<T>(mean,  TCFuncId_LOGNORM_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(standard_dev <= 0 || Utils::isinf(standard_dev));    // standard_dev <= T(0) || Utils::isinf(standard_dev)
        return TCError::NewError<T>(standard_dev,  TCFuncId_LOGNORM_DIST, TCArgPosition_3, TCError_IllegalInput);
    }

    static T InvCDF(const T x, const T mean, const T standard_dev)  
    {
        // Compute for valid inputs 
        if (mean == mean && !Utils::isinf(mean) 
            && x >= 0 && x <= 1 && standard_dev > 0 && !Utils::isinf(standard_dev))  // propagate x == NaN
        {
            const T erf_factor = standard_dev * Utils::sqrt(T(2)) * Utils::erfcinv(T(2) * x); 
            const T probability = Utils::exp(mean - erf_factor);
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(mean))             return mean;
        if (Utils::isnan(standard_dev))     return standard_dev;
        if (Utils::isnan(x))                return x;
        
        // Check for Out-of-domain inputs   
        if (x < 0 || x > 1) 
        {
            return TCError::NewError<T>(x,  TCFuncId_LOGNORM_INV, TCArgPosition_1, TCError_IllegalInput);
        }
        if (Utils::isinf(mean))
        {
            return TCError::NewError<T>(mean,  TCFuncId_LOGNORM_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(standard_dev <= 0 || Utils::isinf(standard_dev));    // standard_dev <= T(0) || Utils::isinf(standard_dev)
        return TCError::NewError<T>(standard_dev,  TCFuncId_LOGNORM_INV, TCArgPosition_3, TCError_IllegalInput);
    }
};      // class Lognorm
}       // namespace Dist