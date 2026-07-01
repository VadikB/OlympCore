//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "moment.h"

using namespace Descriptive;

TCErrorCode stats_redall_moment_f_e(const TCArray* numbers, TCSize_t narrays, const TCFloat center, TCSize_t order, TCFloat* mu)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_moment<TCFloat>(numbers, narrays, center, order, mu);
}

TCErrorCode stats_redall_moment_d_e(const TCArray* numbers, TCSize_t narrays, const TCDouble center, TCSize_t order, TCDouble* mu)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_moment<TCDouble>(numbers, narrays, center, order, mu);
}

TCErrorCode stats_reddim_moment_f_e(const TCArray* numbers, const TCFloat center, TCSize_t order, TCSize_t dim, TCArray* mu)
{
    assert(numbers->m_tag == TC_FLOAT && mu->m_tag == TC_FLOAT); 
    return stats_reddim_moment<TCFloat>(numbers, center, order, dim, mu);
}

TCErrorCode stats_reddim_moment_d_e(const TCArray* numbers, const TCDouble center, TCSize_t order, TCSize_t dim, TCArray* mu)
{
    assert(numbers->m_tag == TC_DOUBLE && mu->m_tag == TC_DOUBLE);
    return stats_reddim_moment<TCDouble>(numbers, center, order, dim, mu);
}