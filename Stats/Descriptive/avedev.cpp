//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "avedev.h"

using namespace Descriptive;

TCErrorCode stats_redall_avedev_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* ave_dev)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_avedev<TCFloat>(numbers, narrays, ave_dev);
}

TCErrorCode stats_redall_avedev_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* ave_dev)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_avedev<TCDouble>(numbers, narrays, ave_dev);
}

TCErrorCode stats_reddim_avedev_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* ave_dev)
{
    assert(numbers->m_tag == TC_FLOAT && ave_dev->m_tag == TC_FLOAT); 
    return stats_reddim_avedev<TCFloat>(numbers, dim, ave_dev);
}

TCErrorCode stats_reddim_avedev_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* ave_dev)
{
    assert(numbers->m_tag == TC_DOUBLE && ave_dev->m_tag == TC_DOUBLE);
    return stats_reddim_avedev<TCDouble>(numbers, dim, ave_dev);
}