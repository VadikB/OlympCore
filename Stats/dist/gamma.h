//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"
#include <iostream>

namespace Dist
{
    template <typename T>
    class Gamma
    {
    public:
        static T PDF(const T x, const T alpha, const T beta)
        {
            if((x >= 0) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);
                if(x == T(0.0)) 
                {
                    if(alpha>=1.0)
                    {
                        return T(0.0);
                    }
                    else
                    {
                        return Utils::infinity<T>();
                    }
                }

                if(Utils::isinf(x)) {
                    return T(0.0);
                }

                T ret;
                if (alpha < 1e-20) {
                    T n1 = 1/x;  // valid for very small alpha
                    T n2 = Utils::exp(-x / beta);
                    T d1 = 1/alpha;  // For every small alpha, gamma(alpha) -> 1/alpha
                    T d2 = Utils::pow(beta, alpha);
                     ret = (n1/d1)*(n2/d2);
                } else if (alpha > 19 || beta < 1e-30) {
                    // Use log version for extreme alpha or beta.  This helps prevent
                    // NaN returns when intermediate expressions underflow or overflow.
                    // The computation is slower, but if the user provides unreasonable inputs,
                    // then he can't expect good performance.
                    T n1 = (alpha - (T) 1)*Utils::log(x);
                    T n2 = -x/beta;
                    T d1 = Utils::gammaln(alpha);
                    T d2 = alpha*Utils::log(beta);
                    ret = Utils::exp(n1 + n2 - d1 - d2);
                    if ( Utils::isnan(ret) ) {
                        if (Utils::isinf(d1) && (alpha > Utils::log(x)) ) {
                            // In case we get a NaN, it's generally because d1 is infinite.
                            // Since gammaln(alpha) grows extrememly fast, the inf in the denom
                            // overwhelms everything else, and the return should be 0.
                            ret = (T) 0;
                        }
                    } 
                } else {
                    // Use the straight version for reasonable inputs.
                    T n1 = Utils::pow(x, alpha-T(1.0));
                    T n2 = Utils::exp(-x / beta);
                    T d1 = Utils::gamma(alpha);
                    T d2 = Utils::pow(beta, alpha);
                    ret = (n1/d1)*(n2/d2);  // Control order of division/multiplication to balance num and demon and prevent under/overflow.
                }
                return ret;
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(x)) return x;

            if((Utils::isinf(alpha)) || (alpha <= 0)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);

            return 0;
        }  // PDF

        static T CDF(const T x, const T alpha, const T beta)
        {
            if((x >= 0) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);

                const T xdbeta = x / beta;
                if((xdbeta == 0) && (x != 0))
                {
                    return Utils::exp(alpha*(Utils::log(x)-Utils::log(beta))-Utils::log(alpha))/Utils::gamma(alpha);
                }

                return Utils::gammainc(alpha, xdbeta);
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(x)) return x;

            if((Utils::isinf(alpha)) || (alpha <= 0)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);

            return 0;
        }  // CDF

        static T InvCDF(const T probability, const T alpha, const T beta)
        {
            if((probability >= T(0.0)) &&
                (probability <= T(1.0)) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_INV, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_INV, TCArgPosition_3, TCError_IllegalInput);
                return Utils::gamma_regularized_p_inv(alpha, probability)*beta;
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(probability)) return probability;

            if((Utils::isinf(alpha)) || (alpha <= 0)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);

            return TCError::NewError<T>(probability, TCFuncId_GAMMA_INV, TCArgPosition_1, TCError_IllegalInput);

        }
    };  //  InvCDF
}