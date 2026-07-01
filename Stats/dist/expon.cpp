//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "expon.h"

using namespace Dist;

TCFloat stats_dist_expon_f(const TCFloat x, const TCFloat lambda, const TCBool cumulative)
{
    if (cumulative)
    {
        return Expon<TCFloat>::CDF(x, lambda);
    }
    else 
    {
        return Expon<TCFloat>::PDF(x, lambda);
    }
}


TCDouble stats_dist_expon_d(const TCDouble x, const TCDouble lambda, const TCBool cumulative)
{
    if (cumulative)
    {
        return Expon<TCDouble>::CDF(x, lambda);
    }
    else
    {
        return Expon<TCDouble>::PDF(x, lambda);
    }
}


TCFloat stats_dist_inv_expon_f(const TCFloat probability, const TCFloat lambda)
{
    return Expon<TCFloat>::InvCDF(probability, lambda);
}

TCDouble stats_dist_inv_expon_d(const TCDouble probability, const TCDouble lambda)
{
    return Expon<TCDouble>::InvCDF(probability, lambda);
}


