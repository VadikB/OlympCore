



#include "stdafx.h"
#include "norm.h"

using namespace Dist;

TCFloat stats_dist_norm_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Norm<TCFloat>::PDF(x, mean, standard_dev);
    }
    
    return Norm<TCFloat>::CDF(x, mean, standard_dev);
}

TCDouble stats_dist_norm_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Norm<TCDouble>::PDF(x, mean, standard_dev);
    }

    return Norm<TCDouble>::CDF(x, mean, standard_dev);
}

TCFloat stats_dist_norm_comp_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev, const TCBool cumulative)
{
    return stats_dist_norm_f(x, mean, standard_dev, cumulative);
}

TCDouble stats_dist_norm_comp_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev, const TCBool cumulative)
{
    return stats_dist_norm_d(x, mean, standard_dev, cumulative);
}


TCFloat stats_dist_inv_norm_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev)
{
    return Norm<TCFloat>::InvCDF(x, mean, standard_dev);
}

TCDouble stats_dist_inv_norm_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev)
{
    return Norm<TCDouble>::InvCDF(x, mean, standard_dev);
}

TCFloat stats_dist_inv_norm_comp_f(const TCFloat x, const TCFloat mean, const TCFloat standard_dev)
{
    return Norm<TCFloat>::InvCDF(x, mean, standard_dev);
}

TCDouble stats_dist_inv_norm_comp_d(const TCDouble x, const TCDouble mean, const TCDouble standard_dev)
{
    return Norm<TCDouble>::InvCDF(x, mean, standard_dev);
}

TCFloat stats_norm_confidence_f(const TCFloat alpha, const TCFloat standard_dev, const TCFloat size)
{
    return Norm<TCFloat>::Confidence(alpha, standard_dev, size);
}

TCDouble stats_norm_confidence_d(const TCDouble alpha, const TCDouble standard_dev, const TCDouble size)
{
    return Norm<TCDouble>::Confidence(alpha, standard_dev, size);
}

TCFloat stats_norm_confidence_comp_f(const TCFloat alpha, const TCFloat standard_dev, const TCFloat size)
{
    return Norm<TCFloat>::Confidence(alpha, standard_dev, size);
}

TCDouble stats_norm_confidence_comp_d(const TCDouble alpha, const TCDouble standard_dev, const TCDouble size)
{
    return Norm<TCDouble>::Confidence(alpha, standard_dev, size);
}