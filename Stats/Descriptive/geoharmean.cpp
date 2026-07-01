//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "geoharmean.h"

using namespace Descriptive;

TCErrorCode stats_reddim_geomean_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_FLOAT && mean->m_tag == TC_FLOAT);
    return stats_reddim_geomean<TCFloat>(numbers, dim, mean);
}

TCErrorCode stats_reddim_geomean_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_DOUBLE && mean->m_tag == TC_DOUBLE);
    return stats_reddim_geomean<TCDouble>(numbers, dim, mean);
}

TCErrorCode stats_redall_geomean_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* mean)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_geomean<TCFloat>(numbers, narrays, mean);
}

TCErrorCode stats_redall_geomean_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* mean)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_geomean<TCDouble>(numbers, narrays, mean);
}

TCErrorCode stats_reddim_harmean_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_FLOAT && mean->m_tag == TC_FLOAT);
    return stats_reddim_harmean<TCFloat>(numbers, dim, mean);
}

TCErrorCode stats_reddim_harmean_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_DOUBLE && mean->m_tag == TC_DOUBLE);
    return stats_reddim_harmean<TCDouble>(numbers, dim, mean);
}

TCErrorCode stats_redall_harmean_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* mean)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_harmean<TCFloat>(numbers, narrays, mean);
}

TCErrorCode stats_redall_harmean_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* mean)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_harmean<TCDouble>(numbers, narrays, mean);
}
