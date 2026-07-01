//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "skew.h"

using namespace Descriptive;

TCErrorCode stats_redall_skew_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* skewness)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_skew<TCFloat>(numbers, narrays, skewness);
}

TCErrorCode stats_redall_skew_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* skewness)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_skew<TCDouble>(numbers, narrays, skewness);
}

TCErrorCode stats_reddim_skew_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* skewness)
{
    assert(numbers->m_tag == TC_FLOAT && skewness->m_tag == TC_FLOAT);
    return stats_reddim_skew<TCFloat>(numbers, dim, skewness);
}

TCErrorCode stats_reddim_skew_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* skewness)
{
    assert(numbers->m_tag == TC_DOUBLE && skewness->m_tag == TC_DOUBLE);
    return stats_reddim_skew<TCDouble>(numbers, dim, skewness);
}