//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "stdev_s.h"

using namespace Descriptive;

TCErrorCode stats_redall_stdev_s_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* stdev)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_stdev_s<TCFloat>(numbers, narrays, stdev);
}

TCErrorCode stats_redall_stdev_s_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* stdev)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_stdev_s<TCDouble>(numbers, narrays, stdev);
}

TCErrorCode stats_reddim_stdev_s_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* stdev)
{
    assert(numbers->m_tag == TC_FLOAT && stdev->m_tag == TC_FLOAT);
    return stats_reddim_stdev_s<TCFloat>(numbers, dim, stdev);
}

TCErrorCode stats_reddim_stdev_s_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* stdev)
{
    assert(numbers->m_tag == TC_DOUBLE && stdev->m_tag == TC_DOUBLE);
    return stats_reddim_stdev_s<TCDouble>(numbers, dim, stdev);
}
