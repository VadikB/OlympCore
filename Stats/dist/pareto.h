//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Pareto
    {
    public:
        static T PDF(const T x, const T scale, const T shape)
        {
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(shape)) return shape;
            if(Utils::isnan(x)) return x;
            
            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_PARETO_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (shape <= 0 || Utils::isinf(shape))
            {
                return TCError::NewError<T>(shape, TCFuncId_PARETO_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            if (x < scale)
            {
                return 0;
            }
            else
            {
                T z = shape / x * Utils::pow(scale / x, shape);
                if (z == 0 || Utils::isnan(z) || Utils::isinf(z))
                {
                    z = Utils::pow((T)2.0, Utils::log2(shape) + shape * (Utils::log2(scale) - Utils::log2(x)) - Utils::log2(x));
                }
                return z;
            }
        }

        static T CDF(const T x, const T scale, const T shape)
        {
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(shape)) return shape;
            if(Utils::isnan(x)) return x;

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_PARETO_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (shape <= 0 || Utils::isinf(shape))
            {
                return TCError::NewError<T>(shape, TCFuncId_PARETO_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            //return x < scale ? 0 : (T)1.0 - Utils::pow(scale / x, shape);
            T z = shape * (Utils::log(scale) - Utils::log(x));
            return x < scale ? 0 : -Utils::expm1(z);
        }

        static T InvCDF(const T probability, const T scale, const T shape)
        {
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(shape)) return shape;
            if(Utils::isnan(probability)) return probability;

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_PARETO_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            if (shape <= 0 || Utils::isinf(shape))
            {
                return TCError::NewError<T>(shape, TCFuncId_PARETO_INV, TCArgPosition_3, TCError_IllegalInput);
            }
            
            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_PARETO_INV, TCArgPosition_1, TCError_IllegalInput);
            }

            //return scale / Utils::pow(1 - probability, 1 / shape);
            return scale * Utils::exp(- Utils::log1p(-probability) / shape);
        }
    };
}