



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Cauchy
    {
    public:
        static T PDF(const T x, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(x)) return x;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_CAUCHY_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_CAUCHY_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            T z = (x - location) / scale;
            
            
            T ret = (T)1.0 / (T)Utils::pi() / (scale * ((T)1.0 + z * z));

            if (ret == 0)
            {
                ret = scale / (T)Utils::pi() / pow(x - location, 2);
            }
            return ret;
        }

        static T CDF(const T x, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(x)) return x;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_CAUCHY_DIST, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_CAUCHY_DIST, TCArgPosition_3, TCError_IllegalInput);
            }

            T z = (x - location) / scale;
            if (z < 0)
            {
                
                return (T)(-1.0) / (T)Utils::pi() * Utils::atan(1 / z);
            }
            else
            {
                
                return (T)1.0 / (T)Utils::pi() * Utils::atan(z) + (T).5;
            }        
        }

        static T InvCDF(const T probability, const T location, const T scale)
        {
            if(Utils::isnan(location)) return location;
            if(Utils::isnan(scale)) return scale;
            if(Utils::isnan(probability)) return probability;

            if (Utils::isinf(location))
            {
                return TCError::NewError<T>(location, TCFuncId_CAUCHY_INV, TCArgPosition_2, TCError_IllegalInput);
            }

            if (scale <= 0 || Utils::isinf(scale))
            {
                return TCError::NewError<T>(scale, TCFuncId_CAUCHY_INV, TCArgPosition_3, TCError_IllegalInput);
            }

            if (probability > 0 && probability < 1)
            {
                if (probability < .5)
                {
                    return location - scale * Utils::cot((T)Utils::pi() * probability);
                }
                else if (probability == .5)
                {
                    return location;
                }
                else
                {
                    return location + scale * Utils::cot((T)Utils::pi() * (1 - probability));
                }
            }
            else if(probability == (T)0.0)
            {
                return -Utils::infinity<T>();
            }
            else if (probability == (T)1.0)
            {
                return Utils::infinity<T>();
            }
            else
            {
                return TCError::NewError<T>(probability, TCFuncId_CAUCHY_INV, TCArgPosition_1, TCError_IllegalInput);
            }    
        }
    };
}