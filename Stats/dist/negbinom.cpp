//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "negbinom.h"
using namespace Dist;

TCFloat stats_dist_negbinom_f(const TCFloat number_f, const TCFloat number_s, const TCFloat probability_s, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Negbinom<TCFloat>::PMF(number_f, number_s, probability_s);
    }
    return Negbinom<TCFloat>::CDF(number_f, number_s, probability_s);
}

TCDouble stats_dist_negbinom_d(const TCDouble number_f, const TCDouble number_s, const TCDouble probability_s, const TCBool cumulative)
{
    if(!cumulative)
    {
        return Negbinom<TCDouble>::PMF(number_f, number_s, probability_s);
    }
    return Negbinom<TCDouble>::CDF(number_f, number_s, probability_s);
}


TCFloat stats_dist_negbinom_comp_f(const TCFloat number_f, const TCFloat number_s, const TCFloat probability_s)
{
    return Negbinom<TCFloat>::PMF(number_f, number_s, probability_s);
}

TCDouble stats_dist_negbinom_comp_d(const TCDouble number_f, const TCDouble number_s, const TCDouble probability_s)
{
    return Negbinom<TCDouble>::PMF(number_f, number_s, probability_s);
}
