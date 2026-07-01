//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "uniform.h"

using namespace Dist;

TCFloat stats_dist_uniform_f(const TCFloat x, const TCFloat lower, const TCFloat upper, const TCBool cumulative) 
{
    if (cumulative)
    {
        return Uniform<TCFloat>::CDF(x, lower, upper);
    }
    else
    {
        return Uniform<TCFloat>::PDF(x, lower, upper);
    }
}

TCDouble stats_dist_uniform_d(const TCDouble x, const TCDouble lower, const TCDouble upper, const TCBool cumulative)
{
    if (cumulative)
    {
        return Uniform<TCDouble>::CDF(x, lower, upper);
    }
    else
    {
        return Uniform<TCDouble>::PDF(x, lower, upper);
    }
}

TCFloat stats_dist_inv_uniform_f(const TCFloat probability, const TCFloat lower, const TCFloat upper) 
{
    return Uniform<TCFloat>::InvCDF(probability, lower, upper);
}

TCDouble stats_dist_inv_uniform_d(const TCDouble probability, const TCDouble lower, const TCDouble upper)
{
    return Uniform<TCDouble>::InvCDF(probability, lower, upper);
}
