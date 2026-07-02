


#pragma once

#include "OlympCoreUtils.h"

#define MAX(a, b)   ((a) < (b) ? (b) : (a))
#define MIN(a, b)   ((a) > (b) ? (b) : (a))
#define ROUND(a)    ((TCInt64)((a)+0.5))

namespace Dist
{
template <typename T>
class Hypgeom
{
private:
    
    
    static __inline T ComputePMF(const T x, const T n, const T M, const T N) 
    {
        T param1 = Utils::combin(n, x);
        T param2 = Utils::combin(N - n, M - x);
        T param3 = Utils::combin(N, M);
        if (Utils::isinf(param3) || Utils::isinf(param2) || Utils::isinf(param1))
        {
            T lnparam3, lnparam2, lnparam1;
            if (n == 0.0 || N == n)
            {
                lnparam3 = 0.0;
            }
            else if (n == 1.0 || (N - n) == 1.0)
            {
                lnparam3 = Utils::log(N);
            }
            else
            {
                lnparam3 = Utils::gammaln(N + 1) - (Utils::gammaln(n + 1) + Utils::gammaln(N - n + 1));
            }

            T t = N - M - n + x;
            if (n == x || t == 0.0) 
            {
                lnparam2 = 0.0;
            }
            else if ((n-x) == 1.0 || t == 1.0)
            {
                lnparam2 = Utils::log(N - M);
            }
            else
            {
                lnparam2 = Utils::gammaln(N - M + 1) - (Utils::gammaln(n - x + 1) + Utils::gammaln(t + 1));
            }

            if (x == 0.0 || M == x) 
            {
                lnparam1 = 0.0;
            }
            else if (x == 1.0 || (M - x) == 1.0)
            {
                lnparam1 = Utils::log(M);
            }
            else
            {
                lnparam1 = Utils::gammaln(M + 1) - (Utils::gammaln(x + 1) + Utils::gammaln(M - x + 1));
            }
            return Utils::exp(lnparam1 + lnparam2 - lnparam3);
        }
        return (param1 * param2) / param3;
    }
public:
    static T PMF(const T sample_s, const T number_sample, const T population_s, const T number_population) 
    {
        const T x = Utils::floor(sample_s);
        const T n = Utils::floor(number_sample);
        const T M = Utils::floor(population_s);
        const T N = Utils::floor(number_population);

        
        if (x == x && M <= N && n <= N && n >= 1 && M >= 0 && N >= 1 && !Utils::isinf(N))   
        {
            if (x < 0 || sample_s != x || (n - x) > (N - M) || x > M || x > n )     
            {
                return 0;
            }
            return ComputePMF(x, n, M, N);
        }

        
        if (Utils::isnan(number_sample))        return number_sample;
        if (Utils::isnan(population_s))         return population_s;
        if (Utils::isnan(number_population))    return number_population;
        if (Utils::isnan(sample_s))             return sample_s;
        
        
        if (N < 1 || Utils::isinf(N))
        {
            return TCError::NewError<T>(number_population,  TCFuncId_HYPGEOM_DIST, TCArgPosition_4, TCError_IllegalInput);
        }
        if (n > N || n < 1)
        {
            return TCError::NewError<T>(number_sample,  TCFuncId_HYPGEOM_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(M > N || M < 0);                              
        return TCError::NewError<T>(population_s,  TCFuncId_HYPGEOM_DIST, TCArgPosition_3, TCError_IllegalInput);
    }

    
    
    static T CDF(const T sample_s, const T number_sample, const T population_s, const T number_population) 
    {
        T x = Utils::floor(sample_s);
        const T n = Utils::floor(number_sample);
        const T M = Utils::floor(population_s);
        const T N = Utils::floor(number_population);

        
        if (x == x && M <= N && n <= N && n >= 1 && M >= 0 && N >= 1 && !Utils::isinf(N))             
        {
            if (x < 0 || (n - x) > (N - M))                          
            {
                return T(0);
            }
            if (x >= M || x >= n)
            {
                return T(1);
            }
            T result = ComputePMF(x, n, M, N);
            if (x != 0 && (n - x) != (N - M))
            {
                if (result < Utils::pow(2.0, -3000.0))
                {
                    if (x < n * M / N)
                    {
                        return T(0);
                    }
                    return T(1);
                }
                T mean = n * M / N;
                T lower = MAX(0, n - N + M);
                T upper = MIN(M,MIN(N, x));
                mean = MIN(ROUND(mean), upper);      
                T diff = 1;
                if (x >= mean)
                {
                    T cdf = ComputePMF(mean, n, M, N);
                    T cdfprev = cdf;
                    while (true)
                    {
                        if (mean - diff >= lower)
                        {
                            cdf += ComputePMF(mean - diff, n, M, N);
                        }

                        if (mean + diff <= upper)
                        {
                            cdf += ComputePMF(mean + diff, n, M, N);
                        }

                        if (Utils::abs(cdf - cdfprev) <= cdfprev * Utils::pow(10.0, -18.0) || cdf >= 1 || cdf != cdf)
                        {
                            return MIN(cdf, 1);
                        }
                        cdfprev = cdf;
                        diff++;
                    }
                }
                else
                {
                    T cdf = ComputePMF(upper, n, M, N);
                    T cdfprev = cdf;
                    while(true)
                    {
                        if (upper - diff >= lower)
                        {
                            cdf += ComputePMF(upper - diff, n, M, N);
                        }
                        if (Utils::abs(cdf - cdfprev) <= cdfprev * Utils::pow(10.0, -18.0) || cdf >= 1 || cdf != cdf)
                        {
                            return MIN(cdf, 1);
                        }
                        cdfprev = cdf;
                        diff++;
                    }
                }
            }
            return MIN(result, 1);
        }

        
        if (Utils::isnan(number_sample))        return number_sample;
        if (Utils::isnan(population_s))         return population_s;
        if (Utils::isnan(number_population))    return number_population;
        if (Utils::isnan(sample_s))             return sample_s;
        
        
        if (N < 1 || Utils::isinf(N))
        {
            return TCError::NewError<T>(number_population,  TCFuncId_HYPGEOM_DIST, TCArgPosition_4, TCError_IllegalInput);
        }
        if (n > N || n < 1)
        {
            return TCError::NewError<T>(number_sample,  TCFuncId_HYPGEOM_DIST, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(M > N || M < 0);                              
        return TCError::NewError<T>(population_s,  TCFuncId_HYPGEOM_DIST, TCArgPosition_3, TCError_IllegalInput);
    }

    static T InvCDF(const T probability, const T number_sample, const T population_s, const T number_population) 
    {
        const T n = Utils::floor(number_sample);
        const T M = Utils::floor(population_s);
        const T N = Utils::floor(number_population);

        
        if (probability >= 0 && probability <= 1 && M <= N && n <= N && n >= 1 && M >= 0 && N >= 1 && !Utils::isinf(N))  
        {
            T result = 0;
            TCInt64 x = MAX(0, n - N + M);
            if (N == 0) return 0;
            if (N == 1)
            {
                result = CDF(0, n, M, N);
                if (result >= probability) return 0;
                else return 1;
            }

            T lower = MAX(0, n - N + M);
            T upper = MIN(N, M);
            if (probability == 0) return lower;
            if (probability == 1) return upper;
            T mean = n * M / N;
            T std = Utils::sqrt(n * M * (N - n) * (N - M) / (N * N * (N - 1)));

            if (std == 0)
            {
                result = Utils::floor(mean);
                if (CDF(result, n, M, N) > probability) return result;
                result = Utils::ceil(mean);
                if (CDF(result, n, M, N) > probability) return result;
            }
            
            result = 0;
            while (true)
            {
                result += ComputePMF((T)x, n, M, N);
                if (result >= probability)
                {
                    break;
                }
                x++;
            }
            return x;
        }

        
        if (Utils::isnan(number_sample))        return number_sample;
        if (Utils::isnan(population_s))         return population_s;
        if (Utils::isnan(number_population))    return number_population;
        if (Utils::isnan(probability))          return probability;

        
        if (probability < 0 || probability > 1) 
        {
            return TCError::NewError<T>(probability,  TCFuncId_HYPGEOM_INV, TCArgPosition_1, TCError_IllegalInput);
        }
        if (N < 1 || Utils::isinf(N))
        {
            return TCError::NewError<T>(number_population,  TCFuncId_HYPGEOM_INV, TCArgPosition_4, TCError_IllegalInput);
        }
        if (n > N || n < 1)
        {
            return TCError::NewError<T>(number_sample,  TCFuncId_HYPGEOM_INV, TCArgPosition_2, TCError_IllegalInput);
        }
        assert(M > N || M < 0);                              
        return TCError::NewError<T>(population_s,  TCFuncId_HYPGEOM_INV, TCArgPosition_3, TCError_IllegalInput);
    }
};      
}       