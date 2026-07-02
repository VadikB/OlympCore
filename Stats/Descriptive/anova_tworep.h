



#pragma once

#include "OlympCoreUtils.h"
#include "f.h"
#include <memory>

using namespace Dist;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_tworep(const TCArray* data,
    TCUInt64  cnt_row_sample,
    T   alpha,
    T*  f_sample,
    T*  f_col,
    T*  f_int,
    T*  p_sample,
    T*  p_col,
    T*  p_int,
    T*  f_crit_sample,
    T*  f_crit_col,
    T*  f_crit_int,
    T*  ss_sample,
    T*  ss_col,
    T*  ss_int,
    T*  ss_err,
    T*  ss_tot,
    TCUInt64* df_sample,
    TCUInt64* df_col,
    TCUInt64* df_int,
    TCUInt64* df_err,
    TCUInt64* df_tot,
    T*  ms_sample,
    T*  ms_col,
    T*  ms_int,
    T*  ms_err,
    TCArray* s_cnt_sample,
    TCArray* s_sum_sample,
    TCArray* s_avg_sample,
    TCArray* s_var_sample,
    TCArray* s_cnt_col,
    TCArray* s_sum_col,
    TCArray* s_avg_col,
    TCArray* s_var_col,
    TCArray* s_cnt_col_sample,
    TCArray* s_sum_col_sample,
    TCArray* s_avg_col_sample,
    TCArray* s_var_col_sample)
    {
        try
        {
            return AnovaTwoRep<T>::ComputeAnovaTwoRep(data, cnt_row_sample, alpha, 
                                                f_sample, f_col, f_int, 
                                                p_sample, p_col, p_int,
                                                f_crit_sample, f_crit_col, f_crit_int,
                                                ss_sample, ss_col, ss_int, ss_err, ss_tot, 
                                                df_sample, df_col, df_int, df_err, df_tot, 
                                                ms_sample, ms_col, ms_int, ms_err, 
                                                s_cnt_sample, s_sum_sample, s_avg_sample, s_var_sample, 
                                                s_cnt_col, s_sum_col, s_avg_col, s_var_col,
                                                s_cnt_col_sample, s_sum_col_sample, s_avg_col_sample, s_var_col_sample);
        }
        catch(std::bad_alloc &ba)
        {
            ba.what();
            return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
        }
        catch(...)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
    }

    template <typename T> class AnovaTwoRepDist;

    template <typename T>
    class AnovaTwoRep
    {
        friend class AnovaTwoRepDist<T>;

    private:
        static unsigned const ULPS = 8;

    public:
        static TCErrorCode ComputeAnovaTwoRep(const TCArray* data, 
        TCUInt64 cnt_row_sample,
        T   alpha,
        T*  f_sample, 
        T*  f_col, 
        T*  f_int, 
        T*  p_sample, 
        T*  p_col, 
        T*  p_int, 
        T*  f_crit_sample, 
        T*  f_crit_col, 
        T*  f_crit_int, 
        T*  ss_sample, 
        T*  ss_col, 
        T*  ss_int, 
        T*  ss_err, 
        T*  ss_tot, 
        TCUInt64* df_sample, 
        TCUInt64* df_col, 
        TCUInt64* df_int, 
        TCUInt64* df_err, 
        TCUInt64* df_tot, 
        T*  ms_sample, 
        T*  ms_col, 
        T*  ms_int, 
        T*  ms_err, 
        TCArray* s_cnt_sample, 
        TCArray* s_sum_sample, 
        TCArray* s_avg_sample, 
        TCArray* s_var_sample, 
        TCArray* s_cnt_col, 
        TCArray* s_sum_col,
        TCArray* s_avg_col, 
        TCArray* s_var_col,
        TCArray* s_cnt_col_sample,
        TCArray* s_sum_col_sample,
        TCArray* s_avg_col_sample,
        TCArray* s_var_col_sample)
        {
            if (!(data->m_numelt) 
                || (data->m_ndims != 2)
                || (data->m_dims[0] % cnt_row_sample)) return tcerror_code_new(TCError_IllegalSize, 1);

            if ((data->m_dims[0] == 1)
                || (data->m_dims[1] == 1)) return tcerror_code_new(TCError_IllegalInput, 1);

            T sum;
            T avg;
            T maxVal = 0;

            TCSize_t n_rows = data->m_dims[0];
            TCSize_t n_cols = data->m_dims[1];

            TCSize_t n_samples = data->m_dims[0] / cnt_row_sample;

            T* s_sum_sample_data = (T*)s_sum_sample->m_data;
            T* s_sum_col_data = (T*)s_sum_col->m_data;
            T* s_sum_col_sample_data = (T*)s_sum_col_sample->m_data;

            memset(s_sum_sample_data, 0, n_samples * sizeof(T));
            memset(s_sum_col_data, 0, n_cols * sizeof(T));
            memset(s_sum_col_sample_data, 0, n_samples * n_cols * sizeof(T));

            memset(s_var_sample->m_data, 0, n_samples * sizeof(T));
            memset(s_var_col->m_data, 0, n_cols * sizeof(T));
            memset(s_var_col_sample->m_data, 0, n_samples * n_cols * sizeof(T));

            TCErrorCode ec = ComputeSum(data, cnt_row_sample, s_sum_sample_data, s_sum_col_data, s_sum_col_sample_data, &sum, &maxVal);

            avg = sum / data->m_numelt;

            if (tcerror_code_get_errorid(ec) != TCError_NoError) return ec;
           
            *df_sample = n_samples - 1;
            *df_col = data->m_dims[1] - 1;
            *df_int = *df_sample * (*df_col);
            *df_tot = data->m_numelt - 1;

            *ss_sample = (T)0;
            *ss_col = (T)0;

            ComputeAvgSSFactor(cnt_row_sample, n_samples, cnt_row_sample * n_cols, n_cols, 
                               (TCUInt64*)s_cnt_sample->m_data, s_sum_sample_data, (T*)s_avg_sample->m_data, ss_sample, avg);

            ComputeAvgSSFactor(cnt_row_sample, n_cols, n_rows, n_samples, 
                               (TCUInt64*)s_cnt_col->m_data, s_sum_col_data, (T*)s_avg_col->m_data, ss_col, avg);

            ComputeVarSSInt(data, cnt_row_sample, 
                            n_samples, n_cols, 
                            (TCUInt64*)s_cnt_col_sample->m_data, (T*)s_sum_col_sample->m_data, (T*)s_avg_col_sample->m_data, (T*)s_var_col_sample->m_data,
                            (T*)s_avg_sample->m_data, (T*)s_avg_col->m_data,
                            (T*)s_var_sample->m_data, (T*)s_var_col->m_data,
                            avg, ss_int, ss_err);

            for (TCSize_t k = 0; k < n_cols; ++k)
            {
                GetData<T>(s_var_col, k) /= GetData<TCUInt64>(s_cnt_col, k) - 1;
            }

            ComputeF(alpha, 
                    *ss_sample, *ss_col, *ss_int, *ss_err, 
                    *df_sample, *df_col, *df_int, *df_tot, 
                    maxVal, 
                    f_sample, f_col, f_int, 
                    p_sample, p_col, p_int, 
                    f_crit_sample, f_crit_col, f_crit_int, 
                    ss_tot, 
                    df_err, 
                    ms_sample, ms_col, ms_int, ms_err);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:
        template <typename T>
        static T& GetData(const TCArray* data, TCSize_t index)
        {
            return (static_cast<T*>(data->m_data))[index];
        }

        template <typename T>
        static TCErrorCode ComputeSum(const TCArray* data, const TCUInt64 cnt_row_sample,
                                        T* s_sum_sample, 
                                        T* s_sum_col,
                                        T* s_sum_col_sample,
                                        T* sum_tot, T* maxVal)
        {
            *sum_tot = (T)0;

            for (TCSize_t index_arr = 0; index_arr < data->m_numelt; ++index_arr)
            {
                TCSize_t n_samples = data->m_dims[0] / cnt_row_sample;

                TCSize_t index_sample = (index_arr / cnt_row_sample) % n_samples;
                TCSize_t index_col = index_arr / data->m_dims[0];
                TCSize_t index_col_sample = index_sample + index_col * n_samples;

                T x = GetData<T>(data, index_arr);

                if (TCError::IsErrorOrMissingImpl<T>::run(&x))
                {
                    return tcerror_code_new(TCError_IllegalInput, 1);
                }

                s_sum_sample[index_sample] += x;
                s_sum_col[index_col] += x;
                s_sum_col_sample[index_col_sample] += x;

                *sum_tot += x;

                *maxVal = std::max<T>(*maxVal, fabs(x));
            }

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        template <typename T>
        static void ComputeAvgSSFactor(const TCUInt64 cnt_row_sample, const TCSize_t nelements, const TCSize_t cnt, const TCSize_t cnt_opp, 
                                       TCUInt64* s_cnt, T* s_sum, T* s_avg, T* ss, const T avg)
        {
            for (TCSize_t i = 0; i < nelements; ++i)
            {
                T avg_dim = s_sum[i] / cnt;

                *ss += (avg_dim - avg) * (avg_dim - avg);               
                
                s_cnt[i] = cnt;
                s_avg[i] = avg_dim;
            }

            *ss *= cnt_row_sample * cnt_opp;
        }

        template <typename T>
        static void ComputeVarSSInt(const TCArray* localData, const TCUInt64 cnt_row_sample, 
                                    const TCSize_t n_samples, const TCSize_t n_cols, 
                                    TCUInt64* s_cnt_col_sample_data, T* s_sum_col_sample_data, T* s_avg_col_sample_data, T* s_var_col_sample_data,
                                    T* s_avg_sample_data, T* s_avg_col_data,
                                    T* s_var_sample_data, T* s_var_col_data,
                                    const T avg_global, T* ss_int, T* ss_err)
        {
            *ss_int = (T)0;
            *ss_err = (T)0;

            for (TCSize_t j = 0; j < n_samples; ++j)
            {
                T avg_sample = s_avg_sample_data[j];
                T var_sample = 0;

                for (TCSize_t k = 0; k < n_cols; ++k)
                {
                    TCSize_t index_col_sample = j + k * n_samples;

                    s_cnt_col_sample_data[index_col_sample] = cnt_row_sample;
                    T avg_col_sample = s_sum_col_sample_data[index_col_sample] / cnt_row_sample;
                        
                    T avg_col = s_avg_col_data[k];

                    *ss_int += (avg_col_sample - avg_sample - avg_col + avg_global) * (avg_col_sample - avg_sample - avg_col + avg_global);

                    for (TCSize_t i = 0; i < cnt_row_sample; ++i)
                    {
                        TCSize_t index_arr = j * cnt_row_sample + k * cnt_row_sample * n_samples + i;
                        T x = GetData<T>(localData, index_arr);
                        *ss_err += (x - avg_col_sample) * (x - avg_col_sample);

                        var_sample += (x - avg_sample) * (x - avg_sample);
                        s_var_col_data[k] += (x - avg_col) * (x - avg_col);
                        s_var_col_sample_data[index_col_sample] += (x - avg_col_sample) * (x - avg_col_sample);
                    }

                    s_var_col_sample_data[index_col_sample] /= (cnt_row_sample - 1);
                    s_avg_col_sample_data[index_col_sample] = avg_col_sample;
                }

                s_var_sample_data[j] = var_sample / (cnt_row_sample * n_cols - 1);
            }

            *ss_int *= cnt_row_sample;
        }

        template <typename T>
        static void ComputeF(const T alpha, 
                             const T ss_sample, const T ss_col, const T ss_int, const T ss_err, 
                             const TCUInt64 df_sample, const TCUInt64 df_col, const TCUInt64 df_int, const TCUInt64 df_tot, 
                             const T maxVal, 
                             T* f_sample, T* f_col, T* f_int, 
                             T* p_sample, T* p_col, T* p_int, 
                             T* f_crit_sample, T* f_crit_col, T* f_crit_int, 
                             T* ss_tot, 
                             TCUInt64* df_err, 
                             T* ms_sample, T* ms_col, T* ms_int, T* ms_err)
        {
            T tolerance = ULPS * std::numeric_limits<T>::epsilon() * maxVal;
            
            *df_err = df_tot - df_sample - df_col - df_int;

            *ss_tot = ss_sample + ss_col + ss_err + ss_int;

            *ms_sample = ss_sample / df_sample;
            *ms_col = ss_col / df_col;
            *ms_int = ss_int / df_int;
            *ms_err = ss_err / *df_err;

            if (fabs(ss_err) < tolerance) 
            {
                *f_sample = ((fabs(ss_sample) < tolerance) ? 0 : Utils::infinity<T>());
                *f_col = ((fabs(ss_col) < tolerance) ? 0 : Utils::infinity<T>());
                *f_int = ((fabs(ss_int) < tolerance) ? 0 : Utils::infinity<T>());
            }
            else
            {
                *f_sample = (ss_sample * (*df_err)) / (ss_err * (df_sample));
                *f_col = (ss_col * (*df_err)) / (ss_err * (df_col));
                *f_int = (ss_int * (*df_err)) / (ss_err * (df_int));
            }

            *p_sample = FDistribution<T>::RT(*f_sample, df_sample, *df_err);
            *p_col = FDistribution<T>::RT(*f_col, df_col, *df_err);
            *p_int = FDistribution<T>::RT(*f_int, df_int, *df_err);

            *f_crit_sample = FDistribution<T>::InvRT(alpha, df_sample, *df_err);
            *f_crit_col = FDistribution<T>::InvRT(alpha, df_col, *df_err);
            *f_crit_int = FDistribution<T>::InvRT(alpha, df_int, *df_err);
        }
    };
}