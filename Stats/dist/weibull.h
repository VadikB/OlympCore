//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Weibull
    {
    public:
        static T PDF(const T x, const T alpha, const T beta)
        {
            if((x >= 0) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_DIST, TCArgPosition_3, TCError_IllegalInput);

                if(Utils::isinf(x)) return 0;
                if(x==0)
                {
                    if(alpha>=1)
                    {
                        if(alpha==1)
                            return T(1.0) / beta;
                        else
                            return 0;
                    }
                    else
                    {
                        return Utils::infinity<T>();
                    }
                }

                T powXDivBetaAlpha;
                
                if(Utils::isinf(x/beta) || (x/beta==T(0.0)))
                {
                    powXDivBetaAlpha = Utils::exp(alpha*(Utils::log(x)-Utils::log(beta)));
                }
                else
                { 
                    powXDivBetaAlpha = Utils::pow(x/beta, alpha);
                }

                T ret = alpha / x * powXDivBetaAlpha * Utils::exp(-powXDivBetaAlpha);
                if(Utils::isnan(ret) || Utils::isinf(ret))
                {
                    ret = Utils::log(alpha) - T(1.0)*Utils::log(x) + alpha*(Utils::log(x)-Utils::log(beta)) - powXDivBetaAlpha;
                    if(Utils::isnan(ret))
                    {
                        //dominating powXDivBetaAlpha>0 -> exp(-powXDivBetaAlpha) = 0
                        return 0;
                    }
                    return Utils::exp(ret);
                }

                return ret;
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(x)) return x;

            if((Utils::isinf(alpha)) || (alpha <= 0)) return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_DIST, TCArgPosition_3, TCError_IllegalInput);

            return 0;
        }

        static T CDF(const T x, const T alpha, const T beta)
        {
            if((x >= 0) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_DIST, TCArgPosition_3, TCError_IllegalInput);

                if(Utils::isinf(x)) return 1;

                if(Utils::isinf(x/beta) || (x/beta==T(0.0)))
                {
                    return (T)- Utils::expm1(-Utils::exp(alpha*(Utils::log(x)-Utils::log(beta))));
                }

                return (T)- Utils::expm1(-Utils::pow(x/beta, alpha));
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(x)) return x;

            if((Utils::isinf(alpha)) || (alpha <= 0))return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_DIST, TCArgPosition_3, TCError_IllegalInput);

            return 0;
        }

        static T InvCDF(const T probability, const T alpha, const T beta)
        {
            if((probability >= T(0.0)) &&
                (probability <= T(1.0)) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_INV, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_INV, TCArgPosition_3, TCError_IllegalInput);

                return beta * Utils::pow(-Utils::log1p(-probability), ((T)1.0)/alpha);
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(probability)) return probability;

            if((Utils::isinf(alpha)) || (alpha <= 0))return TCError::NewError<T>(alpha, TCFuncId_WEIBULL_INV, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_WEIBULL_INV, TCArgPosition_3, TCError_IllegalInput);

            return TCError::NewError<T>(probability, TCFuncId_WEIBULL_INV, TCArgPosition_1, TCError_IllegalInput);
        }
    };
}