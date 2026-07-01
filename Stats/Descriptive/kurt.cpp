//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "kurt.h"

using namespace Descriptive;

TCErrorCode stats_redall_kurt_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* kurtosis)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_kurt<TCFloat>(numbers, narrays, kurtosis);
}

TCErrorCode stats_redall_kurt_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* kurtosis)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_kurt<TCDouble>(numbers, narrays, kurtosis);
}

TCErrorCode stats_reddim_kurt_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* kurtosis)
{
    assert(numbers->m_tag == TC_FLOAT && kurtosis->m_tag == TC_FLOAT);
    return stats_reddim_kurt<TCFloat>(numbers, dim, kurtosis);
}

TCErrorCode stats_reddim_kurt_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* kurtosis)
{
    assert(numbers->m_tag == TC_DOUBLE && kurtosis->m_tag == TC_DOUBLE);
    return stats_reddim_kurt<TCDouble>(numbers, dim, kurtosis);
}
