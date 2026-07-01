//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "cauchy.h"

using namespace Dist;

TCFloat stats_dist_cauchy_f(const TCFloat x, const TCFloat location, const TCFloat scale, const TCBool cumulative) 
{
    if (cumulative)
    {
        return Cauchy<TCFloat>::CDF(x, location, scale);
    }
    else
    {
        return Cauchy<TCFloat>::PDF(x, location, scale);
    }
}

TCDouble stats_dist_cauchy_d(const TCDouble x, const TCDouble location, const TCDouble scale, const TCBool cumulative)
{
    if (cumulative)
    {
        return Cauchy<TCDouble>::CDF(x, location, scale);
    }
    else
    {
        return Cauchy<TCDouble>::PDF(x, location, scale);
    }
}

TCFloat stats_dist_inv_cauchy_f(const TCFloat probability, const TCFloat location, const TCFloat scale) 
{
    return Cauchy<TCFloat>::InvCDF(probability, location, scale);
}

TCDouble stats_dist_inv_cauchy_d(const TCDouble probability, const TCDouble location, const TCDouble scale)
{
    return Cauchy<TCDouble>::InvCDF(probability, location, scale);
}