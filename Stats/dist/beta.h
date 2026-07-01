//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class BetaBasic
    {
    public:
        static T CDF(const T x, const T alpha, const T beta)
        {
            if ( x <= T(0.0) )
            {
                return T(0.0);
            }
            if ( x >= T(1.0) )
            {
                return T(1.0);
            }

            return Utils::betainc(x, alpha, beta);
        }

        static T PDF(const T x, const T alpha, const T beta)
        {
            if ( x <= T(0.0) )
            {
                if(x == T(0.0))
                {
                    if(alpha > T(1.0))
                    {
                        return T(0.0);
                    }
                    else
                    {
                        if(alpha < T(1.0))
                        {
                            return Utils::infinity<T>();
                        }
                        else
                        {
                            return T(1.0)/Utils::beta(alpha, beta);
                        }
                    }
                }
                return T(0.0);
            }
            if ( x >= T(1.0) )
            {
                if(x == T(1.0))
                {
                    if(beta > T(1.0))
                    {
                        return T(0.0);
                    }
                    else
                    {
                        if(beta < T(1.0))
                        {
                            return Utils::infinity<T>();
                        }
                        else
                        {
                            return T(1.0)/Utils::beta(alpha, beta);
                        }
                    }
                }
                return T(0.0);
            }

            //gamma function is more precise than beta function
            //hence using gamma for lower ULP
            T ret = Utils::pow(x, alpha - T(1.0)) * Utils::pow(T(1.0) - x, beta - T(1.0))
                * Utils::gamma(alpha + beta) / Utils::gamma(alpha) / Utils::gamma(beta);

            if(Utils::isnan(ret))
            {
                ret = (alpha-T(1.0))*Utils::log(x) + (beta-T(1.0))*Utils::log1p(-x) +
                    Utils::gammaln(alpha + beta) - Utils::gammaln(alpha) - Utils::gammaln(beta);

                return Utils::exp(ret);
            }

            return ret;
        }
    };

    template <typename T>
    class Beta
    {
    public:
        static T CDF(const T x, const T alpha, const T beta, const T A, const T B)
        {
            if((A <= x) &&
                (x <= B) &&
                (A < B) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_BETA_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_BETA_DIST, TCArgPosition_3, TCError_IllegalInput);
                if(Utils::isinf(A)) return TCError::NewError<T>(A, TCFuncId_BETA_DIST, TCArgPosition_4, TCError_IllegalInput);
                if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_DIST, TCArgPosition_5, TCError_IllegalInput);

                return BetaBasic<T>::CDF((x - A) / (B - A), alpha, beta);
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(A)) return A;
            if(Utils::isnan(B)) return B;
            if(Utils::isnan(x)) return x;

            if((alpha <= 0) || (Utils::isinf(alpha))) return TCError::NewError<T>(alpha, TCFuncId_BETA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((beta <= 0) || (Utils::isinf(beta))) return TCError::NewError<T>(beta, TCFuncId_BETA_DIST, TCArgPosition_3, TCError_IllegalInput);
            if((A >= B) || (Utils::isinf(A))) return TCError::NewError<T>(A, TCFuncId_BETA_DIST, TCArgPosition_4, TCError_IllegalInput);
            if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_DIST, TCArgPosition_5, TCError_IllegalInput);

            if(x >= B) return 1;
            return 0;
        }

        static T PDF(const T x, const T alpha, const T beta, const T A, const T B)
        {
            if((A <= x) &&
                (x <= B) &&
                (A < B) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_BETA_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_BETA_DIST, TCArgPosition_3, TCError_IllegalInput);
                if(Utils::isinf(A)) return TCError::NewError<T>(A, TCFuncId_BETA_DIST, TCArgPosition_4, TCError_IllegalInput);
                if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_DIST, TCArgPosition_5, TCError_IllegalInput);

                return BetaBasic<T>::PDF((x - A) / (B - A), alpha, beta) / (B - A);
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(A)) return A;
            if(Utils::isnan(B)) return B;
            if(Utils::isnan(x)) return x;

            if((alpha <= 0) || (Utils::isinf(alpha))) return TCError::NewError<T>(alpha, TCFuncId_BETA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((beta <= 0) || (Utils::isinf(beta))) return TCError::NewError<T>(beta, TCFuncId_BETA_DIST, TCArgPosition_3, TCError_IllegalInput);
            if((A >= B) || (Utils::isinf(A))) return TCError::NewError<T>(A, TCFuncId_BETA_DIST, TCArgPosition_4, TCError_IllegalInput);
            if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_DIST, TCArgPosition_5, TCError_IllegalInput);

            return 0;
        }

        static T InvCDF(const T probability, const T alpha, const T beta, const T A, const T B)
        {
            if((probability >= 0) &&
                (probability <= 1) &&
                (A < B) &&
                (alpha > 0) &&
                (beta > 0))
            {
                if(Utils::isinf(alpha)) return TCError::NewError<T>(alpha, TCFuncId_BETA_INV, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(beta)) return TCError::NewError<T>(beta, TCFuncId_BETA_INV, TCArgPosition_3, TCError_IllegalInput);
                if(Utils::isinf(A)) return TCError::NewError<T>(A, TCFuncId_BETA_INV, TCArgPosition_4, TCError_IllegalInput);
                if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_INV, TCArgPosition_5, TCError_IllegalInput);

                const T tmp = Utils::invbetainc(probability, alpha, beta);

                return A + tmp * (B - A);
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(A)) return A;
            if(Utils::isnan(B)) return B;
            if(Utils::isnan(probability)) return probability;

            if((alpha <= 0) || (Utils::isinf(alpha))) return TCError::NewError<T>(alpha, TCFuncId_BETA_INV, TCArgPosition_2, TCError_IllegalInput);
            if((beta <= 0) || (Utils::isinf(beta))) return TCError::NewError<T>(beta, TCFuncId_BETA_INV, TCArgPosition_3, TCError_IllegalInput);
            if((A >= B) || (Utils::isinf(A))) return TCError::NewError<T>(A, TCFuncId_BETA_INV, TCArgPosition_4, TCError_IllegalInput);
            if(Utils::isinf(B)) return TCError::NewError<T>(B, TCFuncId_BETA_INV, TCArgPosition_5, TCError_IllegalInput);

            return TCError::NewError<T>(probability, TCFuncId_BETA_INV, TCArgPosition_1, TCError_IllegalInput);
        }
    };
}