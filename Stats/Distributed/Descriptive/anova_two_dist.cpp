



#include "stdafx.h"
#include "OlympCoreDist.h"
#include "anova_two_dist.h"

using namespace Descriptive;

TCErrorCode stats_infer_anova_two_compute_f_e_dist(const TCDistArray* data, 
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
    TCDistArray* s_cnt_row, 
    TCDistArray* s_sum_row, 
    TCDistArray* s_avg_row, 
    TCDistArray* s_var_row, 
    TCDistArray* s_cnt_col, 
    TCDistArray* s_sum_col, 
    TCDistArray* s_avg_col, 
    TCDistArray* s_var_col) 
{
    return stats_infer_anova_two_dist<TCFloat>(data, alpha, 
        f_row, f_col, 
        p_row, p_col, 
        f_crit_row, f_crit_col, 
        ss_row, ss_col, ss_err, ss_tot, 
        df_row, df_col, df_err, df_tot, 
        ms_row, ms_col, ms_err, 
        s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
        s_cnt_col, s_sum_col, s_avg_col, s_var_col);
}

TCErrorCode stats_infer_anova_two_compute_d_e_dist(const TCDistArray* data,
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
    TCDistArray* s_cnt_row, 
    TCDistArray* s_sum_row, 
    TCDistArray* s_avg_row, 
    TCDistArray* s_var_row, 
    TCDistArray* s_cnt_col, 
    TCDistArray* s_sum_col, 
    TCDistArray* s_avg_col, 
    TCDistArray* s_var_col)
{
    return stats_infer_anova_two_dist<TCDouble>(data, alpha, 
        f_row, f_col, 
        p_row, p_col, 
        f_crit_row, f_crit_col, 
        ss_row, ss_col, ss_err, ss_tot, 
        df_row, df_col, df_err, df_tot, 
        ms_row, ms_col, ms_err, 
        s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
        s_cnt_col, s_sum_col, s_avg_col, s_var_col);
}