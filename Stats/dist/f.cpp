



#include "stdafx.h"
#include "f.h"

using namespace Dist;

TCFloat stats_dist_f_f(const TCFloat x, const TCFloat deg_freedom1, const TCFloat deg_freedom2, const TCBool cumulative)
{
    TCFloat result = 0;

    if (cumulative)
    {
        result = FDistribution<TCFloat>::CDF(x, deg_freedom1, deg_freedom2);
    }
    else
    {
        result = FDistribution<TCFloat>::PDF(x, deg_freedom1, deg_freedom2);
    }

    return result;
}

TCDouble stats_dist_f_d(const TCDouble x, const TCDouble deg_freedom1, const TCDouble deg_freedom2, const TCBool cumulative)
{
    TCDouble result = 0;

    if (cumulative)
    {
        result = FDistribution<TCDouble>::CDF(x, deg_freedom1, deg_freedom2);
    }
    else
    {
        result = FDistribution<TCDouble>::PDF(x, deg_freedom1, deg_freedom2);
    }

    return result;
}


TCFloat stats_dist_rt_f_f(const TCFloat x, const TCFloat deg_freedom1, const TCFloat deg_freedom2)
{
    return FDistribution<TCFloat>::RT(x, deg_freedom1, deg_freedom2);
}

TCDouble stats_dist_rt_f_d(const TCDouble x, const TCDouble deg_freedom1, const TCDouble deg_freedom2)
{
    return FDistribution<TCDouble>::RT(x, deg_freedom1, deg_freedom2);
}

TCFloat stats_dist_inv_f_f(const TCFloat probability, const TCFloat deg_freedom1, const TCFloat deg_freedom2)
{
    return FDistribution<TCFloat>::InvCDF(probability, deg_freedom1, deg_freedom2);
}

TCDouble stats_dist_inv_f_d(const TCDouble probability, const TCDouble deg_freedom1, const TCDouble deg_freedom2)
{
    return FDistribution<TCDouble>::InvCDF(probability, deg_freedom1, deg_freedom2);
}
 

TCFloat stats_dist_inv_rt_f_f(const TCFloat probability, const TCFloat deg_freedom1, const TCFloat deg_freedom2)
{
    return FDistribution<TCFloat>::InvRT(probability, deg_freedom1, deg_freedom2);
}

TCDouble stats_dist_inv_rt_f_d(const TCDouble probability, const TCDouble deg_freedom1, const TCDouble deg_freedom2)
{
    return FDistribution<TCDouble>::InvRT(probability, deg_freedom1, deg_freedom2);
}