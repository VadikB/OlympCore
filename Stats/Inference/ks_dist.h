//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <TCTypes.h>
#include <OlympCoreUtils.h>
#include <TCConsts.h>

#include <vector>

namespace Dist
{
    template <typename T>
    class KSTwoSided;

    template <typename T>
    class KSOneSided
    {
    public:
        static T CDF(T x, TCSize_t n)
        {
            if (n < 3000)
            {
                return Birnhaum_Tingey(x, n);
            }
            return Smirnov(x, n);
        }
        friend class KSTwoSided<T>;

    private:
        template<typename T>
        static T GetEPS()
        {
            static_assert("KsOneSided::GetEPS is specialized by wrong type.")
        }
        template<>
        static TCFloat GetEPS<TCFloat>()
        {
            return 1e-6;
        }
        template<>
        static TCDouble GetEPS<TCDouble>()
        {
            return 1e-16;
        }

        static T Birnhaum_Tingey(T x, TCSize_t n)
        {
            const T EPS = GetEPS<T>();
            TCSize_t j_bound = (TCSize_t)(n * (1. - x));
            T sum = 0.;
            T term = 0.;
            TCSize_t j = 1;
            T logC = Utils::log((T)n);

            if (j_bound > 0)
            {
                do
                {
                    T t = (T)j / n + x;
                    term = logC + (j - 1) * Utils::log(t) +
                        (n - j) * Utils::log1p(-t);
                    term = Utils::exp(term);
                    sum += term;
                    logC += Utils::log((T)(n - j) / (j + 1));
                } while (j++ < j_bound && term >= EPS * sum);
                sum *= x;
            }

            sum += Utils::exp(n * Utils::log1p(-x));

            return sum;
        }

        static T Smirnov(T x, TCSize_t n)
        {
            const T z2 = n * x * x;
            
            return Utils::exp(-2 * z2);
        }
    }; // class KSOneSided

    template <typename T>
    class KSTwoSided
    {
    public:
        static T CDF(T x, TCSize_t n)
        {

            if (n * x * x >= 18.)
            {
                return 0.;
            }

            if (n * x * x >= 4 && n <= 140 && x < 1 - 1. / n)
            {
                return 2. * KSOneSided<T>::Birnhaum_Tingey(x, n);
            }

            if ((x <= 1. / n) || (x >= (1 - 1. / n)))
            {
                return Ruben_Gambino(x, n);
            }

            if (n > 140)
            {
                return Pelz_Good(x, n);
            }

            return Durbin(x, n);
        }

    private:
        template<typename T>
        static T GetEPS()
        {
            static_assert("KSTwoSided::GetEPS is specialized by wrong type.");
        }
        template<>
        static TCFloat GetEPS<TCFloat>()
        {
            return 1e-6;
        }
        template<>
        static TCDouble GetEPS<TCDouble>()
        {
            return 1e-16;
        }

        static T Ruben_Gambino(T x, TCSize_t n)
        {
            if (x <= 0.5 / n)
            {
                return T(1.);
            }
            else if (x <= 1. / n)
            {
                T t = Utils::pow(2 * x - 1. / n, n);
                return 1. - Utils::exp(Utils::gammaln_precise((T)(n + 1))) * t;
            }
            else if ((1 - 1. / n <= x) && (x < 1.))
            {
                return 2 * Utils::pow(1 - x, n);
            }

            // We can't come here
            return TCError::NewErrorImpl<T>::run(tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown));
        }

        static void mmult(const std::vector<T>& A, const std::vector<T>& B,
            std::vector<T>& C, TCSize_t m, std::vector<T>& tmp)
        {
            for (TCSize_t i = 0; i < m; i++)
            {
                for (TCSize_t j = 0; j < m; j++)
                {
                    tmp[i * m + j] = 0.;
                    for(TCSize_t k = 0; k < m; k++)
                    {
                        tmp[i * m + j] += A[i * m + k] * B[k * m + j];
                    }
                }
            }

            for (TCSize_t i = 0; i < m; i++)
            {
                for (TCSize_t j = 0; j < m; j++)
                {
                    C[i * m + j] = tmp[i * m + j];
                }
            }
        }

