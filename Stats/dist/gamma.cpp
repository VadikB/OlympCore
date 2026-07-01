//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "gamma.h"
using namespace Dist;

TCFloat stats_dist_gamma_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Gamma<TCFloat>::PDF(x, alpha, beta);
    }
    return Gamma<TCFloat>::CDF(x, alpha, beta);
}

TCDouble stats_dist_gamma_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Gamma<TCDouble>::PDF(x, alpha, beta);
    }
    return Gamma<TCDouble>::CDF(x, alpha, beta);
}

TCFloat stats_dist_inv_gamma_f(const TCFloat x, const TCFloat alpha, const TCFloat beta)
{
    return Gamma<TCFloat>::InvCDF(x, alpha, beta);
}

TCDouble stats_dist_inv_gamma_d(const TCDouble x, const TCDouble alpha, const TCDouble beta)
{
    return Gamma<TCDouble>::InvCDF(x, alpha, beta);
}

TCFloat stats_dist_gamma_comp_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCBool cumulative)
{
	return stats_dist_gamma_f(x, alpha, beta, cumulative);
}

TCDouble stats_dist_gamma_comp_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCBool cumulative)
{
	return stats_dist_gamma_d(x, alpha, beta, cumulative);
}

TCFloat stats_dist_inv_gamma_comp_f(const TCFloat x, const TCFloat alpha, const TCFloat beta)
{
    return Gamma<TCFloat>::InvCDF(x, alpha, beta);
}

TCDouble stats_dist_inv_gamma_comp_d(const TCDouble x, const TCDouble alpha, const TCDouble beta)
{
    return Gamma<TCDouble>::InvCDF(x, alpha, beta);
}
