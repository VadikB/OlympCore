



#include "stdafx.h"
#include "geometric.h"

using namespace Dist;


TCDouble stats_dist_geometric_d(const TCDouble x, const TCDouble p, const TCBool cumulative)
{
    if (cumulative)
    {
        return Dist::Geometric<TCDouble>::CDF(x, p);
    }
    else
    {
        return Dist::Geometric<TCDouble>::PMF(x, p);
    }
}

TCFloat stats_dist_geometric_f(const TCFloat x, const TCFloat p, const TCBool cumulative)
{
    if (cumulative)
    {
        return Dist::Geometric<TCFloat>::CDF(x, p);
    }
    else
    {
        return Dist::Geometric<TCFloat>::PMF(x, p);
    }
}

TCDouble stats_dist_inv_geometric_d(const TCDouble x, const TCDouble p)
{
    return Dist::Geometric<TCDouble>::InvCDF(x, p);
}

TCFloat stats_dist_inv_geometric_f(const TCFloat x, const TCFloat p)
{
    return Dist::Geometric<TCFloat>::InvCDF(x, p);
}