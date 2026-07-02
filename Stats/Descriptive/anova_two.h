



#pragma once

#include "OlympCoreUtils.h"
#include "f.h"
#include <memory>
#include <algorithm>

using namespace Dist;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_two(const TCArray* data,
    T alpha, 
    T* f_row, 
    T* f_col, 
    T* p_row, 
    T* p_col, 
    T* f_crit_row, 
    T* f_crit_col, 
    T* ss_row, 
    T* ss_col, 
    T* ss_err, 
    T* ss_tot, 
    TCUInt64* df_row, 
    TCUInt64* df_col, 
    TCUInt64* df_err, 
    TCUInt64* df_tot, 
    T* ms_row, 
    T* ms_col, 
    T* ms_err, 
    TCArray* s_cnt_row, 
    TCArray* s_sum_row, 
    TCArray* s_avg_row, 
    TCArray* s_var_row, 
    TCArray* s_cnt_col, 
    TCArray* s_sum_col, 
    TCArray* s_avg_col, 
    TCArray* s_var_col)
    {
        try
        {
            return AnovaTwo<T>::ComputeAnovaTwo(data, alpha, 
                                                f_row, f_col, 
                                                p_row, p_col, 
                                                f_crit_row, f_crit_col, 
                                                ss_row, ss_col, ss_err, ss_tot, 
                                                df_row, df_col, df_err, df_tot, 
                                                ms_row, ms_col, ms_err, 
                                                s_cnt_row, s_sum_row, s_avg_row, s_var_row, 
                                                s_cnt_col, s_sum_col, s_avg_col, s_var_col);
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

    template <typename T> class AnovaTwoDist;

    template <typename T>
    class AnovaTwo
    {
        friend class AnovaTwoDist<T>;

    private:
        static unsigned const ULPS = 8;

    public:
        static TCErrorCode ComputeAnovaTwo(const TCArray* data, 
        T alpha, 
        T* f_row, 
        T* f_col, 
        T* p_row, 
        T* p_col, 
        T* f_crit_row, 
        T* f_crit_col, 
        T* ss_row, 
        T* ss_col, 
        T* ss_err, 
        T* ss_tot, 
        TCUInt64* df_row, 
        TCUInt64* df_col, 
        TCUInt64* df_err, 
        TCUInt64* df_tot, 
        T* ms_row, 
        T* ms_col, 
        T* ms_err, 
        TCArray* s_cnt_row, 
        TCArray* s_sum_row, 
        TCArray* s_avg_row, 
        TCArray* s_var_row, 
        TCArray* s_cnt_col, 
        TCArray* s_sum_col, 
        TCArray* s_avg_col, 
        TCArray* s_var_col)
        {
            if (!(data->m_numelt) 
                || (data->m_ndims != 2)) 
            {
                return tcerror_code_new(TCError_IllegalSize, 1);
            }

            T sum;
            T maxVal = 0;
            TCSize_t cnt = data->m_numelt;

            TCSize_t rows = data->m_dims[0];
            TCSize_t cols = data->m_dims[1];

            memset(s_cnt_row->m_data, 0, rows * sizeof(TCUInt64));
            memset(s_cnt_col->m_data, 0, cols * sizeof(TCUInt64));

            memset(s_sum_row->m_data, 0, rows * sizeof(T));
            memset(s_sum_col->m_data, 0, cols * sizeof(T));

            TCErrorCode ec = ComputeSum(data, (T*)s_sum_row->m_data, (T*)s_sum_col->m_data, &sum, &maxVal);

            if (tcerror_code_get_errorid(ec) != TCError_NoError) return ec;

            T avg = sum / cnt;

            *ss_tot = 0;

            ComputeAvgVarSS<T, false>(data, 0, avg, cols, (T*)s_sum_row->m_data, 
                                      (TCUInt64*)s_cnt_row->m_data, (T*)s_avg_row->m_data, (T*)s_var_row->m_data, ss_row, ss_tot);
                       
            ComputeAvgVarSS<T, true>(data, 1, avg, rows, (T*)s_sum_col->m_data, 
                                      (TCUInt64*)s_cnt_col->m_data, (T*)s_avg_col->m_data, (T*)s_var_col->m_data, ss_col, ss_tot);

            *df_row = rows - 1;
            *df_col = cols - 1;
            *df_tot = cnt - 1;

            ComputeF(alpha, *ss_row, *ss_col, *ss_tot, *df_row, *df_col, *df_tot, maxVal, 
                     f_row, f_col, p_row, p_col, f_crit_row, f_crit_col, ss_err, df_err, ms_row, ms_col, ms_err);

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:

        template <typename T>
        static T& GetData(const TCArray* data, TCSize_t index)
        {
            return (static_cast<T*>(data->m_data))[index];
        }

        template <typename T>
        static TCErrorCode ComputeSum(const TCArray* data, 
                                                T* s_sum_row, 
                                                T* s_sum_col,
                                                T* sum_tot, T* maxVal)
        {
            *sum_tot = 0;
            TCSize_t rows = data->m_dims[0];

            for (TCSize_t index_arr = 0; index_arr < data->m_numelt; ++index_arr)
            {
                TCSize_t index_row = index_arr % rows;
                TCSize_t index_col = index_arr / rows;

                T x = GetData<T>(data, index_arr);
                
                if (TCError::IsErrorOrMissingImpl<T>::run(&x))
                {
                    return tcerror_code_new(TCError_IllegalInput, 1);
                }

                s_sum_row[index_row] += x;
                s_sum_col[index_col] += x;

                *sum_tot += x;

                *maxVal = std::max<T>(*maxVal, fabs(x));
            }

            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        template <typename T, TCBool computeSSTotal>
        static void ComputeAvgVarSS(const TCArray* data, const TCSize_t dim, const T avg_tot, const TCSize_t cnt_dim,  const T* s_sum_dim, 
                                    TCUInt64* s_cnt_dim, T* s_avg_dim, T* s_var_dim, T* ss_dim, T* ss_tot, bool varDistributed = false)
        {
            *ss_dim = 0; 
            TCSize_t index_dim[2];

            for (index_dim[dim] = 0; index_dim[dim] < data->m_dims[dim]; ++(index_dim[dim]))
            {
                T avg_dim = s_sum_dim[index_dim[dim]] / cnt_dim;

                *ss_dim += (avg_dim - avg_tot) * (avg_dim - avg_tot);

                T var = 0;
                for (index_dim[(dim + 1) % 2] = 0; index_dim[(dim + 1) % 2] < data->m_dims[(dim + 1) % 2]; ++(index_dim[(dim + 1) % 2]))
                {
                    TCSize_t index_arr = index_dim[0] + index_dim[1] * data->m_dims[0];

                    T x = GetData<T>(data, index_arr);

                    if (computeSSTotal) 
                    {
                        *ss_tot += (x - avg_tot) * (x - avg_tot);
                    }
                    var += (x - avg_dim) * (x - avg_dim);
                }
                
                s_var_dim[index_dim[dim]] = ((varDistributed) ? var : var / (cnt_dim - 1));

                s_avg_dim[index_dim[dim]] = avg_dim;
                s_cnt_dim[index_dim[dim]] = cnt_dim;
            }

            *ss_dim *= cnt_dim;
        }

        template <typename T>
        static void ComputeF(const T alpha, 
                             const T ss_row, const T ss_col, const T ss_tot, 
                             const TCUInt64 df_row, const TCUInt64 df_col, const TCUInt64 df_tot, 
                             const T maxVal, 
                             T* f_row, T* f_col, T* p_row, T* p_col, T* f_crit_row, T* f_crit_col, 
                             T* ss_err, 
                             TCUInt64* df_err, 
                             T* ms_row, T* ms_col, T* ms_err)
        {
                *df_err = df_tot - df_row - df_col;

                *ss_err = ss_tot - ss_row - ss_col;

                *ms_row = ss_row / df_row;
                *ms_col = ss_col / df_col;
                *ms_err = *ss_err / *df_err;

                T tolerance = ULPS * std::numeric_limits<T>::epsilon() * maxVal;

                if (fabs(*ss_err) < tolerance) 
                {
                    *f_row = ((fabs(ss_row) < tolerance) ? 0 : Utils::infinity<T>());
                    *f_col = ((fabs(ss_col) < tolerance) ? 0 : Utils::infinity<T>());
                }
                else
                {
                    *f_row = (ss_row * (*df_err)) / (*ss_err * df_row);
                    *f_col = (ss_col * (*df_err)) / (*ss_err * df_col);
                }

                *p_row = FDistribution<T>::RT(*f_row, df_row, *df_err);
                *p_col = FDistribution<T>::RT(*f_col, df_col, *df_err);

                *f_crit_row = FDistribution<T>::InvRT(alpha, df_row, *df_err);
                *f_crit_col = FDistribution<T>::InvRT(alpha, df_col, *df_err);
        }
    };
}