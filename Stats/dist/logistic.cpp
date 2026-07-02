



#include "stdafx.h"
#include "logistic.h"

using namespace Dist;

TCDouble stats_dist_logistic_d(const TCDouble x, const TCDouble mean, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Logistic<TCDouble>::CDF(x, mean, scale);
    }
    else
    {
        return Logistic<TCDouble>::PDF(x, mean, scale);
    }
}

TCFloat stats_dist_logistic_f(const TCFloat x, const TCFloat mean, const TCFloat scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Logistic<TCFloat>::CDF(x, mean, scale);
    }
    else
    {
        return Logistic<TCFloat>::PDF(x, mean, scale);
    }
}

TCDouble stats_dist_inv_logistic_d(const TCDouble probability, const TCDouble mean, const TCDouble scale)
{
    return Logistic<TCDouble>::InvCDF(probability, mean, scale);
}

TCFloat stats_dist_inv_logistic_f(const TCFloat probability, const TCFloat mean, const TCFloat scale)
{
    return Logistic<TCFloat>::InvCDF(probability, mean, scale);
}
