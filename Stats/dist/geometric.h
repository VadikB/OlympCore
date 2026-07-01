//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>
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

        if ((x >= fzero) &&                                       //special value and NaN verification 
            (p > fzero && p <= fone))                             //domain and NaN verification
        {
            if (Utils::isinf(x))      return fzero;               //special value check 
            if (Utils::floor(x) != x) return fzero;               //special value check
            if (x == 0) return p;
            return Utils::exp(x * Utils::log1p(-p)) * p;
        }
        if (Utils::isnan(p)) return p;                            //NaN return
        if (Utils::isnan(x)) return x;                            //NaN return
        if (p <= fzero || p > fone)                               //domain return
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        if (x < fzero) return fzero;                              //special value return for x < fzero
        return TCError::NewError<T>(x, TCFuncId_GEOMETRIC_DIST, TCArgPosition_UnKnown, TCError_Internal);
    }

    static T CDF(T x, T p) 
    {
        T fone=1, fzero=0;
        if ((x >= fzero) &&                                       //algorithm and NaN verification
            (p > fzero && p <= fone))                             //domain and NaN verification
        {
            if (Utils::isinf(x)) return fone;                     //algorithm or special value check 
            return -Utils::expm1((Utils::floor(x) + fone) * Utils::log1p(-p));
        }
        if (Utils::isnan(p)) return p;                            //NaN return
        if (Utils::isnan(x)) return x;                            //NaN return
        if (p <= fzero || p > fone)                               //domain return
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        if (x < 0) return fzero;                                  //special value return for x < fzero
        return TCError::NewError<T>(x, TCFuncId_GEOMETRIC_DIST, TCArgPosition_UnKnown, TCError_Internal);
    }

    static T InvCDF(T y, T p) 
    {
        T fone=1, fzero=0;
        if ((y >= fzero && y <= fone) &&                             //domain and NaN verification
            (p > fzero && p <= fone))                                //domain and NaN verification
        {
            if (p == fone) return fzero;
            if (y == fzero) return fzero;
            return static_cast<T> (Utils::ceil(Utils::log1p(-y)/Utils::log1p(-p)) - fone);
        }
        if (Utils::isnan(p)) return p;                               //NaN return
        if (Utils::isnan(y)) return y;                               //NaN return
        if (y < fzero || y > fone)                                   //domain check
        {
            return TCError::NewError<T>(y, TCFuncId_GEOMETRIC_INV, TCArgPosition_1, TCError_IllegalInput);
        }
        if (p <= fzero || p > fone)                                  //domain check
        {
            return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        return TCError::NewError<T>(p, TCFuncId_GEOMETRIC_INV, TCArgPosition_UnKnown, TCError_Internal);
    }
    
private:
    Geometric();
};      // class Geometric
}  // namespace Dist
