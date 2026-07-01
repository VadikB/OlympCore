//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "rayleigh.h"

using namespace Dist;

// C interface functions
TCDouble stats_dist_rayleigh_d(const TCDouble x, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Rayleigh<TCDouble>::CDF(x, scale);
    }
    else
    {
        return Rayleigh<TCDouble>::PDF(x, scale);
    }
}

TCFloat stats_dist_rayleigh_f(const TCFloat x, const TCFloat scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Rayleigh<TCFloat>::CDF(x, scale);
    }
    else
    {
        return Rayleigh<TCFloat>::PDF(x, scale);
    }
}

TCDouble stats_dist_inv_rayleigh_d(const TCDouble x, const TCDouble scale)
{
    return Rayleigh<TCDouble>::InvCDF(x, scale);
}

TCFloat stats_dist_inv_rayleigh_f(const TCFloat x, const TCFloat scale)
{
    return Rayleigh<TCFloat>::InvCDF(x, scale);
}

