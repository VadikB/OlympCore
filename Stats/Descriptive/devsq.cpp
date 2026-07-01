//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "devsq.h"

using namespace Descriptive;

TCErrorCode stats_redall_devsq_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* dev_sq)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_devsq<TCFloat>(numbers, narrays, dev_sq);
}

TCErrorCode stats_redall_devsq_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* dev_sq)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_devsq<TCDouble>(numbers, narrays, dev_sq);
}

TCErrorCode stats_reddim_devsq_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* dev_sq)
{
    assert(numbers->m_tag == TC_FLOAT && dev_sq->m_tag == TC_FLOAT);
    return stats_reddim_devsq<TCFloat>(numbers, dim, dev_sq);
}

TCErrorCode stats_reddim_devsq_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* dev_sq)
{
    assert(numbers->m_tag == TC_DOUBLE && dev_sq->m_tag == TC_DOUBLE);
    return stats_reddim_devsq<TCDouble>(numbers, dim, dev_sq);
}