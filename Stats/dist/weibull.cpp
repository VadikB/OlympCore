



#include "stdafx.h"
#include "weibull.h"
using namespace Dist;

TCFloat stats_dist_weibull_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCBool cumulative) 
{
    if(!cumulative)
    {
        return Weibull<TCFloat>::PDF(x, alpha, beta);
    }
    return Weibull<TCFloat>::CDF(x, alpha, beta);
}

TCDouble stats_dist_weibull_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Weibull<TCDouble>::PDF(x, alpha, beta);
    }
    return Weibull<TCDouble>::CDF(x, alpha, beta);
}

TCFloat stats_dist_weibull_comp_f(const TCFloat x, const TCFloat alpha, const TCFloat beta, const TCBool cumulative) 
{
	return stats_dist_weibull_f(x, alpha, beta, cumulative);
}

TCDouble stats_dist_weibull_comp_d(const TCDouble x, const TCDouble alpha, const TCDouble beta, const TCBool cumulative)
{
	return stats_dist_weibull_d(x, alpha, beta, cumulative);
}

TCFloat stats_dist_inv_weibull_f(const TCFloat probability, const TCFloat alpha, const TCFloat beta) 
{
    return Weibull<TCFloat>::InvCDF(probability, alpha, beta);
}

TCDouble stats_dist_inv_weibull_d(const TCDouble probability, const TCDouble alpha, const TCDouble beta)
{
    return Weibull<TCDouble>::InvCDF(probability, alpha, beta);
}
