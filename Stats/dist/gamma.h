



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
                    T n1 = 1/x;  
                    T n2 = Utils::exp(-x / beta);
                    T d1 = 1/alpha;  
                    T d2 = Utils::pow(beta, alpha);
                     ret = (n1/d1)*(n2/d2);
                } else if (alpha > 19 || beta < 1e-30) {
                    
                    
                    
                    
                    T n1 = (alpha - (T) 1)*Utils::log(x);
                    T n2 = -x/beta;
                    T d1 = Utils::gammaln(alpha);
                    T d2 = alpha*Utils::log(beta);
                    ret = Utils::exp(n1 + n2 - d1 - d2);
                    if ( Utils::isnan(ret) ) {
                        if (Utils::isinf(d1) && (alpha > Utils::log(x)) ) {
                            
                            
                            
                            ret = (T) 0;
                        }
                    } 
                } else {
                    
                    T n1 = Utils::pow(x, alpha-T(1.0));
                    T n2 = Utils::exp(-x / beta);
                    T d1 = Utils::gamma(alpha);
                    T d2 = Utils::pow(beta, alpha);
                    ret = (n1/d1)*(n2/d2);  
                }
                return ret;
            }

            if(Utils::isnan(alpha)) return alpha;
            if(Utils::isnan(beta)) return beta;
            if(Utils::isnan(x)) return x;

            if((Utils::isinf(alpha)) || (alpha <= 0)) return TCError::NewError<T>(alpha, TCFuncId_GAMMA_DIST, TCArgPosition_2, TCError_IllegalInput);
            if((Utils::isinf(beta)) || (beta <= 0)) return TCError::NewError<T>(beta, TCFuncId_GAMMA_DIST, TCArgPosition_3, TCError_IllegalInput);

            return 0;
        }  

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
        }  

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
    };  
}