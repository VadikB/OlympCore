



#include "stdafx.h"
#include "poisson.h"

using namespace Dist;

TCFloat stats_dist_poisson_f(const TCFloat x, const TCFloat mean, const TCBool cumulative)
{
    if (cumulative)
    {
        return Poisson<TCFloat>::CDF(x, mean);
    }
    else
    {
        return Poisson<TCFloat>::PDF(x, mean);
    }
}


TCDouble stats_dist_poisson_d(const TCDouble x, const TCDouble mean, const TCBool cumulative)
{
    if (cumulative)
    {
        return Poisson<TCDouble>::CDF(x, mean);
    }
    else
    {
        return Poisson<TCDouble>::PDF(x, mean);
    }
}


TCFloat stats_dist_inv_poisson_f(const TCFloat probability, const TCFloat mean)
{
    return Poisson<TCFloat>::InvCDF(probability, mean);
}

TCDouble stats_dist_inv_poisson_d(const TCDouble probability, const TCDouble mean)
{
    return Poisson<TCDouble>::InvCDF(probability, mean);
}