        template<typename T>
        static T Norm()
        {
            static_assert("KSTwoSided::Norm is specialized by wrong type.");
        }
        template<>
        static TCFloat Norm<TCFloat>()
        {
            return 1e25;
        }
        template<>
        static TCDouble Norm<TCDouble>()
        {
            return 1e140;
        }

        template<typename T>
        static T INorm()
        {
            static_assert("KSTwoSided::INorm is specialized by wrong type.");
        }
        template<>
        static TCFloat INorm<TCFloat>()
        {
            return 1e-25;
        }
        template<>
        static TCDouble INorm<TCDouble>()
        {
            return 1e-140;
        }

        template<typename T>
        static TCSize_t LogNorm()
        {
            static_assert("KSTwoSided::LogNorm is specialized by wrong type.");
        }
        template<>
        static TCSize_t LogNorm<TCFloat>()
        {
            return 25;
        }
        template<>
        static TCSize_t LogNorm<TCDouble>()
        {
            return 140;
        }

        static void renormalize(std::vector<T>& A, TCSize_t m, TCSize_t* eA)
        {
           for (TCSize_t i = 0; i < m * m; i++)
           {
              A[i] *= INorm<T>();
           }
           *eA += LogNorm<T>();
        }

        static void mpow(const std::vector<T>& A, std::vector<T>& V,
            TCSize_t* eV, TCSize_t m, TCSize_t n)
        {
            std::vector<T> tmp(m * m);
            std::vector<T> pow(m * m);
            TCSize_t epow = 0;

            *eV = 0;

            for(TCSize_t i = 0; i < m; i++)
            {
                for(TCSize_t j = 0; j < m; j++)
                {
                    V[i * m + j] = 0.;
                    pow[i * m + j] = A[i * m + j];
                }
                V[i * m + i] = 1.;
            }

            while (n)
            {
                if(n % 2)
                {
                    mmult(V, pow, V, m, tmp);
                    *eV += epow;
                    if (V[m / 2 * m + m / 2] > Norm<T>())
                    {
                        renormalize(V, m, eV);
                    }
                }

                mmult(pow, pow, pow, m, tmp);
                epow *= 2;
                if (tmp[m / 2 * m + m / 2] > Norm<T>())
                {
                    renormalize(pow, m, &epow);
                }

                n /= 2;
            }
        }

        static T Durbin(T x, TCSize_t n)
        {
            const T z2 = x * x * n;

            if (z2 > 7.24 || (z2 > 3.76 && n > 99))
            {
                T tmp = -(2.000071 + .331 / Utils::sqrt((T)n) + 1.409 / n) * z2;
                return 2 * Utils::exp(tmp);
            }

            TCSize_t k = (TCSize_t)(n * x) + 1;
            TCSize_t m = 2 * k - 1;
            T h = k - n * x;
            T s = 0.;
            TCSize_t eQ = 0;
                
            std::vector<T> H(m * m);
            std::vector<T> Q(m * m);
                
            // Form matrix H
            for (TCSSize_t i = 0; i < (TCSSize_t)m; i++)
            {
                for (TCSSize_t j = 0; j < (TCSSize_t)m; j++)
                {
                    if (i - j + 1 < 0)
                    {
                        H[i * m + j] = 0.;
                    }
                    else
                    {
                        H[i * m + j] = 1.;
                    }
                }
            }

            for (TCSize_t i = 0; i < m; i++)
            {
                H[i * m] -= Utils::pow(h, (T)(i + 1));
                H[(m - 1) * m + i] -= Utils::pow(h, (T)(m - i));
            }
               
            H[(m - 1) * m] += (2 * h - 1 > 0 ? Utils::pow(2 * h - 1, (T)m) : 0.);

            for (TCSSize_t i = 0; i < (TCSSize_t)m; i++)
            {
                for (TCSSize_t j = 0; j < (TCSSize_t)m; j++)
                {
                    if (i - j + 1 > 0)
                    {
                        for (T g = 1.; g <= i - j + 1; g += 1.)
                        {
                            H[i * m + j] /= g;
                        }
                    }
                }
            }

            mpow (H, Q, &eQ, m, n);
            s = Q[(k - 1) * m + k - 1];

            for (TCSize_t i = 1; i <= n; i++)
            {
                s *= (T)i / n;

                if (s < INorm<T>())
                {
                    s *= Norm<T>();
                    eQ -= LogNorm<T>();
                }
            }

            s *= Utils::pow(10., eQ);
            return 1. - s;
        }

