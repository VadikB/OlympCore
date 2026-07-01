//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "average.h"

using namespace Descriptive;

TCErrorCode stats_reddim_average_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_FLOAT && mean->m_tag == TC_FLOAT);
    return stats_reddim_average<TCFloat>(numbers, dim, mean);
}

TCErrorCode stats_reddim_average_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* mean)
{
    assert(numbers->m_tag == TC_DOUBLE && mean->m_tag == TC_DOUBLE);
    return stats_reddim_average<TCDouble>(numbers, dim, mean);
}

TCErrorCode stats_redall_average_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* mean)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_average<TCFloat>(numbers, narrays, mean);
}

TCErrorCode stats_redall_average_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* mean)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_average<TCDouble>(numbers, narrays, mean);
}
