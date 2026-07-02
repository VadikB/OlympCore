


#pragma once

#include "OlympCoreUtils.h"
#include <limits>
using namespace std;

namespace Dist
{
template <typename T>
class Geometric
{
public:

    static T PMF(T x, T p) 
    {
        T fone=1, fzero=0;

        if ((x >= fzero) &&                                       
            (p > fzero && p <= fone))                             
        {
            if (Utils::isinf(x))      return fzero;               
            if (Utils::floor(x) != x) return fzero;               
            if (x == 0) return p;
            return Utils::exp(x * Utils::log1p(-p)) * p;
        }
        if (Utils::isnan(p)) return p;                            
        if (Utils::isnan(x)) return x;                            
        if (p <= fzero || p > fone)                               
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        if (x < fzero) return fzero;                              
        return TCError::NewError<T>(x, TCFuncId_GEOMETRIC_DIST, TCArgPosition_UnKnown, TCError_Internal);
    }

    static T CDF(T x, T p) 
    {
        T fone=1, fzero=0;
        if ((x >= fzero) &&                                       
            (p > fzero && p <= fone))                             
        {
            if (Utils::isinf(x)) return fone;                     
            return -Utils::expm1((Utils::floor(x) + fone) * Utils::log1p(-p));
        }
        if (Utils::isnan(p)) return p;                            
        if (Utils::isnan(x)) return x;                            
        if (p <= fzero || p > fone)                               
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        if (x < 0) return fzero;                                  
        return TCError::NewError<T>(x, TCFuncId_GEOMETRIC_DIST, TCArgPosition_UnKnown, TCError_Internal);
    }

    static T InvCDF(T y, T p) 
    {
        T fone=1, fzero=0;
        if ((y >= fzero && y <= fone) &&                             
            (p > fzero && p <= fone))                                
        {
            if (p == fone) return fzero;
            if (y == fzero) return fzero;
            return static_cast<T> (Utils::ceil(Utils::log1p(-y)/Utils::log1p(-p)) - fone);
        }
        if (Utils::isnan(p)) return p;                               
        if (Utils::isnan(y)) return y;                               
        if (y < fzero || y > fone)                                   
        {
            return TCError::NewError<T>(y, TCFuncId_GEOMETRIC_INV, TCArgPosition_1, TCError_IllegalInput);
        }
        if (p <= fzero || p > fone)                                  
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_INV, TCArgPosition_UnKnown, TCError_Internal);
    }
    
private:
    Geometric();
};      
}  
