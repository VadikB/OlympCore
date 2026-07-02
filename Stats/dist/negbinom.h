



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Negbinom
    {
    public:
        static T PMF(const T number_f, const T number_s, const T probability_s)
        {
            T num_f, num_s;

            num_f = Utils::floor(number_f);
            num_s = Utils::floor(number_s);

            if((probability_s > 0) &&
                (probability_s <= 1) &&
                (number_f >= 0) &&
                (number_s >= 1))
            {
                if(Utils::isinf(number_s) || (number_s < 1)) return TCError::NewError<T>(number_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(number_f)) return 0;

                if(number_f!=num_f) return 0;

                T ret = Utils::combin(num_f+num_s-1, num_f) *
                    Utils::pow(T(1.0)-probability_s, num_f) *
                    Utils::pow(probability_s, num_s);

                if(Utils::isnan(ret) || Utils::isinf(ret))
                {
                    ret = Utils::gammaln(num_f+num_s) - Utils::gammaln(num_f+1) - Utils::gammaln(num_s)
                        + num_f * Utils::log1p(-probability_s) + num_s * Utils::log(probability_s);

                    return Utils::exp(ret);
                }

                return ret;
            }

            if(Utils::isnan(number_s)) return number_s;
            if(Utils::isnan(number_f)) return number_f;
            if(Utils::isnan(probability_s)) return probability_s;

            if(Utils::isinf(number_s) || (number_s < 1)) return TCError::NewError<T>(number_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
            if(Utils::isinf(probability_s) || (probability_s <= 0) || (probability_s > 1))
            {
                return TCError::NewError<T>(probability_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            return 0;
        }

        static T CDF(const T number_f, const T number_s, const T probability_s)
        {
            if((probability_s > 0) &&
                (probability_s <= 1) &&
                (number_f >= 0) &&
                (number_s >= 1))
            {
                if(Utils::isinf(number_s) || (number_s < 1)) return TCError::NewError<T>(number_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
                if(Utils::isinf(number_f)) return 1;

                return Utils::betainc(probability_s, T(Utils::floor(number_s)), T(Utils::floor(number_f+1)));
            }

            if(Utils::isnan(number_s)) return number_s;
            if(Utils::isnan(number_f)) return number_f;
            if(Utils::isnan(probability_s)) return probability_s;

            if(Utils::isinf(number_s) || (number_s < 1)) return TCError::NewError<T>(number_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_2, TCError_IllegalInput);
            if(Utils::isinf(probability_s) || (probability_s <= 0) || (probability_s > 1))
            {
                return TCError::NewError<T>(probability_s, TCFuncId_NEGBINOM_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            return 0;
        }
    };
}