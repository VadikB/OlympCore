//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"


namespace Dist
{
    template <typename T>
    class Uniform
    {
    public:
        static T PDF(const T x, const T lower, const T upper)
        {
            if(Utils::isnan(lower)) return lower;
            if(Utils::isnan(upper)) return upper;
            if(Utils::isnan(x)) return x;

            if (lower >= upper || Utils::isinf(lower))
            {
                return TCError::NewError<T>(lower, TCFuncId_UNIFORM_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (Utils::isinf(upper))
            {
                return TCError::NewError<T>(upper, TCFuncId_UNIFORM_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            if (x >= lower && x <= upper)
            {
	            return static_cast<T>(1) / (upper - lower);
            }
            else
            {
                return 0;
            }
        }

        static T CDF(const T x, const T lower, const T upper)
        {
            if(Utils::isnan(lower)) return lower;
            if(Utils::isnan(upper)) return upper;
            if(Utils::isnan(x)) return x;

            if (lower >= upper || Utils::isinf(lower))
            {
                return TCError::NewError<T>(lower, TCFuncId_UNIFORM_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (Utils::isinf(upper))
            {
                return TCError::NewError<T>(upper, TCFuncId_UNIFORM_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            T mid = (lower + upper) / 2.0;
            if (x <= lower)
            {
                return 0;
            }
            else if (x >= upper)
            {
                return 1;
            }
            else if (x >= mid)
            {
                return (x - mid) / (upper- mid) / 2.0 + 0.5;

            }
            else
            {
                return (x - lower) / (mid - lower) / 2.0;

            }
        }

        static T InvCDF(const T probability, const T lower, const T upper)
        {
            if(Utils::isnan(lower)) return lower;
            if(Utils::isnan(upper)) return upper;
            if(Utils::isnan(probability)) return probability;

            if (lower >= upper || Utils::isinf(lower))
            {
                return TCError::NewError<T>(lower, TCFuncId_UNIFORM_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            if (Utils::isinf(upper))
            {
                return TCError::NewError<T>(upper, TCFuncId_UNIFORM_INV, TCArgPosition_3, TCError_IllegalInput);
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_UNIFORM_INV, TCArgPosition_1, TCError_IllegalInput);
            }
            else if (probability > 0.5)
            {
                return upper * probability + lower * (1-probability);
            }
            else
            {
                return lower + probability * (upper - lower);
            }
        }
    };
}