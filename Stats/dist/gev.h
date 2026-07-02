



#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{
    template <typename T>
    class Gev
    {
    public:
        static T PDF(const T x, const T location, const T scale, const T shape)
        {
            T result = 0;
            result = p_dist_gev(stats_enum_dist_pdf, x, location, scale, shape);
            return result;
        }

        static T CDF(const T x, const T location, const T scale, const T shape)
        {
            T result = 0;
            result = p_dist_gev(stats_enum_dist_cdf, x, location, scale, shape);
            return result;
        }

        static T InvCDF(const T probability, const T location, const T scale, const T shape)
        {
            T result = 0;
            T t = 0;

            
            BAIL_ON_NAN(probability, result);
            BAIL_ON_NAN(location, result);
            BAIL_ON_NAN(scale, result);
            BAIL_ON_NAN(shape, result);

            
            if ( Utils::isinf(location) )
            {
                result = TCError::NewError<T>(location,  TCFuncId_GEV_INV, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if ( Utils::isinf(scale) )
            {
                result = TCError::NewError<T>(scale,  TCFuncId_GEV_INV, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            if ( Utils::isinf(shape) )
            {
                result = TCError::NewError<T>(shape,  TCFuncId_GEV_INV, TCArgPosition_4, TCError_IllegalInput);
                BAIL();
            }

            
            if (scale <= 0)
            {
                result = TCError::NewError<T>(scale,  TCFuncId_GEV_INV, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            
            if (probability < 0 || probability > 1)
            {
                result = Utils::quiet_NaN<T>();
                BAIL();
            }

            t = -Utils::log(probability);

            if (0 == shape)
            {
                result =  - scale * Utils::log(t) + location;
            }
            else
            {
                result = scale/shape * Utils::expm1 (- shape *Utils::log(t) ) + location;

                if (Utils::isinf(result))
                {
                    result = scale * Utils::expm1 (- shape *Utils::log(t) ) /shape + location;
                    if (Utils::isinf(result))
                    {
                        result = Utils::exp (Utils::log(scale) - shape *Utils::log(t) ) /shape + location;
                    }
                }
            }

        exitlabel:
            return result;
        }

    protected:
        static T  p_dist_gev(const STATS_DIST_TYPE type, const T x, const T location, const T scale, const T shape)
        {
            T result = 0;
            T logresult = 0;
            T normalized_x = 0;
            T lnt = 0;
            T t = 0;
            T boundary = 0;
            T product = 0;
            T temp = 0;

            
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(location, result);
            BAIL_ON_NAN(scale, result);
            BAIL_ON_NAN(shape, result);

            
            if ( Utils::isinf(location) )
            {
                result = TCError::NewError<T>(location,  TCFuncId_GEV_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if ( Utils::isinf(scale) )
            {
                result = TCError::NewError<T>(scale,  TCFuncId_GEV_DIST, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            if ( Utils::isinf(shape) )
            {
                result = TCError::NewError<T>(shape,  TCFuncId_GEV_DIST, TCArgPosition_4, TCError_IllegalInput);
                BAIL();
            }

            
            if (scale <= 0)
            {
                result = TCError::NewError<T>(scale,  TCFuncId_GEV_DIST, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            
            normalized_x = (x-location)/scale;
            if (0 == shape)
            {
                t = Utils::exp( - normalized_x );
            }
            else
            {
                assert (0 != shape);
                t = Utils::pow (1 + shape * normalized_x, - 1/shape);
            }

            
            boundary = location - scale/shape;
            

            if (stats_enum_dist_pdf == type)  
            {
                if (shape < 0)
                {
                    if (x > boundary)
                    {
                        result = 0;
                    }
                    else if (Utils::isinf(x)) 
                    {
                        result = 0;
                    }
                    else
                    {
                        if (Utils::isinf(normalized_x) == false )
                        {                        
                            product = shape * normalized_x;
                            if (Utils::isinf(product))
                            {
                                assert (Utils::isinf(shape) == false);
                                assert (Utils::isinf(normalized_x) == false);

                                if (Utils::isinf(x-location) && x != 0 && location != 0)
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                                }
                                else
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                                }
                            }
                            else
                            {
                                lnt = - 1/shape * Utils::log1p (product);
                            }
                            
                            t = Utils::exp(lnt);

                            logresult = - Utils::log(scale) + (shape + 1)* lnt -t;
                            result = Utils::exp(logresult);
                            
                        }
                        else
                        {
                            
                            product = (shape * x - shape * location)/scale;

                            if (Utils::isinf(shape * x) && Utils::isinf(shape * location) && Utils::isnan(product) )
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                            }
                            else if (Utils::isinf(product))
                            {
                                if (Utils::isinf(x-location) && x != 0 && location != 0)
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                                }
                                else
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                                }
                            }
                            else
                            {
                                lnt = - 1/shape * Utils::log1p (product);
                            }

                            
                            if (lnt > 0 && Utils::isinf(lnt))
                            {
                                result = 0.0;
                            }
                            else
                            {
                                t = Utils::exp(lnt);

                                logresult = - Utils::log(scale) + (shape + 1)* lnt -t;
                                result = Utils::exp(logresult);
                            }
                        }
                    }
                }
                
                else if ( 0 == shape )
                {
                    if (Utils::isinf(x)) 
                    {
                        result = 0;
                    }
                    else
                    {
                        
                        if (t > 0 && Utils::isinf(t) )
                        {
                            result = 0.00;
                        }
                        else
                        {
                            result = 1/scale * Utils::pow(t, shape + 1) * Utils::exp(-t);
                        }
                    }
                }
                
                else
                {
                    assert (shape > 0);
                    if (x < boundary)
                    {
                        result = 0;
                    }
                    else
                    {
                        if (Utils::isinf(normalized_x) == false )
                        {                        
                            product = shape * normalized_x;
                            if (Utils::isinf(product))
                            {
                                assert (Utils::isinf(shape) == false);
                                assert (Utils::isinf(normalized_x) == false);

                                if (Utils::isinf(x-location) && x != 0 && location != 0)
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                                }
                                else
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                                }
                            }
                            else
                            {
                                lnt = - 1/shape * Utils::log1p (product);
                            }
                            
                            t = Utils::exp(lnt);

                            logresult = - Utils::log(scale) + (shape + 1)* lnt -t;
                            result = Utils::exp(logresult);
                            
                        }
                        else
                        {
                            
                            product = (shape * x - shape * location)/scale;

                            if (Utils::isinf(shape * x) && Utils::isinf(shape * location) && Utils::isnan(product) )
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                            }
                            else if (Utils::isinf(product))
                            {
                                if (Utils::isinf(x-location) && x != 0 && location != 0)
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                                }
                                else
                                {
                                    lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                                }
                            }
                            else
                            {
                                lnt = - 1/shape * Utils::log1p (product);
                            }

                            t = Utils::exp(lnt);

                            logresult = - Utils::log(scale) + (shape + 1)* lnt -t;
                            result = Utils::exp(logresult);
                        }
                    }
                }
            } 
            else 
            {
                assert (stats_enum_dist_cdf == type);

                if (0 == shape)
                {
                    if (
                        Utils::isinf(normalized_x) 
                        && Utils::isinf(x-location) 
                        && Utils::isinf(x/scale - location/scale) == false 
                        && Utils::isnan(x/scale - location/scale) == false
                        )
                    {
                        temp = x/scale - location/scale;
                        normalized_x = temp;
                    }

                    t = Utils::exp( - normalized_x );
                }
                else
                {
                    assert (0 != shape);
                    
                    
                    if (
                        Utils::isinf(normalized_x) 
                        && Utils::isinf(x-location) 
                        && Utils::isinf(x/scale - location/scale) == false 
                        && Utils::isnan(x/scale - location/scale) == false
                        )
                    {
                        temp = x/scale - location/scale;
                        normalized_x = temp;
                    }

                    if (Utils::isinf(normalized_x))
                    {
                        
                        product = (shape * x - shape * location)/scale;

                        if (Utils::isinf(shape * x) && Utils::isinf(shape * location) && Utils::isnan(product) )
                        {
                            lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                        }
                        else if (Utils::isinf(product))
                        {
                            if (Utils::isinf(x-location) && x != 0 && location != 0)
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                            }
                            else
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                            }
                        }
                        else
                        {
                            lnt = - 1/shape * Utils::log1p (product);
                        }
                    }
                    else
                    {
                        product = shape * normalized_x;
                        if (Utils::isinf(product))
                        {
                            assert (Utils::isinf(shape) == false);
                            assert (Utils::isinf(normalized_x) == false);

                            if (Utils::isinf(x-location) && x != 0 && location != 0)
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x) )+ Utils::log( Utils::abs(1-location/x) ) - Utils::log(scale) );
                            }
                            else
                            {
                                lnt = - 1/shape * ( Utils::log( Utils::abs(shape) ) + Utils::log( Utils::abs(x-location) ) - Utils::log(scale) );
                            }
                        }
                        else
                        {
                            lnt = - 1/shape * Utils::log1p (product);
                        }
                    }
                    t = Utils::exp(lnt);
                }


                if (shape < 0)
                {
                    if (x > boundary)
                    {
                        result = 1;
                    }
                    else
                    {
                        result = Utils::exp(-t);
                    }
                }
                else if ( 0 == shape )
                {
                    result = Utils::exp(-t);
                }
                else
                {
                    assert (shape > 0);
                    if (x < boundary)
                    {
                        result = 0;
                    }
                    else
                    {
                        result = Utils::exp(-t);
                    }
                }
            }  

        exitlabel:
            return result;
        }
    };

}

