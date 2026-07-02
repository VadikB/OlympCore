



#include "stdafx.h"
#include "laplace.h"

using namespace Dist;

TCFloat stats_dist_laplace_f(const TCFloat x, const TCFloat location, const TCFloat scale, const TCBool cumulative) 
{
    if (cumulative)
    {
        return Laplace<TCFloat>::CDF(x, location, scale);
    }
    else
    {
        return Laplace<TCFloat>::PDF(x, location, scale);
    }
}

TCDouble stats_dist_laplace_d(const TCDouble x, const TCDouble location, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Laplace<TCDouble>::CDF(x, location, scale);
    }
    else
    {
        return Laplace<TCDouble>::PDF(x, location, scale);
    }
}

TCFloat stats_dist_inv_laplace_f(const TCFloat probability, const TCFloat location, const TCFloat scale) 
{
    return Laplace<TCFloat>::InvCDF(probability, location, scale);
}

TCDouble stats_dist_inv_laplace_d(const TCDouble probability, const TCDouble location, const TCDouble scale)
{
    return Laplace<TCDouble>::InvCDF(probability, location, scale);
}