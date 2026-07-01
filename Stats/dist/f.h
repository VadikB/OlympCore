//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "OlympCoreUtils.h"

namespace Dist
{

    template <typename T>
    class FDistribution
    {
    public:
        static T PDF(const T x, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0;
            STATS_DIST_TYPE dist_type = stats_enum_dist_pdf;
            result = p_dist_f(dist_type, x, deg_freedom1, deg_freedom2);
            return result;
        }

        static T CDF(const T x, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0;
            STATS_DIST_TYPE dist_type = stats_enum_dist_cdf;
            result = p_dist_f(dist_type, x, deg_freedom1, deg_freedom2);
            return result;
        }

        static T RT(const T x, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0.0;

            result = p_dist_f(stats_enum_dist_rcdf, x, deg_freedom1, deg_freedom2);

            return result;    
        }

        static T InvCDF(const T probability, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0;
            TCBool b_left = true;

            result = p_dist_inv_f(b_left, probability, deg_freedom1, deg_freedom2);

            return result;

        }

        static T InvRT(const T probability, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0;
            TCBool b_left = false;

            result = p_dist_inv_f(b_left, probability, deg_freedom1, deg_freedom2);

            return result;
        }

    protected:
        static T  p_dist_f(STATS_DIST_TYPE type, const T x, const T deg_freedom1, const T deg_freedom2)
        {
            T result = 0;
            T d1 =0, d2 =0;
            T z=0, z1=0, z2=0;
            T u=0;
            T numerator = 0, denominator = 0;
            T inf_val = Utils::infinity<T>();

            //check for NaN as betainc's NaN hanlding is not available now
            BAIL_ON_NAN(x, result);
            BAIL_ON_NAN(deg_freedom1, result);
            BAIL_ON_NAN(deg_freedom2, result);

            if (deg_freedom1 < 1 || Utils::isinf(deg_freedom1) )
            {
                result = TCError::NewError<T>(deg_freedom1,  TCFuncId_F_DIST, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if (deg_freedom2 < 1 || Utils::isinf(deg_freedom2) )
            {
                result = TCError::NewError<T>(deg_freedom2,  TCFuncId_F_DIST, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            d1 = Utils::floor(deg_freedom1);
            d2 = Utils::floor(deg_freedom2);

            if (stats_enum_dist_rcdf == type)
            {
                if(x <0)
                {
                    result = 1;
                }
                else if (Utils::isinf(x))   //x = +inf
                {
                    result = 0;
                }
                else 
                {
                    u = d2/(d1 * x + d2);
                    result = (T) Utils::betainc(u, (T)0.5*d2, (T)0.5*d1); 
                }
            }
            else if (stats_enum_dist_cdf == type)
            {
                if(x <0)
                {
                    result = 0;
                }
                else if (Utils::isinf(x))   //x = +inf
                {
                    result = 1;
                }
                else 
                {
                    if (Utils::isinf(d1)== false && Utils::isinf(x)== false && Utils::isinf(d1 *x))
                    {
                        //d1 * x is inf, cannot compute d1 * x, change a different format
                        z = 1.0 / (1.0 + d2/d1 /x);
                    }
                    else
                    {
                        z = (d1 * x)/(d1 * x + d2);
                    }
                    result = (T) Utils::betainc(z, (T)0.5*d1, (T)0.5*d2); 
                }
            }
            else if (stats_enum_dist_pdf == type)
            {
                if (x < 0)
                {
                    result = 0;
                }
                else if (Utils::isinf(x))   //x = +inf
                {
                    result = 0;
                }
                else if (0 == x)
                {
                    if (d1 < 2)
                    {
                        result = inf_val;
                    }
                    else if (2 == d1)
                    {
                        denominator = Utils::pow(d1/d2, (T)0.5*d1);
                        numerator = (T) Utils::beta((T)0.5*d1, (T)0.5*d2);
                        result = denominator / numerator;
                    }
                    else
                    {
                        assert (d1 > 2);
                        result = 0;
                    }
                }
                else
                {
                    z1 = (d1 * x)/(d1 * x + d2);
                    z2 = d2/(d1 *x + d2);
                    T d1t = (T)0.5*d1;
                    T d2t = (T)0.5*d2;

                    denominator = Utils::pow(z1, d1t) * Utils::pow(z2, d2t);
                    numerator = x* (T) Utils::beta(d1t, d2t);
                    result = denominator / numerator;

                    if (Utils::isnan(result))
                    {
                        T logresult = d1t * Utils::log(z1) + d2t * Utils::log(z2) 
                            - Utils::log(x) - Utils::betaln ( d1t, d2t);
                    
                        result = Utils::exp(logresult);
                    }
                }
            }    
            else
            {
                assert(false);
            }
        
        exitlabel:
            return result;
        }

        static T p_dist_inv_f(const TCBool b_left, const T probability, const T deg_freedom1, const T deg_freedom2)
        {
            T d1t, d2t, uz;
            T result = 0;
            T d1 =0, d2 =0;
            T zinv=0;
            T margin = 0.01;

            //check for NaN as betainc's NaN hanlding is not available now
            BAIL_ON_NAN(probability, result);
            BAIL_ON_NAN(deg_freedom1, result);
            BAIL_ON_NAN(deg_freedom2, result);

            if (deg_freedom1 < 1 || Utils::isinf(deg_freedom1) )
            {
                result = TCError::NewError<T>(deg_freedom1,  TCFuncId_F_INV, TCArgPosition_2, TCError_IllegalInput);
                BAIL();
            }

            if (deg_freedom2 < 1 || Utils::isinf(deg_freedom2) )
            {
                result = TCError::NewError<T>(deg_freedom2,  TCFuncId_F_INV, TCArgPosition_3, TCError_IllegalInput);
                BAIL();
            }

            //probability must be between 0 and 1
            if (probability < 0 || probability > 1)
            {
                result = TCError::NewError<T>(probability,  TCFuncId_F_INV, TCArgPosition_1, TCError_IllegalInput);
                BAIL();
            }

            d1 = Utils::floor(deg_freedom1);
            d2 = Utils::floor(deg_freedom2);
            if (b_left)
            {
                d1t = (T)0.5 * d1;;
                d2t = (T)0.5 * d2;
            }
            else
            {
                d1t = (T)0.5 * d2;;
                d2t = (T)0.5 * d1;
            }

            if (probability < 0.5 - margin)
            {
                uz = (T)Utils::invbetainc(probability, d1t, d2t);
                if (b_left)
                    result = (d2 * uz) / (d1 * (1 - uz));
                else
                    result = d2 * (1 - uz) / (d1 * uz);
            }
            else if(probability >= 0.5 + margin)
            {
                uz = (T) Utils::invbetainc(1-probability, d2t, d1t);
                if (b_left)
                    result = d2 * (1 - uz) / ( d1 * uz);
                else
                    result = (d2 * uz) / (d1 * (1 - uz));
            }
            else
            {
                uz = (T)Utils::invbetainc(probability, d1t, d2t);
                zinv = (T)Utils::invbetainc(1-probability, d2t, d1t);
                if (b_left)
                    result = (d2 * uz) / (d1 * zinv);
                else
                    result = (d2 * zinv) / (d1 * uz);
            }
        exitlabel:
            return result;
        }
    };
}
