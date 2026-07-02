



#include "stdafx.h"
#include "moment_central.h"

using namespace Descriptive;

TCErrorCode stats_redall_moment_central_f_e(const TCArray* numbers, TCSize_t narrays, TCSize_t order, TCFloat* mu_central)
{
    assert(numbers->m_tag == TC_FLOAT);
    return stats_redall_moment_central<TCFloat>(numbers, narrays, order, mu_central);
}

TCErrorCode stats_redall_moment_central_d_e(const TCArray* numbers, TCSize_t narrays, TCSize_t order, TCDouble* mu_central)
{
    assert(numbers->m_tag == TC_DOUBLE);
    return stats_redall_moment_central<TCDouble>(numbers, narrays, order, mu_central);
}

TCErrorCode stats_reddim_moment_central_f_e(const TCArray* numbers, TCSize_t order, TCSize_t dim, TCArray* mu_central)
{
    assert(numbers->m_tag == TC_FLOAT && mu_central->m_tag == TC_FLOAT); 
    return stats_reddim_moment_central<TCFloat>(numbers, order, dim, mu_central);
}

TCErrorCode stats_reddim_moment_central_d_e(const TCArray* numbers, TCSize_t order, TCSize_t dim, TCArray* mu_central)
{
    assert(numbers->m_tag == TC_DOUBLE && mu_central->m_tag == TC_DOUBLE);
    return stats_reddim_moment_central<TCDouble>(numbers, order, dim, mu_central);
}