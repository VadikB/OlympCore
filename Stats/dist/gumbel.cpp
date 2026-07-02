



#include "stdafx.h"
#include "gumbel.h"

using namespace Dist;


TCDouble stats_dist_max_gumbel_d(const TCDouble x, const TCDouble location, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Gumbel_Max<TCDouble>::CDF(x, location, scale);
    }
    else
    {
        return Gumbel_Max<TCDouble>::PDF(x, location, scale);
    }
}

TCFloat stats_dist_max_gumbel_f(const TCFloat x, const TCFloat location, const TCFloat scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Gumbel_Max<TCFloat>::CDF(x, location, scale);
    }
    else
    {
        return Gumbel_Max<TCFloat>::PDF(x, location, scale);
    }
}

TCDouble stats_dist_inv_max_gumbel_d(const TCDouble x, const TCDouble location, const TCDouble scale)
{
    return Gumbel_Max<TCDouble>::InvCDF(x, location, scale);
}

TCFloat stats_dist_inv_max_gumbel_f(const TCFloat x, const TCFloat location, const TCFloat scale)
{
    return Gumbel_Max<TCFloat>::InvCDF(x, location, scale);
}


TCDouble stats_dist_min_gumbel_d(const TCDouble x, const TCDouble location, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Gumbel_Min<TCDouble>::CDF(x, location, scale);
    }
    else
    {
        return Gumbel_Min<TCDouble>::PDF(x, location, scale);
    }
}

TCFloat stats_dist_min_gumbel_f(const TCFloat x, const TCFloat location, const TCFloat scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Gumbel_Min<TCFloat>::CDF(x, location, scale);
    }
    else
    {
        return Gumbel_Min<TCFloat>::PDF(x, location, scale);
    }
}

TCDouble stats_dist_inv_min_gumbel_d(const TCDouble x, const TCDouble location, const TCDouble scale)
{
    return Gumbel_Min<TCDouble>::InvCDF(x, location, scale);
}

TCFloat stats_dist_inv_min_gumbel_f(const TCFloat x, const TCFloat location, const TCFloat scale)
{
    return Gumbel_Min<TCFloat>::InvCDF(x, location, scale);
}