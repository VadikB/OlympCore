//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "beta.h"

using namespace Dist;

TCFloat stats_dist_beta_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCBool cumulative, const TCFloat A, const TCFloat B)
{
    if(!cumulative)
    {
        return Beta<TCFloat>::PDF(x, alpha, beta, A, B);
    }

    return Beta<TCFloat>::CDF(x, alpha, beta, A,B);
}

TCDouble stats_dist_beta_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCBool cumulative, const TCDouble A, const TCDouble B)
{
    if(!cumulative)
    {
        return Beta<TCDouble>::PDF(x, alpha, beta, A, B);
    }

    return Beta<TCDouble>::CDF(x, alpha, beta, A,B);
}

TCFloat stats_dist_beta_comp_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCFloat A, const TCFloat B)
{
    return Beta<TCFloat>::CDF(x, alpha, beta, A,B);
}

TCDouble stats_dist_beta_comp_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCDouble A, const TCDouble B)
{
    return Beta<TCDouble>::CDF(x, alpha, beta, A,B);
}


TCFloat stats_dist_inv_beta_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCFloat A, const TCFloat B)
{
    return Beta<TCFloat>::InvCDF(x, alpha, beta, A, B);
}

TCDouble stats_dist_inv_beta_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCDouble A, const TCDouble B)
{
    return Beta<TCDouble>::InvCDF(x, alpha, beta, A, B);
}

TCFloat stats_dist_inv_beta_comp_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCFloat A, const TCFloat B)
{
    return Beta<TCFloat>::InvCDF(x, alpha, beta, A, B);
}

TCDouble stats_dist_inv_beta_comp_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCDouble A, const TCDouble B)
{
    return Beta<TCDouble>::InvCDF(x, alpha, beta, A, B);
}
