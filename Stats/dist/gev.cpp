//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "gev.h"

using namespace Dist;

TCFloat stats_dist_gev_f(TCFloat x, TCFloat location, TCFloat scale, TCFloat shape, TCBool cumulative)
{
    if (cumulative)
    {
        return Gev<TCFloat>::CDF(x, location, scale, shape);
    }
    else
    {
        return Gev<TCFloat>::PDF(x, location, scale, shape);
    }
}


TCDouble stats_dist_gev_d(TCDouble x, TCDouble location, TCDouble scale, TCDouble shape, TCBool cumulative)
{
    if (cumulative)
    {
        return Gev<TCDouble>::CDF(x, location, scale, shape);
    }
    else
    {
        return Gev<TCDouble>::PDF(x, location, scale, shape);
    }
}


TCFloat stats_dist_inv_gev_f(TCFloat probability, TCFloat location, TCFloat scale, TCFloat shape)
{
    return Gev<TCFloat>::InvCDF(probability, location, scale, shape);
}

TCDouble stats_dist_inv_gev_d(TCDouble probability, TCDouble location, TCDouble scale, TCDouble shape)
{
    return Gev<TCDouble>::InvCDF(probability, location, scale, shape);
}


