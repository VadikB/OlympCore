//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "OlympCoreUtils.h"
#include "norm.h"

using namespace Utils;

namespace Dist
{
    template <typename T>
    class Chisq
    {
    public:
        static T CDF(const T x, const T deg_freedom)
        {
            if (x == x &&
                deg_freedom >= 1 && !Utils::isinf(deg_freedom))
            {
                if (x < 0)
                {
                    return 0;
                }

                const T kd2 = Utils::floor(deg_freedom) / 2;
                return Utils::gamma_regularized_p(kd2, x/2);
            }

            if (Utils::isnan(x))
            {
                return x;
            }
            if (Utils::isnan(deg_freedom))
            {
                return deg_freedom;
            }

            // At this point, should have deg_freedom < 1 or deg_freedom == inf.
            assert(deg_freedom < 1 || Utils::isinf(deg_freedom));
            return TCError::NewError<T>(deg_freedom, TCFuncId_CHISQ_DIST, TCArgPosition_2, TCError_IllegalInput);
        }

        static T PDF(const T x, const T deg_freedom)
        {
            if (x == x &&
                deg_freedom >= 1 && !Utils::isinf(deg_freedom))
            {
                if (x < 0)
                {
                    return 0;
                }
                else if (Utils::isinf(x)) // NaN is returned when x = +Inf without this check
                {
                    return 0;
                }

                const T dof = Utils::floor(deg_freedom);
                if (dof < ChisqHelpers::Traits<T>::NormDistThreshold())
                {
                    const T kd2 = dof/2;
                    const T pdfNumerator = Utils::pow(x, kd2 - 1) * Utils::exp(-x / 2);
                    const T pdfDenominator = Utils::pow(2, kd2) * Utils::gamma(kd2);

                    if (isinf(pdfNumerator) || isinf(pdfDenominator))
                    {
                        // We may be here because of an overflow in the formulation of the PDF numerator, denominator, or both
                        // above. Try doing the evaluation with an alternate formulation.
                        const T powTerms = (kd2-1) * Utils::log2(x) - kd2;
                        const T expTerm = -x/2 * Utils::log2(Utils::exp(static_cast<T>(1)));
                        const T gammaTerm = -Utils::gammaln(kd2) / Utils::log(static_cast<T>(2));
                        return Utils::pow(2, powTerms + expTerm + gammaTerm);
                    }
                    else
                    {
                        return pdfNumerator/pdfDenominator;
                    }
                }
                else // use normal distribution
                {
                    return Norm<T>::PDF(x, dof, Utils::sqrt(static_cast<T>(2))*Utils::sqrt(dof));
                }
            }

            if (Utils::isnan(x))
            {
                return x;
            }
            if (Utils::isnan(deg_freedom))
            {
                return deg_freedom;
            }

            // At this point, should have deg_freedom < 1 or deg_freedom == inf.
            assert(deg_freedom < 1 || Utils::isinf(deg_freedom));
            return TCError::NewError<T>(deg_freedom, TCFuncId_CHISQ_DIST, TCArgPosition_2, TCError_IllegalInput);
        }

        static T InvCDF(const T probability, const T deg_freedom)
        {
            if (probability >= 0 && probability <= 1 &&
                deg_freedom >= 1 && !Utils::isinf(deg_freedom))
            {
                const T kd2 = Utils::floor(deg_freedom) / 2;
                return 2 * Utils::gamma_regularized_p_inv(kd2, probability);
            }

            if (Utils::isnan(probability))
            {
                return probability;
            }
            if (Utils::isnan(deg_freedom))
            {
                return deg_freedom;
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_CHISQ_INV, TCArgPosition_1, TCError_IllegalInput);
            }

            // At this point, should have deg_freedom < 1 or deg_freedom == inf.
            assert(deg_freedom < 1 || Utils::isinf(deg_freedom));
            return TCError::NewError<T>(deg_freedom, TCFuncId_CHISQ_INV, TCArgPosition_2, TCError_IllegalInput);
        }

        static T RT(const T x, const T deg_freedom)
        {
            if (x == x &&
                deg_freedom >= 1 && !Utils::isinf(deg_freedom))
            {
                if (x < 0)
                {
                    return 1;
                }

                const T kd2 = Utils::floor(deg_freedom) / 2;
                return Utils::gamma_regularized_q(kd2, x / 2);
            }

            if (Utils::isnan(x))
            {
                return x;
            }
            if (Utils::isnan(deg_freedom))
            {
                return deg_freedom;
            }

            // At this point, should have deg_freedom < 1 or deg_freedom == inf.
            assert(deg_freedom < 1 || Utils::isinf(deg_freedom));
            return TCError::NewError<T>(deg_freedom, TCFuncId_CHISQ_DIST_RT, TCArgPosition_2, TCError_IllegalInput);
        }

        static T InvRT(const T probability, const T deg_freedom)
        {
            if (probability >= 0 && probability <= 1 &&
                deg_freedom >= 1 && !Utils::isinf(deg_freedom))
            {
                const T kd2 = Utils::floor(deg_freedom) / 2;
                return 2 * Utils::gamma_regularized_q_inv(kd2, probability);
            }

            if (Utils::isnan(probability))
            {
                return probability;
            }
            if (Utils::isnan(deg_freedom))
            {
                return deg_freedom;
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_CHISQ_INV_RT, TCArgPosition_1, TCError_IllegalInput);
            }

            // At this point, should have deg_freedom < 1 or deg_freedom == inf.
            assert(deg_freedom < 1 || Utils::isinf(deg_freedom));
            return TCError::NewError<T>(deg_freedom, TCFuncId_CHISQ_INV_RT, TCArgPosition_2, TCError_IllegalInput);
        }

    private:
        Chisq();
    };

    namespace ChisqHelpers
    {
        template <typename T>
        struct Traits
        {
            // If degrees of freedom is larger than NormDistThreshold, the PDF
            // calls the normal distribution PDF instead of the using the
            // chi-square PDF formulation.
            static T NormDistThreshold()
            {
                return static_cast<T>(0);
            }
        };

        template <>
        struct Traits<TCDouble>
        {
            static TCDouble NormDistThreshold()
            {
                return static_cast<TCDouble>(1e250);
            }
        };

        template <>
        struct Traits<TCFloat>
        {
            static TCFloat NormDistThreshold()
            {
                return static_cast<TCFloat>(1e30);
            }
        };
    }
}
