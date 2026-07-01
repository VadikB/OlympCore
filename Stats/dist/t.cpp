//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "t.h"

using namespace Dist;

TCFloat stats_dist_t_f(const TCFloat x, const TCFloat deg_freedom, const TCBool cumulative) 
{
    if (cumulative)
    {
        return StudentT<TCFloat>::CDF(x, deg_freedom);
    }
    else
    {
        return StudentT<TCFloat>::PDF(x, deg_freedom);
    }
}

TCDouble stats_dist_t_d(const TCDouble x, const TCDouble deg_freedom, const TCBool cumulative)
{
    if (cumulative)
    {
        return StudentT<TCDouble>::CDF(x, deg_freedom);
    }
    else
    {
        return StudentT<TCDouble>::PDF(x, deg_freedom);
    }
}

TCFloat stats_dist_rt_t_f(const TCFloat x, const TCFloat deg_freedom) 
{
    return StudentT<TCFloat>::RT(x, deg_freedom);
}

TCDouble stats_dist_rt_t_d(const TCDouble x, const TCDouble deg_freedom)
{
    return StudentT<TCDouble>::RT(x, deg_freedom);
}

TCFloat stats_dist_2t_t_f(const TCFloat x, const TCFloat deg_freedom) 
{
    return StudentT<TCFloat>::TwoT(x, deg_freedom);
}

TCDouble stats_dist_2t_t_d(const TCDouble x, const TCDouble deg_freedom)
{
    return StudentT<TCDouble>::TwoT(x, deg_freedom);
}

TCFloat stats_dist_t_comp_f(const TCFloat x, const TCFloat deg_freedom, const TCFloat tails) 
{
    return StudentT<TCFloat>::Comp(x, deg_freedom, tails);
}

TCDouble stats_dist_t_comp_d(const TCDouble x, const TCDouble deg_freedom, const TCDouble tails)
{
    return StudentT<TCDouble>::Comp(x, deg_freedom, tails);
}

TCFloat stats_dist_inv_t_f(const TCFloat probability, const TCFloat deg_freedom) 
{
    return StudentT<TCFloat>::InvCDF(probability, deg_freedom);
}

TCDouble stats_dist_inv_t_d(const TCDouble probability, const TCDouble deg_freedom)
{
    return StudentT<TCDouble>::InvCDF(probability, deg_freedom);
}

TCFloat stats_dist_inv_rt_t_f(const TCFloat probability, const TCFloat deg_freedom) 
{
    return StudentT<TCFloat>::InvRT(probability, deg_freedom);
}

TCDouble stats_dist_inv_rt_t_d(const TCDouble probability, const TCDouble deg_freedom)
{
    return StudentT<TCDouble>::InvRT(probability, deg_freedom);
}

TCFloat stats_dist_inv_2t_t_f(const TCFloat probability, const TCFloat deg_freedom) 
{
    return StudentT<TCFloat>::InvTwoT(probability, deg_freedom);
}

TCDouble stats_dist_inv_2t_t_d(const TCDouble probability, const TCDouble deg_freedom)
{
    return StudentT<TCDouble>::InvTwoT(probability, deg_freedom);
}

TCFloat stats_t_confidence_f(const TCFloat alpha, const TCFloat standard_dev, const TCFloat size)
{
    return StudentT<TCFloat>::Confidence(alpha, standard_dev, size);
}

TCDouble stats_t_confidence_d(const TCDouble alpha, const TCDouble standard_dev, const TCDouble size)
{
    return StudentT<TCDouble>::Confidence(alpha, standard_dev, size);
}
