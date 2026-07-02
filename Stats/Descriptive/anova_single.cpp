



#include "stdafx.h"
#include "anova_single.h"

using namespace Descriptive;

TCErrorCode stats_infer_anova_single_compute_f_e(const TCArray* data, 
    TCFloat alpha, 
    TCBool bycol,
    TCFloat* f, 
    TCFloat* p, 
    TCFloat* f_crit, 
    TCFloat* ssb, 
    TCFloat* ssw, 
    TCFloat* sst, 
    TCUInt64* dfb, 
    TCUInt64* dfw, 
    TCUInt64* dft, 
    TCFloat* msb, 
    TCFloat* msw, 
    TCArray* s_cnt, 
    TCArray* s_sum, 
    TCArray* s_avg, 
    TCArray* s_var) 
{
    assert(data->m_tag == TC_FLOAT);
    assert(s_cnt->m_tag == TC_UINT64);
    assert(s_sum->m_tag == TC_FLOAT);
    assert(s_avg->m_tag == TC_FLOAT);
    assert(s_var->m_tag == TC_FLOAT);

    return stats_infer_anova_single<TCFloat>(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
}


TCErrorCode stats_infer_anova_single_compute_d_e(const TCArray* data,
    TCDouble alpha, 
    TCBool bycol, 
    TCDouble* f, 
    TCDouble* p, 
    TCDouble* f_crit, 
    TCDouble* ssb, 
    TCDouble* ssw, 
    TCDouble* sst, 
    TCUInt64* dfb, 
    TCUInt64* dfw, 
    TCUInt64* dft,  
    TCDouble* msb, 
    TCDouble* msw, 
    TCArray* s_cnt, 
    TCArray* s_sum,
    TCArray* s_avg, 
    TCArray* s_var)
{
    assert(data->m_tag == TC_DOUBLE);
    assert(s_cnt->m_tag == TC_UINT64);
    assert(s_sum->m_tag == TC_DOUBLE);
    assert(s_avg->m_tag == TC_DOUBLE);
    assert(s_var->m_tag == TC_DOUBLE);

    return stats_infer_anova_single<TCDouble>(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
}
