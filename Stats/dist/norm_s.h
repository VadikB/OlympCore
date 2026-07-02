



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Norm_S
    {
    public:
        static T CDF(const T z)
        {
            if(Utils::isnan(z)) return z;
            return Utils::erfc(-z / Utils::sqrt(T(2.0))) / T(2.0);
        }

        static T PDF(const T z)
        {
            if(Utils::isnan(z)) return z;
            return Utils::exp(- z * z / T(2.0)) / Utils::sqrt(T(2.0) * Utils::pi());
        }

        static T InvCDF(const T probability)
        {
            if(Utils::isnan(probability)) return probability;
            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_NORM_S_INV, TCArgPosition_1, TCError_IllegalInput);
            }
            
            return -Utils::sqrt(T(2.0)) * Utils::erfcinv(T(2.0)*probability);
        }
    };
}