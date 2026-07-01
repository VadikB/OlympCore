//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>
#pragma once

#include "OlympCoreUtils.h"

namespace Dist
{
template <typename T>
class Gumbel_Min
{
public:
    static T PDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (scale > 0 && !Utils::isinf(scale) && location == location && !Utils::isinf(location))   // propagate x == NaN
        {
            if (Utils::isinf(x))    return 0;
            const T z = Utils::exp((x - location) / scale);
            if (Utils::isinf(z))    return 0;
            const T probability = z * Utils::exp(-z) / scale;
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MIN_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        else                            // scale <= 0 || Utils::isinf(scale)
        {
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MIN_DIST, TCArgPosition_3, TCError_IllegalInput);
        }
    }

    static T CDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (scale > 0 && !(Utils::isinf(scale) || Utils::isinf(location)))   // propagate x == NaN and location == NaN
        {
            T z;
            if (Utils::isinf(x - location) && 
                !(Utils::isinf(location / scale) || Utils::isinf(x / scale)))
            {
                z = Utils::exp(x / scale - location / scale);
            }
            else
            {
                z = Utils::exp((x - location) / scale);
            }
            const T probability = -Utils::expm1(-z); 
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MIN_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        else                            // scale <= 0 || Utils::isinf(scale)
        {
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MIN_DIST, TCArgPosition_3, TCError_IllegalInput);
        }
    }

    static T InvCDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (x >= 0 && x <= 1 && scale > 0 && !(Utils::isinf(scale) || Utils::isinf(location)))
        {
            const T probability = location + scale * Utils::log(-Utils::log1p(-x));
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MIN_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        if (scale <= 0 || Utils::isinf(scale))
        {
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MIN_INV, TCArgPosition_3, TCError_IllegalInput);
        }

        assert(x < 0 || x > 1);             // x < 0 || x > 1
        return TCError::NewError<T>(x,  TCFuncId_GUMBEL_MIN_INV, TCArgPosition_1, TCError_IllegalInput);
    }
};      // class Gumbel_Min

template <typename T>
class Gumbel_Max
{
public:
    static T PDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (scale > 0 && !Utils::isinf(scale) && location == location && !Utils::isinf(location))   // propagate x == NaN
        {
            if (Utils::isinf(x))    return 0;  
            const T z = Utils::exp((location - x) / scale);
            if (Utils::isinf(z))    return 0;
            const T probability = z * Utils::exp(-z) / scale;
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MAX_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        else                            // scale <= 0 || Utils::isinf(scale)
        {
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MAX_DIST, TCArgPosition_3, TCError_IllegalInput);
        }
    }

    static T CDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (scale > 0 && !(Utils::isinf(scale) || Utils::isinf(location)))   // propagate x == NaN and location == NaN
        {
            T z;
            if (Utils::isinf(location - x) && 
                !(Utils::isinf(location / scale) || Utils::isinf(x / scale)))
            {
                z = Utils::exp(location / scale - x / scale);
            }
            else
            {
                z = Utils::exp((location - x) / scale);
            }
            const T probability = Utils::exp(-z);
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MAX_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        else                            // scale <= 0 || Utils::isinf(scale)
        {
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MAX_DIST, TCArgPosition_3, TCError_IllegalInput);
        }
    }

    static T InvCDF(const T x, const T location, const T scale) 
    {
        // Compute for valid inputs
        if (x >= 0 && x <= 1 && scale > 0 && !(Utils::isinf(scale) || Utils::isinf(location)))
        {
            const T probability = location - scale * Utils::log(-Utils::log(x));
            return probability;
        }

        // Check for NaN inputs
        if (Utils::isnan(location)) return location;
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;

        // Check for Out-of-domain inputs 
        if (Utils::isinf(location))
        {
            return TCError::NewError<T>(location,  TCFuncId_GUMBEL_MAX_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        if (scale <= 0 || Utils::isinf(scale))
        {
            return TCError::NewError<T>(scale,  TCFuncId_GUMBEL_MAX_INV, TCArgPosition_3, TCError_IllegalInput);
        }

        assert(x < 0 || x > 1);             // x < 0 || x > 1
        return TCError::NewError<T>(x,  TCFuncId_GUMBEL_MAX_INV, TCArgPosition_1, TCError_IllegalInput);
    }
};      // class Gumbel_Max
}       // namespace Dist