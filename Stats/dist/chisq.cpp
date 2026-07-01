//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "chisq.h"

using namespace Dist;

///////////////////////////////////////////////////////////
// CHISQ.DIST
///////////////////////////////////////////////////////////
TCDouble stats_dist_chisq_d(const TCDouble x, const TCDouble deg_freedom, const TCBool cumulative)
{
    if (cumulative)
    {
        return Chisq<TCDouble>::CDF(x, deg_freedom);
    }
    else
    {
        return Chisq<TCDouble>::PDF(x, deg_freedom);
    }
}

TCFloat stats_dist_chisq_f(const TCFloat x, const TCFloat deg_freedom, const TCBool cumulative)
{
    if (cumulative)
    {
        return Chisq<TCFloat>::CDF(x, deg_freedom);
    }
    else
    {
        return Chisq<TCFloat>::PDF(x, deg_freedom);
    }
}


///////////////////////////////////////////////////////////
// CHISQ.INV
///////////////////////////////////////////////////////////
TCDouble stats_dist_inv_chisq_d(const TCDouble probability, const TCDouble deg_freedom)
{
    return Chisq<TCDouble>::InvCDF(probability, deg_freedom);
}

TCFloat stats_dist_inv_chisq_f(const TCFloat probability, const TCFloat deg_freedom)
{
    return Chisq<TCFloat>::InvCDF(probability, deg_freedom);
}


///////////////////////////////////////////////////////////
// CHISQ.DIST.RT
///////////////////////////////////////////////////////////
TCDouble stats_dist_rt_chisq_d(const TCDouble x, const TCDouble deg_freedom)
{
    return Chisq<TCDouble>::RT(x, deg_freedom);
}

TCFloat stats_dist_rt_chisq_f(const TCFloat x, const TCFloat deg_freedom)
{
    return Chisq<TCFloat>::RT(x, deg_freedom);
}


///////////////////////////////////////////////////////////
// CHISQ.INV.RT
///////////////////////////////////////////////////////////
TCDouble stats_dist_inv_rt_chisq_d(const TCDouble probability, const TCDouble deg_freedom)
{
    return Chisq<TCDouble>::InvRT(probability, deg_freedom);
}

TCFloat stats_dist_inv_rt_chisq_f(const TCFloat probability, const TCFloat deg_freedom)
{
    return Chisq<TCFloat>::InvRT(probability, deg_freedom);
}
