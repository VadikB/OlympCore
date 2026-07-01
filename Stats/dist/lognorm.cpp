//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "lognorm.h"

using namespace Dist;

// C interface functions
TCDouble stats_dist_lognorm_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev, const TCBool cumulative)
{
    if (cumulative)
    {
        return Lognorm<TCDouble>::CDF(x, mean, standard_dev);
    }
    else
    {
        return Lognorm<TCDouble>::PDF(x, mean, standard_dev);
    }
}

TCFloat stats_dist_lognorm_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev, const TCBool cumulative)
{
    if (cumulative)
    {
        return Lognorm<TCFloat>::CDF(x, mean, standard_dev);
    }
    else
    {
        return Lognorm<TCFloat>::PDF(x, mean, standard_dev);
    }
}

TCDouble stats_dist_inv_lognorm_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev)
{
    return Lognorm<TCDouble>::InvCDF(x, mean, standard_dev);
}

TCFloat stats_dist_inv_lognorm_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev)
{
    return Lognorm<TCFloat>::InvCDF(x, mean, standard_dev);
}

TCDouble stats_dist_lognorm_comp_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev)
{
    return Lognorm<TCDouble>::CDF(x, mean, standard_dev);
}

TCFloat stats_dist_lognorm_comp_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev)
{
    return Lognorm<TCFloat>::CDF(x, mean, standard_dev);
}