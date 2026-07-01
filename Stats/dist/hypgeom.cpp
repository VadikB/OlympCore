//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "Hypgeom.h"

using namespace Dist;

// C interface functions
TCDouble stats_dist_hypgeom_d(const TCDouble sample_s, const TCDouble number_sample, const TCDouble population_s, const TCDouble number_population, const TCBool cumulative)
{
    if (cumulative)
    {
        return Hypgeom<TCDouble>::CDF(sample_s, number_sample, population_s, number_population);
    }
    else
    {
        return Hypgeom<TCDouble>::PMF(sample_s, number_sample, population_s, number_population);
    }
}

TCFloat stats_dist_hypgeom_f(const TCFloat sample_s, const TCFloat number_sample, const TCFloat population_s, const TCFloat number_population, const TCBool cumulative)
{
    if (cumulative)
    {
        return Hypgeom<TCFloat>::CDF(sample_s, number_sample, population_s, number_population);
    }
    else
    {
        return Hypgeom<TCFloat>::PMF(sample_s, number_sample, population_s, number_population);
    }
}

TCDouble stats_dist_inv_hypgeom_d(const TCDouble probability, const TCDouble number_sample, const TCDouble population_s, const TCDouble number_population)
{
    return Hypgeom<TCDouble>::InvCDF(probability, number_sample, population_s, number_population);
}

TCFloat stats_dist_inv_hypgeom_f(const TCFloat probability, const TCFloat number_sample, const TCFloat population_s, const TCFloat number_population)
{
    return Hypgeom<TCFloat>::InvCDF(probability, number_sample, population_s, number_population);
}

TCDouble stats_dist_hypgeom_comp_d(const TCDouble sample_s, const TCDouble number_sample, const TCDouble population_s, const TCDouble number_population)
{
    return Hypgeom<TCDouble>::PMF(sample_s, number_sample, population_s, number_population);
}

TCFloat stats_dist_hypgeom_comp_f(const TCFloat sample_s, const TCFloat number_sample, const TCFloat population_s, const TCFloat number_population)
{
    return Hypgeom<TCFloat>::PMF(sample_s, number_sample, population_s, number_population);
}