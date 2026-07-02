



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class StudentT
    {
    private:
        static T DF(const T x, const T deg_freedom, const TCBool cumulative)
        {
            if(Utils::isnan(deg_freedom)) return deg_freedom;
            if(Utils::isnan(x)) return x;
            
            if (deg_freedom < 1 || Utils::isinf(deg_freedom))
            {
                return TCError::NewError<T>(deg_freedom, TCFuncId_T_DIST, TCArgPosition_2, TCError_IllegalInput);
            }
            
            T deg = Utils::floor(deg_freedom);
            T deg_half = deg / (T)2.0;
            T deg1_half = (deg + (T)1.0) / (T)2.0;
            T z;
            
            if (x > (T)0.0)
            {
                T tmp = Utils::sqrt(deg / (x * x) + (T)1.0);
                z = ((T)1.0 + tmp) / ((T)2.0 * tmp); 
            }
            else if (x < (T)0.0)
            {
                T tmp = Utils::sqrt(x * x + deg);
                
                z = deg_half / (x * x + deg - tmp * x);
            }
    
            if (cumulative)
            {
                if (x == (T)0.0)
                {
                    return (T)0.5;
                }
                else
                {
                    return Utils::betainc(z, deg_half, deg_half);
                }
            }
            else
            {
                {
                    return Utils::exp(Utils::gammaln(deg1_half) - deg1_half * Utils::log((T)1.0 + x * x /deg) - Utils::gammaln(deg_half) - Utils::log(Utils::sqrt(deg * (T)Utils::pi())));
                }
            }
        }

    public:
        static T PDF(const T x, const T deg_freedom)
        {
            return DF(x, deg_freedom, 0);
        }

        static T CDF(const T x, const T deg_freedom)
        {
            return DF(x, deg_freedom, 1);
        }

        static T Confidence(const T alpha, const T standard_dev, const T size)
        {
            if((alpha >= 0) &&
                (alpha <= 1) && 
                (standard_dev > 0) &&
                (size >= 2))
            {
                if(Utils::floor(size)==T(2.0))
                {
                    
                    
                    return Utils::cot(alpha*Utils::pi()/T(2.0)) * standard_dev
                        / Utils::sqrt(Utils::floor(size));
                }
                else if(Utils::floor(size)!=T(3.0))
                {
                    if(alpha<T(0.7))
                    {
                        
                        T deg_freedom = Utils::floor(size)-T(1.0);
                        T deg = Utils::floor(deg_freedom);
                        T deg_half = deg / T(2.0);

                        T invbeta = Utils::invbetainc(alpha, deg_half, (T)0.5);
                        T invcdf = Utils::sqrt(deg) * Utils::sqrt((T)1.0 / invbeta - (T)1.0);
                    
                        return invcdf * standard_dev / Utils::sqrt(Utils::floor(size));
                    }
                }

                return InvCDF((T(2.0)-alpha)/T(2.0), Utils::floor(size)-T(1.0)) * 
                    standard_dev / Utils::sqrt(Utils::floor(size));
            }

            if(Utils::isnan(standard_dev)) return standard_dev;
            if(Utils::isnan(size)) return size;
            if(Utils::isnan(alpha)) return alpha;

            return Utils::quiet_NaN<T>(TCError_IllegalInput);
        }

        static T InvCDF(const T probability, const T deg_freedom)
        {
            if(Utils::isnan(deg_freedom)) return deg_freedom;
            if(Utils::isnan(probability)) return probability;

            if (deg_freedom < 1 || Utils::isinf(deg_freedom))
            {
                return TCError::NewError<T>(deg_freedom, TCFuncId_T_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(deg_freedom, TCFuncId_T_INV, TCArgPosition_1, TCError_IllegalInput);
            }

            T deg = Utils::floor(deg_freedom);

            if (deg == (T)1.0)
            {
                if (probability == (T)1.0)
                {    
                    return Utils::infinity<T>(); 
                }
                else if (probability == (T)0.5) 
                {
                    return (T)0.0;
                }
                else 
                {    
                    return (T) -Utils::cot(probability * (T)Utils::pi());
                }
            } 
            else if (deg == (T)2.0)
            {
                return ((T)2.0 * probability - (T)1.0) / Utils::sqrt((T)2.0 * probability * ((T)1.0 - probability)); 
            }
            else
            {
                T deg_half = deg / (T)2.0;
                T invbeta;
                T result, comp_prob;
                
                if (probability == (T)0.5) 
                {
                    return (T)0.0;
                }
                else if (probability > (T)0.35 && probability < (T)0.65 )
                {
                    comp_prob = (probability < (T)0.5) ? (T)0.5 - probability : probability - (T)0.5;
                    invbeta = Utils::invbetainc((T)2.0 * comp_prob, (T)0.5, deg_half);
                    result = Utils::sqrt(deg) * Utils::sqrt(invbeta / ((T)1.0 - invbeta));
                }
                else
                {
                    comp_prob = (probability < (T)0.5) ? probability : (T)1.0 - probability;
                    invbeta = Utils::invbetainc((T)2.0 * comp_prob, deg_half, (T)0.5);
                    result = Utils::sqrt(deg) * Utils::sqrt((T)1.0 / invbeta - (T)1.0);
                }
                return (probability < (T)0.5) ? -result : result; 
            }
        }

        static T RT(const T x, const T deg_freedom)
        {
            return StudentT<T>::CDF(-x, deg_freedom);
        }

        static T InvRT(const T probability, const T deg_freedom)
        {
            return - StudentT<T>::InvCDF(probability, deg_freedom);
        }

        static T TwoT(const T x, const T deg_freedom)
        {
            if(Utils::isnan(deg_freedom)) return deg_freedom;

            if (x < (T)0.0)
            {
                return TCError::NewError<T>(x, TCFuncId_T_DIST, TCArgPosition_1, TCError_IllegalInput);
            }
            else if (x == (T)0.0)
            {
                return (T)1.0;
            }
            else
            {
                return (T)2.0 * StudentT<T>::CDF(-x, deg_freedom);
            }
        }

        static T InvTwoT(const T probability, const T deg_freedom)
        {
            if(Utils::isnan(deg_freedom)) return deg_freedom;
            
            if (probability < (T)0.0 || probability > (T)1.0)
            {
                return TCError::NewError<T>(probability, TCFuncId_T_INV, TCArgPosition_1, TCError_IllegalInput);
            }
            else
            {
                return - StudentT<T>::InvCDF((T)0.5 * probability, deg_freedom);
            }
        }

        static T Comp(const T x, const T deg_freedom, const T tails) 
        {
            if(Utils::isnan(deg_freedom)) return deg_freedom;
            if(Utils::isnan(tails)) return tails;
            
            T t = Utils::floor(tails);
            if ( x < (T)0.0 || t < (T)1.0 || t >= (T)3.0)
            {
                return Utils::quiet_NaN<T>();
            }
            else if (t == (T)1.0)
            {
                return StudentT<T>::RT(x, deg_freedom);
            }
            else
            {
                return StudentT<T>::TwoT(x, deg_freedom);
            }     
        }
    };
}