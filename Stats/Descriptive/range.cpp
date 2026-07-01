//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "range.h"

using namespace Descriptive;

TCErrorCode stats_redall_range_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* range)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_range<TCFloat>(numbers, narrays, range);
}

TCErrorCode stats_redall_range_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* range)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_range<TCDouble>(numbers, narrays, range);
}

TCErrorCode stats_reddim_range_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* range)
{
    assert(numbers->m_tag == TC_FLOAT && range->m_tag == TC_FLOAT);
    return stats_reddim_range<TCFloat>(numbers, dim, range);
}

TCErrorCode stats_reddim_range_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* range)
{
    assert(numbers->m_tag == TC_DOUBLE && range->m_tag == TC_DOUBLE);
    return stats_reddim_range<TCDouble>(numbers, dim, range);
}