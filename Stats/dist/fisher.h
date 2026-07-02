



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Fisher
    {
    public:
        static T CDF(const T x)
        {
            if((x >= -1) &&
                (x <= 1))
            {
                return ((T)(Utils::log1p(x) - Utils::log1p(-x))) / T(2.0);
            }

            if(Utils::isnan(x)) return x;

            return Utils::quiet_NaN<T>(TCError_IllegalInput);
        }

        static T InvCDF(const T x)
        {
            if(Utils::isnan(x)) return x;
            if(x==0) return 0;

            return T(1.0) / (T(1.0) + T(2.0) / ((T)Utils::expm1(T(2.0) * x)));
        }
    };
}