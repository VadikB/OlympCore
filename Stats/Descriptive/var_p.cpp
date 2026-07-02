



#include "stdafx.h"
#include "var_p.h"

using namespace Descriptive;

TCErrorCode stats_redall_var_p_f_e(const TCArray* numbers, const TCSize_t narrays, TCFloat* variance)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_var_p<TCFloat>(numbers, narrays, variance);
}

TCErrorCode stats_redall_var_p_d_e(const TCArray* numbers, const TCSize_t narrays, TCDouble* variance)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_var_p<TCDouble>(numbers, narrays, variance);
}

TCErrorCode stats_reddim_var_p_f_e(const TCArray* numbers, const TCSize_t dim, TCArray* variance)
{
    assert(numbers->m_tag == TC_FLOAT && variance->m_tag == TC_FLOAT);
    return stats_reddim_var_p<TCFloat>(numbers, dim, variance);
}

TCErrorCode stats_reddim_var_p_d_e(const TCArray* numbers, const TCSize_t dim, TCArray* variance)
{
    assert(numbers->m_tag == TC_DOUBLE && variance->m_tag == TC_DOUBLE);
    return stats_reddim_var_p<TCDouble>(numbers, dim, variance);
}
