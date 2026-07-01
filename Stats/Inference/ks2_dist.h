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
    class KS2Samples
    {
    public:
        static T CDF(T x, TCSize_t n, TCSize_t m, TCHypothesis test)
        {
            if (n < 1000 && m < 1000)
            {
                return Exact(x, n, m, test);
            }

            if (test == TwoTail)
            {
                return TwoSidedApproximation(x, n, m);
            }

            return OneSidedApproximation(x, n, m);
        }

    private:
        template<typename T>
        static T GetEPS()
        {
            static_assert("KS2Samples::GetEPS is specialized by wrong type.")
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

        static T OneSidedApproximation(T x, TCSize_t n, TCSize_t m)
        {
            const T z2 = n * m * x * x / (n + m);
            
            return Utils::exp(- 2 * z2);
        }

        static T TwoSidedApproximation(T x, TCSize_t n, TCSize_t m)
        {
            if (x == 0)
                return 1.;

            const T z2 = n * m * x * x / (n + m);
            const T z = Utils::sqrt(z2);
            const T p = M_PI * M_PI / (2. * z2);
            const int MAX_IT = 100;
            const T EPS = GetEPS<T>();
            T sum = 0.;
            T item = 0.;
            int k = 1;

            do
            {
                T t = k - 1./2;
                T t2 = t * t;
                item =	Utils::exp(- t2 * p);
                sum += item;
            } while (k++ <= MAX_IT && item > EPS * sum);
            sum *= M_SQRT2PI / z;

            return 1. - sum;
        }

        static T Exact(T x, TCSize_t n, TCSize_t m, TCHypothesis test)
        {
            T bound = x * n * m - 1. / 2;

            std::vector<T> B(m + 1);

            if (test != OneTailGT)
            {
                for(TCSize_t j = 0; j <= m; j++)
                {
                    B[j] = (((T)j * n > bound) ? 0. : 1.);
                }
            }
            else
            {
                for(TCSize_t j = 0; j <= m; j++)
                {
                    B[j] = 1.;
                }
            }

            for (TCSize_t i = 1; i <= n; i++)
            {
                for (TCSize_t j = 0; j <= m; j++)
                {
                    T dist = 0;
                    switch (test)
                    {
                    case TwoTail:
                        dist = std::abs((T)i * m - (T)j * n);
                        break;
                    case OneTailGT:
                        dist = (T)i * m - (T)j * n;
                        break;
                    case OneTailLT:
                        dist = (T)j * n - (T)i * m;
                        break;
                    }

                    if (dist > bound)
                    {
                        B[j] = 0;
                    }
                    else
                    {
                        if (j > 0)
                        {
                            B[j] = B[j-1] * (T)j / ((T)(i + j)) +
                                B[j] * (T)i / ((T)(i + j));
                        }
                    }
                }
            }

            return 1. - B[m];
        }
    }; // class KS2Samples
} // namespace Dist