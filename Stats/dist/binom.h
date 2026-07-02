



#pragma once

#include "OlympCoreUtils.h"

using namespace Utils;

namespace Dist
{
    template <typename T>
    class Binom
    {
    public:
        static T CDF(const T number_s, const T trials, const T probability_s)
        {
            if (number_s == number_s &&
                trials >= 0 && !Utils::isinf(trials) &&
                probability_s >= 0 && probability_s <= 1)
            {
                if (number_s < 0)
                {
                    return 0;
                }

                const T ntrials = Utils::floor(trials);
                const T n_s = Utils::floor(number_s);

                if (n_s >= ntrials)
                {
                    return 1;
                }

                if (probability_s > 0.000001)
                {
                    return betainc(1-probability_s, ntrials-n_s, 1+n_s);
                }
                else
                {
                    return 1 - betainc(probability_s, 1+n_s, ntrials-n_s);
                }
            }

            if (Utils::isnan(number_s))
            {
                return number_s;
            }
            if (Utils::isnan(trials))
            {
                return trials;
            }
            if (Utils::isnan(probability_s))
            {
                return probability_s;
            }

            if (trials < 0 || Utils::isinf(trials))
            {
                return TCError::NewError<T>(trials, TCFuncId_BINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            
            assert(probability_s < 0 || probability_s > 1);
            return TCError::NewError<T>(probability_s, TCFuncId_BINOM_DIST, TCArgPosition_3, TCError_IllegalInput);
        }

        static T PMF(const T number_s, const T trials, const T probability_s)
        {
            if (number_s == number_s &&
                trials >= 0 && !Utils::isinf(trials) &&
                probability_s >= 0 && probability_s <= 1)
            {
                if (number_s < 0 || number_s != Utils::floor(number_s))
                {
                    return 0;
                }
                if (Utils::isinf(number_s))
                {
                    return 0;
                }

                const T ntrials = Utils::floor(trials);

                if (number_s > ntrials)
                {
                    return 0;
                }

                if (probability_s > 0 && probability_s < 1)
                {
                    const T lnCombin = GammaFact(ntrials) - (GammaFact(number_s) + GammaFact(ntrials-number_s));
                    const T lnPowTerm1 = number_s * Utils::log(probability_s);
                    const T lnPowTerm2 = (ntrials-number_s) * Utils::log1p(-probability_s);
                    const T lnPmf = lnCombin + lnPowTerm1 + lnPowTerm2;

                    if (lnPmf == lnPmf)
                    {
                        return Utils::exp(lnPmf);
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if (probability_s == 0)
                {
                    return number_s == 0;
                }
                else 
                {
                    return ntrials == number_s;
                }
            }

            if (Utils::isnan(number_s))
            {
                return number_s;
            }
            if (Utils::isnan(trials))
            {
                return trials;
            }
            if (Utils::isnan(probability_s))
            {
                return probability_s;
            }

            if (trials < 0 || Utils::isinf(trials))
            {
                return TCError::NewError<T>(trials, TCFuncId_BINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            
            assert(probability_s < 0 || probability_s > 1);
            return TCError::NewError<T>(probability_s, TCFuncId_BINOM_DIST, TCArgPosition_3, TCError_IllegalInput);
        }

        static T InvCDF(const T probability, const T trials, const T probability_s)
        {
            if (probability >= 0 && probability <= 1 &&
                trials >= 0 && !Utils::isinf(trials) &&
                probability_s >= 0 && probability_s <= 1)
            {
                const T ntrials = Utils::floor(trials);

                if (probability == 1 && probability_s > 0)
                {
                    return ntrials;
                }
                else
                {
                    return DiscreteBisection(0, ntrials, probability, trials, probability_s);
                }
            }

            if (Utils::isnan(probability))
            {
                return probability;
            }
            if (Utils::isnan(trials))
            {
                return trials;
            }
            if (Utils::isnan(probability_s))
            {
                return probability_s;
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_BINOM_INV, TCArgPosition_3, TCError_IllegalInput);
            }
            if (trials < 0 || Utils::isinf(trials))
            {
                return TCError::NewError<T>(trials, TCFuncId_BINOM_INV, TCArgPosition_1, TCError_IllegalInput);
            }

            
            assert(probability_s < 0 || probability_s > 1);
            return TCError::NewError<T>(probability_s, TCFuncId_BINOM_INV, TCArgPosition_2, TCError_IllegalInput);
        }

    private:
        Binom();

        
        static T GammaFact(const T n)
        {
            return Utils::gammaln(n+1);
        }

        
        
        
        
        
        static T NextLower(const T in)
        {
            const T inEps = eps(in);
            if (inEps < static_cast<T>(1))
            {
                return in-static_cast<T>(1);
            }
            return in-inEps;
        }

        static T DiscreteBisection(T lower, T upper, const T probability, const T trials, const T probability_s)
        {
            assert(lower <= upper);
            assert(lower == Utils::floor(lower));
            assert(upper == Utils::floor(upper));

            T lowerCDF = CDF(lower, trials, probability_s);
            T nextLower = NextLower(upper);
            while (lower < nextLower)
            {
                const T midpoint = Utils::floor(lower + (upper - lower) / 2);
                const T cdf = CDF(midpoint, trials, probability_s);
                if (cdf < probability)
                {
                    lower = midpoint;
                    lowerCDF = cdf;
                }
                else
                {
                    upper = midpoint;
                    nextLower = NextLower(upper);
                }
            }

            if (lowerCDF >= probability)
            {
                return lower;
            }
            else
            {
                return upper;
            }
        }
    };
}