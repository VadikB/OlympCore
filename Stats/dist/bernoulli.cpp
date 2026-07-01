//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "bernoulli.h"

using namespace Dist;

TCFloat stats_dist_bernoulli_f(const TCFloat x, const TCFloat success_probability, const TCBool cumulative)
{
    if (cumulative)
    {
        return Bernoulli<TCFloat>::CDF(x, success_probability);
    }
    else
    {
        return Bernoulli<TCFloat>::PMF(x, success_probability);
    }
}


TCDouble stats_dist_bernoulli_d(const TCDouble x, const TCDouble success_probability, const TCBool cumulative)
{
    if (cumulative)
    {
        return Bernoulli<TCDouble>::CDF(x, success_probability);
    }
    else
    {
        return Bernoulli<TCDouble>::PMF(x, success_probability);
    }
}


TCFloat stats_dist_inv_bernoulli_f(const TCFloat probability, const TCFloat success_probability)
{
    return Bernoulli<TCFloat>::InvCDF(probability, success_probability);
}

TCDouble stats_dist_inv_bernoulli_d(const TCDouble probability, const TCDouble success_probability)
{
    return Bernoulli<TCDouble>::InvCDF(probability, success_probability);
}


