



#include "stdafx.h"
#include "var_s.h"

using namespace Descriptive;

TCErrorCode stats_redall_var_s_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* variance)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_var_s<TCFloat>(numbers, narrays, variance);
}

TCErrorCode stats_redall_var_s_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* variance)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_var_s<TCDouble>(numbers, narrays, variance);
}

TCErrorCode stats_reddim_var_s_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* variance)
{
    assert(numbers->m_tag == TC_FLOAT && variance->m_tag == TC_FLOAT);
    return stats_reddim_var_s<TCFloat>(numbers, dim, variance);
}

TCErrorCode stats_reddim_var_s_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* variance)
{
    assert(numbers->m_tag == TC_DOUBLE && variance->m_tag == TC_DOUBLE);
    return stats_reddim_var_s<TCDouble>(numbers, dim, variance);
}
