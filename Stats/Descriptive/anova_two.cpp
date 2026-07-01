//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "anova_two.h"

using namespace Descriptive;

TCErrorCode stats_infer_anova_two_compute_f_e(const TCArray* data, 
    TCFloat alpha, 
    TCFloat* f_row, 
    TCFloat* f_col, 
    TCFloat* p_row, 
    TCFloat* p_col, 
    TCFloat* f_crit_row, 
    TCFloat* f_crit_col, 
    TCFloat* ss_row, 
    TCFloat* ss_col, 
    TCFloat* ss_err, 
    TCFloat* ss_tot, 
    TCUInt64* df_row, 
    TCUInt64* df_col, 
    TCUInt64* df_err, 
    TCUInt64* df_tot, 
    TCFloat* ms_row, 
    TCFloat* ms_col, 
    TCFloat* ms_err, 
    TCArray* s_cnt_row, 
    TCArray* s_sum_row, 
    TCArray* s_avg_row, 
    TCArray* s_var_row, 
    TCArray* s_cnt_col, 
    TCArray* s_sum_col, 
    TCArray* s_avg_col, 
    TCArray* s_var_col) 
{
    assert(data->m_tag == TC_FLOAT);
    assert((s_cnt_row)->m_tag == TC_UINT64);
    assert((s_sum_row)->m_tag == TC_FLOAT);
    assert((s_avg_row)->m_tag == TC_FLOAT);
    assert((s_var_row)->m_tag == TC_FLOAT);

    return stats_infer_anova_two<TCFloat>(data, alpha, 
        f_row, f_col, 
        p_row, p_col, 
        f_crit_row, f_crit_col, 
        ss_row, ss_col, ss_err, ss_tot, 
        df_row, df_col, df_err, df_tot, 
        ms_row, ms_col, ms_err, 
        s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
        s_cnt_col, s_sum_col, s_avg_col, s_var_col);
}


TCErrorCode stats_infer_anova_two_compute_d_e(const TCArray* data,
    TCDouble alpha, 
    TCDouble* f_row, 
    TCDouble* f_col, 
    TCDouble* p_row, 
    TCDouble* p_col, 
    TCDouble* f_crit_row, 
    TCDouble* f_crit_col, 
    TCDouble* ss_row, 
    TCDouble* ss_col, 
    TCDouble* ss_err, 
    TCDouble* ss_tot, 
    TCUInt64* df_row, 
    TCUInt64* df_col, 
    TCUInt64* df_err, 
    TCUInt64* df_tot, 
    TCDouble* ms_row, 
    TCDouble* ms_col, 
    TCDouble* ms_err, 
    TCArray* s_cnt_row, 
    TCArray* s_sum_row, 
    TCArray* s_avg_row, 
    TCArray* s_var_row, 
    TCArray* s_cnt_col, 
    TCArray* s_sum_col, 
    TCArray* s_avg_col, 
    TCArray* s_var_col)
{
    assert(data->m_tag == TC_DOUBLE);
    assert((s_cnt_row)->m_tag == TC_UINT64);
    assert((s_sum_row)->m_tag == TC_DOUBLE);
    assert((s_avg_row)->m_tag == TC_DOUBLE);
    assert((s_var_row)->m_tag == TC_DOUBLE);

    return stats_infer_anova_two<TCDouble>(data, alpha, 
        f_row, f_col, 
        p_row, p_col, 
        f_crit_row, f_crit_col, 
        ss_row, ss_col, ss_err, ss_tot, 
        df_row, df_col, df_err, df_tot, 
        ms_row, ms_col, ms_err, 
        s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
        s_cnt_col, s_sum_col, s_avg_col, s_var_col);
}
