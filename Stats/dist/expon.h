



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Expon
    {
    public:
        static T PDF(const T x, const T lambda)
        {
            T result = 0;
            T y = 0;
    
            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(lambda, result);

            
            if (lambda <= 0 || Utils::isinf(lambda))
            {
                result = TCError::NewError<T>(lambda,  TCFuncId_EXPON_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            y = -lambda * x;
            if (x < 0)
            {
                result = 0;
            }
            else
            {
                result = lambda * Utils::exp(y);
            }
        
        exitlabel:
            return result;
        }

        static T CDF(const T x, const T lambda)
        {
            T result = 0;
            T y = 0;
    
            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(lambda, result);

            
            if (lambda <= 0 || Utils::isinf(lambda))
            {
                result = TCError::NewError<T>(lambda,  TCFuncId_EXPON_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            y = -lambda * x;

            if(x < 0)
            {
                result = 0;
            }
            else
            {
                result = -Utils::expm1(y);
            }

        exitlabel:
            return result;
        }

        static T InvCDF(const T probability, const T lambda)
        {
            T result = 0;

            
            BAIL_ON_NAN(probability, result);
            BAIL_ON_NAN(lambda, result);

            
            if (lambda <= 0 || Utils::isinf(lambda))
            {
                result = TCError::NewError<T>(lambda,  TCFuncId_EXPON_INV, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            
            if (probability < 0 || probability > 1)
            {
                result = TCError::NewError<T>(probability,  TCFuncId_EXPON_INV, TCArgPosition_1, TCError_IllegalInput);
                BAIL();
            }

            result = - T(1.00)/lambda * Utils::log1p(-probability);

        exitlabel:
            return result;
        }
    };

}