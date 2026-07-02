



#include "stdafx.h"
#include "norm_s.h"

using namespace Dist;

TCFloat stats_dist_norm_s_f(const TCFloat z, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Norm_S<TCFloat>::PDF(z);
    }
    
    return Norm_S<TCFloat>::CDF(z);
}

TCDouble stats_dist_norm_s_d(const TCDouble z, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Norm_S<TCDouble>::PDF(z);
    }

    return Norm_S<TCDouble>::CDF(z);
}

TCFloat stats_dist_inv_norm_s_f(const TCFloat probability)
{
    return Norm_S<TCFloat>::InvCDF(probability);
}

TCDouble stats_dist_inv_norm_s_d(const TCDouble probability)
{
    return Norm_S<TCDouble>::InvCDF(probability);
}

TCFloat stats_dist_norm_s_comp_f(const TCFloat z)
{
    return Norm_S<TCFloat>::CDF(z);
}

TCDouble stats_dist_norm_s_comp_d(const TCDouble z)
{
    return Norm_S<TCDouble>::CDF(z);
}