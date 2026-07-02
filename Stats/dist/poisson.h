



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"
#include "norm.h"

namespace Dist
{
    template <typename T>
    class Poisson
    {
    public:
        static T PDF(const T x, const T mean)
        {
            T result = 0;
            T logresult = 0;
    
            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(mean, result);

            
            if (mean < 0 || Utils::isinf(mean))
            {
                result = TCError::NewError<T>(mean,  TCFuncId_POISSON_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if (x < 0)
            {
                result = 0;
            }
            else if (Utils::isinf(x))
            {
                result = 0;
            }
            else
            {
                if (x != (T) Utils::floor(x))
                {
                    result = 0;
                }
                else
                {
                    
                    result = Utils::exp(-mean); 
                    if (result != 0)        
                    {
                        for (T idx = 1; idx <= x; idx++)
                        {
                            result *= mean/idx;
                            if (0 == result)
                            {
                                break;
                            }
                        }
                    }

                    else                 
                    {
                        T gammaln_val = Utils::gammaln(x + 1);
                        if (Utils::isinf(gammaln_val))  
                        {
                            T norm_mean = mean;
                            T norm_sigma = sqrt(mean);                            
                            result = Norm<T>::PDF(x, norm_mean, norm_sigma);
                        }
                        else
                        {
                            logresult = - mean + x * Utils::log(mean) - gammaln_val;
                            result = Utils::exp(logresult);
                        }
                    }

                }
            }


        exitlabel:
            return result;
        }

        static T CDF(const T x, const T mean)
        {
            T result = 0;
            T x_truncated = 0;
    
            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(mean, result);

            
            if (mean < 0 || Utils::isinf(mean))
            {
                result = TCError::NewError<T>(mean,  TCFuncId_POISSON_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if(x <0)
            {
                result = 0;
            }
            else if (Utils::isinf(x))
            {
                result = 1;
            }
            else
            {
                x_truncated = (T) Utils::floor(x);
                
                result = (T) Utils::gamma_regularized_q(1+x_truncated, mean);

                if (result > 1) result = 1;
                assert (0 <= result && result <= 1);
            }

        exitlabel:
            return result;
        }

        static T InvCDF(const T probability, const T mean)
        {
            T result = 0;
            T lower = 0;
            T upper = 1;
            const TCSize_t MAX_ITERATION_NUMBER = 1200; 
            TCSize_t iteration = 0;

            
            BAIL_ON_NAN(probability, result);
            BAIL_ON_NAN(mean, result);

            
            if (mean < 0 || Utils::isinf(mean))
            {
                result = TCError::NewError<T>(mean,  TCFuncId_POISSON_INV, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            
            if (probability < 0 || probability > 1)
            {
                result = TCError::NewError<T>(probability,  TCFuncId_POISSON_INV, TCArgPosition_1, TCError_IllegalInput);
                BAIL();
            }

            
            if (0 == mean)
            {
                result = 0;
                BAIL();
            }

            
            if (1 == probability)
            {
                assert ( 0 != mean);
                result = Utils::infinity<T>();
                BAIL();
            }
#if 0
                
                result = (T) Utils::gamma_regularized_q(2.8362596673541700e+278, 4.4942328371557894e+306);
#endif

            if (probability <= CDF(0, mean))
            {
                result = 0;
                BAIL();
            }

            lower = 0;
            upper = 1;

            
            do 
            {
                T cdf_upper = CDF(upper, mean);

                assert ( 0 <= cdf_upper && cdf_upper <= 1);
                if (! (cdf_upper < probability && Utils::isinf(upper) == false) )
                {
                    break;
                }
                
                lower = upper;
                upper = 2* lower +1;
            } 
            while (Utils::isinf(upper) == false);

            
            if (Utils::isinf(upper))
            {
                result = Utils::infinity<T>();
                BAIL();
            }

            assert (CDF(lower, mean) < probability && probability <= CDF(upper, mean));

            while(upper - lower >1 && iteration < MAX_ITERATION_NUMBER)
            {
                T middle = Utils::floor ( (lower + upper)/2);
                T cdf_middle = CDF(middle, mean);
                if (cdf_middle < probability)
                { 
                    lower = middle; 
                }
                else 
                { 
                    upper = middle;
                }
                iteration ++;
            }

            result = upper;

exitlabel:
            return result;
        }

    };

}