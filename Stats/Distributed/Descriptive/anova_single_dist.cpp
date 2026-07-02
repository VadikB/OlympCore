



#include "stdafx.h"
#include "OlympCoreDist.h"
#include "anova_single_dist.h"

using namespace Descriptive;

TCErrorCode stats_infer_anova_single_compute_d_e_dist(const TCDistArray* data,
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
    TCDistArray* s_cnt, 
    TCDistArray* s_sum,
    TCDistArray* s_avg, 
    TCDistArray* s_var)
{
    return stats_infer_anova_single_dist<TCDouble>(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
}

TCErrorCode stats_infer_anova_single_compute_f_e_dist(const TCDistArray* data,
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
    TCDistArray* s_cnt, 
    TCDistArray* s_sum,
    TCDistArray* s_avg, 
    TCDistArray* s_var)
{
    return stats_infer_anova_single_dist<TCFloat>(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
}