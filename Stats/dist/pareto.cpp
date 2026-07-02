



#include "stdafx.h"
#include "pareto.h"

using namespace Dist;

TCFloat stats_dist_pareto_f(const TCFloat x, const TCFloat scale, const TCFloat shape, const TCBool cumulative) 
{
    if (cumulative)
    {
        return Pareto<TCFloat>::CDF(x, scale, shape);
    }
    else
    {
        return Pareto<TCFloat>::PDF(x, scale, shape);
    }
}

TCDouble stats_dist_pareto_d(const TCDouble x, const TCDouble scale, const TCDouble shape, const TCBool cumulative)
{
    if (cumulative)
    {
        return Pareto<TCDouble>::CDF(x, scale, shape);
    }
    else
    {
        return Pareto<TCDouble>::PDF(x, scale, shape);
    }
}

TCFloat stats_dist_inv_pareto_f(const TCFloat probability, const TCFloat scale, const TCFloat shape) 
{
    return Pareto<TCFloat>::InvCDF(probability, scale, shape);
}

TCDouble stats_dist_inv_pareto_d(const TCDouble probability, const TCDouble scale, const TCDouble shape)
{
    return Pareto<TCDouble>::InvCDF(probability, scale, shape);
}
