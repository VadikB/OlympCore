


#pragma once

#include "OlympCoreUtils.h"

namespace Dist
{
template <typename T>
class Rayleigh
{
public:
    static T PDF(const T x, const T scale) 
    {
        
        if (scale > 0 && !Utils::isinf(scale))                  
        {
            if (x <= 0 || Utils::isinf(x))                      
            {
                return 0;
            }
            const T z = x / scale;
            T probability = (z / scale) * Utils::exp(-(z * z) / T(2));
            if (Utils::isnan(probability))
            {
                const T y = T(2) * (Utils::log2(x) - Utils::log2(scale)) - T(1);
                const T log2exp1 = T(1.4426950408889634074);
                probability = Utils::pow(2, Utils::log2(x) - T(2) * Utils::log2(scale) - Utils::pow(2, y) * log2exp1);
            }
            return probability;
        }

        
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;
        
        
        assert(scale <= 0 || Utils::isinf(scale));              
        return TCError::NewError<T>(scale,  TCFuncId_RAYLEIGH_DIST, TCArgPosition_2, TCError_IllegalInput);
    }

    static T CDF(const T x, const T scale) 
    {
        
        if (scale > 0 && !Utils::isinf(scale))                  
        {
            if (x <= 0)                      
            {
                return 0;
            }
            const T z = x / scale;
            const T probability = (T)-Utils::expm1(-(z * z) / T(2));
            return probability;
        }

        
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;
        
        
        assert(scale <= 0 || Utils::isinf(scale));              
        return TCError::NewError<T>(scale,  TCFuncId_RAYLEIGH_DIST, TCArgPosition_2, TCError_IllegalInput);
    }

    static T InvCDF(const T x, const T scale) 
    {
        
        if (x >= 0 && x <= 1 && scale > 0 && !Utils::isinf(scale))                  
        {
            const T probability = scale * Utils::sqrt(-T(2) * Utils::log1p(-x));
            return probability;
        }

        
        if (Utils::isnan(scale))    return scale;
        if (Utils::isnan(x))        return x;
        
        
        if (x < 0 || x > 1)
        {
            return TCError::NewError<T>(x,  TCFuncId_RAYLEIGH_INV, TCArgPosition_1, TCError_IllegalInput);
        }
        assert(scale <= 0 || Utils::isinf(scale));              
        return TCError::NewError<T>(scale,  TCFuncId_RAYLEIGH_INV, TCArgPosition_2, TCError_IllegalInput);
    }
};      
}       