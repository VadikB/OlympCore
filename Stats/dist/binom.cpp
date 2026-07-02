



#include "stdafx.h"
#include "binom.h"

using namespace Dist;

TCDouble stats_dist_binom_d(const TCDouble number_s, const TCDouble trials, const TCDouble probability_s,
    const TCBool cumulative)
{
    if (cumulative)
    {
        return Binom<TCDouble>::CDF(number_s, trials, probability_s);
    }
    else
    {
        return Binom<TCDouble>::PMF(number_s, trials, probability_s);
    }
}

TCFloat stats_dist_binom_f(const TCFloat number_s, const TCFloat trials, const TCFloat probability_s,
    const TCBool cumulative)
{
    if (cumulative)
    {
        return Binom<TCFloat>::CDF(number_s, trials, probability_s);
    }
    else
    {
        return Binom<TCFloat>::PMF(number_s, trials, probability_s);
    }
}

TCDouble stats_dist_inv_binom_d(const TCDouble trials, const TCDouble probability_s, const TCDouble probability)
{
    return Binom<TCDouble>::InvCDF(probability, trials, probability_s);
}

TCFloat stats_dist_inv_binom_f(const TCFloat trials, const TCFloat probability_s, const TCFloat probability)
{
    return Binom<TCFloat>::InvCDF(probability, trials, probability_s);
}
