//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Norm
    {
    public:
        static T CDF(const T x, const T mean, const T standard_dev)
        {
            if((standard_dev > 0) &&
                (x == x) &&
                (mean == mean))
            {
                if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(standard_dev)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_DIST, TCArgPosition_3, TCError_IllegalInput);

                return (Utils::erfc(-(x-mean) / Utils::sqrt(T(2.0)) / standard_dev)) / T(2.0);
            }

            if(Utils::isnan(mean)) return mean;
            if(Utils::isnan(standard_dev)) return standard_dev;
            if(Utils::isnan(x)) return x;

            if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(standard_dev)) || (standard_dev<=0)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_DIST, TCArgPosition_3, TCError_IllegalInput);

            return TCError::NewError<T>(x, TCFuncId_NORM_DIST, TCArgPosition_1, TCError_IllegalInput);
        }

        static T PDF(const T x, const T mean, const T standard_dev)
        {
            if((standard_dev > 0) &&
                (x == x) &&
                (mean == mean))
            {
                if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(standard_dev)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_DIST, TCArgPosition_3, TCError_IllegalInput);

                return Utils::exp(-Utils::pow((x-mean) / standard_dev, T(2.0)) / T(2.0))
                    / standard_dev / Utils::sqrt(T(2.0) * Utils::pi());
            }

            if(Utils::isnan(mean)) return mean;
            if(Utils::isnan(standard_dev)) return standard_dev;
            if(Utils::isnan(x)) return x;

            if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(standard_dev)) || (standard_dev<=0)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_DIST, TCArgPosition_3, TCError_IllegalInput);

            return TCError::NewError<T>(x, TCFuncId_NORM_DIST, TCArgPosition_1, TCError_IllegalInput);
        }

        static T Confidence(const T alpha, const T standard_dev, const T size)
        {
            if((alpha >= 0) &&
                (alpha <= 1) &&
                (standard_dev > 0) &&
                (size >= 1))
            {
                if(Utils::isinf(standard_dev)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_INV, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(size)) return TCError::NewError<T>(size, TCFuncId_NORM_INV, TCArgPosition_3, TCError_IllegalInput);

                return standard_dev * Utils::sqrt(T(2.0)) * Utils::erfinv(T(1.0)-alpha)
                    /Utils::sqrt(Utils::floor(size));
            }

            if(Utils::isnan(standard_dev)) return standard_dev;
            if(Utils::isnan(size)) return size;
            if(Utils::isnan(alpha)) return alpha;

            if((Utils::isinf(standard_dev)) || (standard_dev<=0)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_INV, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(size)) || (size < 1)) return TCError::NewError<T>(size, TCFuncId_NORM_INV, TCArgPosition_3, TCError_IllegalInput);

            return TCError::NewError<T>(alpha, TCFuncId_NORM_INV, TCArgPosition_1, TCError_IllegalInput);
        }

        static T InvCDF(const T x, const T mean, const T standard_dev)
        {
            if((standard_dev > 0) &&
                (x >= 0) &&
                (x <= 1) &&
                (mean == mean))
            {
                if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_INV, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(standard_dev)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_INV, TCArgPosition_2, TCError_IllegalInput);

                return mean - standard_dev * Utils::sqrt(T(2.0)) * Utils::erfcinv(T(2.0)*x);
            }

            if(Utils::isnan(mean)) return mean;
            if(Utils::isnan(standard_dev)) return standard_dev;
            if(Utils::isnan(x)) return x;

            if(Utils::isinf(mean)) return TCError::NewError<T>(mean, TCFuncId_NORM_INV, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(standard_dev)) || (standard_dev<=0)) return TCError::NewError<T>(standard_dev, TCFuncId_NORM_INV, TCArgPosition_3, TCError_IllegalInput);
            
            return TCError::NewError<T>(x, TCFuncId_NORM_INV, TCArgPosition_1, TCError_IllegalInput);
        }
    };
}