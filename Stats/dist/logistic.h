



#pragma once

#include "OlympCoreUtils.h"

using namespace Utils;

namespace Dist
{
    template <typename T, class TestClass = T>
    class Logistic
    {
        friend TestClass;

    public:
        static T CDF(const T x, const T mean, const T scale)
        {
            if (x == x &&
                !Utils::isinf(mean) && mean == mean &&
                scale > 0 && !Utils::isinf(scale))
            {
                const T expTerm = Utils::exp(-(x - mean)/scale);
                return 1 / (1 + expTerm);
            }

            if (Utils::isnan(x))
            {
                return x;
            }
            if (Utils::isnan(mean))
            {
                return mean;
            }
            if (Utils::isnan(scale))
            {
                return scale;
            }

            if (Utils::isinf(mean))
            {
                return TCError::NewError<T>(mean, TCFuncId_LOGISTIC_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale, TCFuncId_LOGISTIC_DIST, TCArgPosition_3, TCError_IllegalInput);
        }

        static T PDF(const T x, const T mean, const T scale)
        {
            if (x == x &&
                !Utils::isinf(mean) && mean == mean &&
                scale > 0 && !Utils::isinf(scale))
            {
                const T expComponent = PDFExpComponent(x, mean, scale);
                return expComponent / (scale*Utils::pow((1 + expComponent), 2));
            }

            if (Utils::isnan(x))
            {
                return x;
            }
            if (Utils::isnan(mean))
            {
                return mean;
            }
            if (Utils::isnan(scale))
            {
                return scale;
            }

            if (Utils::isinf(mean))
            {
                return TCError::NewError<T>(mean, TCFuncId_LOGISTIC_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale, TCFuncId_LOGISTIC_DIST, TCArgPosition_3, TCError_IllegalInput);
        }

        static T InvCDF(const T probability, const T mean, const T scale)
        {
            if (probability >= 0 && probability <= 1 &&
                !Utils::isinf(mean) && mean == mean &&
                scale > 0 && !Utils::isinf(scale))
            {
                return mean + scale * Utils::log(probability/(1-probability));
            }

            if (Utils::isnan(probability))
            {
                return probability;
            }
            if (Utils::isnan(mean))
            {
                return mean;
            }
            if (Utils::isnan(scale))
            {
                return scale;
            }

            if (probability < 0 || probability > 1)
            {
                return TCError::NewError<T>(probability, TCFuncId_LOGISTIC_INV, TCArgPosition_1, TCError_IllegalInput);
            }
            if (Utils::isinf(mean))
            {
                return TCError::NewError<T>(mean, TCFuncId_LOGISTIC_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            
            assert(scale <= 0 || Utils::isinf(scale));
            return TCError::NewError<T>(scale, TCFuncId_LOGISTIC_INV, TCArgPosition_3, TCError_IllegalInput);
        }

    private:
        
        
        
        
        
        static T PDFExpComponent(const T x, const T mean, const T scale)
        {
            assert(scale > 0);

            if (x < mean)
            {
                return Utils::exp((x - mean)/scale);
            }
            else
            {
                return Utils::exp(-(x - mean)/scale);
            }
        }

        Logistic();
    };
}