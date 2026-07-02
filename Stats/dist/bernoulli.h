



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Bernoulli
    {
    public:
        static T PMF(const T x, const T success_probability)
        {
            T result = 0;
    
            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(success_probability, result);

            
            if (success_probability < 0 || success_probability > 1)
            {
                result = TCError::NewError<T>(success_probability,  TCFuncId_BERNOULLI_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if (0 == x)
            {
                result = 1 - success_probability;
            }
            else if (1 == x)
            {
                result = success_probability;
            }
            else
            {
                result = 0;
            }

        exitlabel:
            return result;

        }

        static T CDF(const T x, const T success_probability)
        {
            T result = 0;

            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(success_probability, result);

            
            if (success_probability < 0 || success_probability > 1)
            {
                result = TCError::NewError<T>(success_probability,  TCFuncId_BERNOULLI_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if(x < 0)
            {
                result = 0;
            }
            else if (x >= 0 && x < 1)
            {
                result = 1 - success_probability;
            }
            else
            {
                assert (x >= 1);
                result = 1;
            }

        exitlabel:
            return result;
        }

        static T InvCDF(const T probability, const T success_probability)
        {
            T result = 0;

            
            BAIL_ON_NAN(probability, result);
            BAIL_ON_NAN(success_probability, result);

            
            if (success_probability < 0 || success_probability > 1)
            {
                result = TCError::NewError<T>(success_probability,  TCFuncId_BERNOULLI_INV, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            
            if (probability < 0 || probability > 1)
            {
                result = TCError::NewError<T>(probability,  TCFuncId_BERNOULLI_INV, TCArgPosition_1, TCError_IllegalInput);
                BAIL();
            }

            if (probability <= 1 - success_probability)
            {
                result = 0;
            }
            else
            {
                result = 1;
            }

        exitlabel:
            return result;
        }
    };

}

