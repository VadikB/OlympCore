//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Laplace
    {
    public:
        static T PDF(const T x, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(x)) return x;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_LAPLACE_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_LAPLACE_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            T z = (x - location) / scale;
            if (x < location)
            {
                return (T)0.5 * Utils::exp (z) / scale;
            }
            else
            {
                return (T)0.5 * Utils::exp(-z) / scale;
            }
        }

        static T CDF(const T x, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(x)) return x;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_LAPLACE_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_LAPLACE_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            T z = (x - location) / scale;
            if (x < location)
            {
                return (T)0.5 * Utils::exp(z);  
            }
            else
            {
                return(T)0.5 - (T)0.5 * Utils::expm1(-z);
            }
        }

        static T InvCDF(const T probability, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(probability)) return probability;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_LAPLACE_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_LAPLACE_INV, TCArgPosition_3, TCError_IllegalInput);
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_LAPLACE_INV, TCArgPosition_1, TCError_IllegalInput);
            }
    
            if (probability < (T)0.5)
            {
                return location + scale * Utils::log((T)2.0 * probability);
            }
            else
            {
                return location - scale * (Utils::log((T)2.0) + Utils::log1p(- probability));
            }
        }
    };
}