        static T Pelz_Good(T x, TCSize_t n)
        {
            const int MAX_IT = 40;
            const T EPS = GetEPS<T>();
            const T SQRT2N = Utils::sqrt((T)n);
            const T SQRT2PI_2 = Utils::sqrt(M_PI_2);
            const T z = SQRT2N * x;
            const T z2 = z * z;
            const T z3 = z2 * z;
            const T z4 = z2 * z2;
            const T z6 = z4 * z2;
            const T z7 = z6 * z;
            const T z8 = z4 * z4;
            const T z10 = z8 * z2;
            const T PI2 = M_PI * M_PI;
            const T PI4 = PI2 * PI2;
            const T PI6 = PI4 * PI2;
            const T p = PI2 / (2. * z * z);
            T sum = 0.;
            T item = 0.;
            T K = 0.;
            int k = 0;

            k = 1;
            do
            {
                T t = k - 1./2;
                T t2 = t * t;
                item =	Utils::exp(-t2*p);
                sum += item;
            } while (k++ <= MAX_IT && item > EPS * sum);
            sum *= M_SQRT2PI/z;

            k = 0;
            K = 0.;
            do
            {
                T t = k + 1. / 2;
                T t2 = t * t;
                item = (PI2 * t2 - z2) * Utils::exp(-t2 * p);
                K += item;
            } while (k++ < MAX_IT && fabs(item) > EPS * fabs(K));
            sum += K * SQRT2PI_2 / (3. * z4 * SQRT2N);

            k = 0;
            K = 0.;
            do
            {
                T t = k + 1. / 2;
                T t2 = t * t;
                T t4 = t2 * t2;
                item = 6 * z6 + 2 * z4 + PI2 * (2 * z4 - 5 * z2) * t2 +
                        PI4 * (1 - 2 * z2) * t4;
                item *= Utils::exp(-t2 * p);
                K += item;
            } while (k++ < MAX_IT && fabs(item) > EPS * fabs(K));
            sum += K * SQRT2PI_2 / (36. * n * z7);

            k = 1;
            K = 0.;
            do
            {
                T t2 = k * k;
                item =	PI2 * t2 * Utils::exp(-t2 * p);
                K += item;
            } while (k++ <= MAX_IT && item > EPS * K);
            sum -= K * SQRT2PI_2 / (18. * n * z3);

            k = 0;
            K = 0.;
            do
            {
                T t = k + 1. / 2;
                T t2 = t * t;
                T t4 = t2 * t2;
                T t6 = t4 * t2;
                item = -30 * z6 - 90 * z8 + PI2 * (135 * z4 - 96 * z6) * t2 +
                        PI4 * (212 * z4 - 60 * z2) * t4 + PI6 * t6 * (5 - 30 * z2);
                item *= Utils::exp(-t2 * p);
                K += item;
            } while (k++ < MAX_IT && item > EPS * K);
            sum += K * SQRT2PI_2 / (3240. * SQRT2N * n * z10);

            k = 1;
            K = 0.;
            do
            {
                T t2 = k * k; T t4 = t2 * t2;
                item =	(3 * PI2 * t2 * z2 - PI4 * t4) * Utils::exp(-t2 * p);
                K += item;
            } while (k++ <= MAX_IT && item > EPS * K);
            sum += K * SQRT2PI_2 / (108. * SQRT2N * n * z6);

            return 1. - sum;
        }
    }; // class KSTwoSided
} // namespace Dist