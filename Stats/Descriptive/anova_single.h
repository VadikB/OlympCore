//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "OlympCoreUtils.h"
#include "f.h"
#include <new>
#include <algorithm>

using namespace Dist;

namespace Descriptive
{
    template <typename T>
    TCErrorCode stats_infer_anova_single(const TCArray* data, 
    T alpha, 
    TCBool bycol,
    T* f, 
    T* p, 
    T* f_crit, 
    T* ssb, 
    T* ssw, 
    T* sst, 
    TCUInt64* dfb, 
    TCUInt64* dfw, 
    TCUInt64* dft, 
    T* msb, 
    T* msw, 
    TCArray* s_cnt, 
    TCArray* s_sum, 
    TCArray* s_avg, 
    TCArray* s_var)
    {
        try
        {
            return AnovaSingle<T>::ComputeAnovaSingle(data, alpha, bycol, f, p, f_crit, ssb, ssw, sst, dfb, dfw, dft, msb, msw, s_cnt, s_sum, s_avg, s_var);
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


    template <typename T> class AnovaSingleDist;

    template <typename T>
    class AnovaSingle
    {
        friend class AnovaSingleDist<T>;

    private:
        static unsigned const ULPS = 8;

    public:
        static TCErrorCode ComputeAnovaSingle(const TCArray* data, 
        T alpha, 
        TCBool bycol,
        T* f, 
        T* p, 
        T* f_crit, 
        T* ssb, 
        T* ssw, 
        T* sst, 
        TCUInt64* dfb, 
        TCUInt64* dfw, 
        TCUInt64* dft, 
        T* msb, 
        T* msw, 
        TCArray* s_cnt, 
        TCArray* s_sum, 
        TCArray* s_avg, 
        TCArray* s_var)
        {
            if (!(data->m_numelt) 
                || (data->m_ndims != 2) 
                || (data->m_dims[0] < 2 && !bycol) 
                || (data->m_dims[1] < 2 && bycol)) 
            {
                return tcerror_code_new(TCError_IllegalSize, 1);
            }

            T avg;
            T sum;
            TCUInt64 cnt;
            TCSize_t dim = (bycol ? 2 : 1);
            T maxVal = 0;

            ComputeSumCount(data, dim, (TCUInt64*)s_cnt->m_data, (T*)s_sum->m_data, &sum, &cnt, &maxVal);

            avg = sum / cnt;

            *dfb = data->m_dims[dim - 1] - 1;
            *dfw = cnt - data->m_dims[dim - 1];
            *dft = *dfw + *dfb;

            ComputeAvgVarSS(data, dim, (TCUInt64*)s_cnt->m_data, (T*)s_sum->m_data, (T*)s_avg->m_data, (T*)s_var->m_data, avg, ssb, ssw);

            ComputeF(alpha, *ssb, *ssw, *dfb, *dfw, maxVal, f, p, f_crit, sst, dft, msb, msw);
           
            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    private:

        template <typename T>
        static T& GetData(const TCArray* data, TCSize_t index)
        {
            return (static_cast<T*>(data->m_data))[index];
        }

        template <typename T>
        static void ComputeSumCount(const TCArray* data, TCSize_t dim, TCUInt64* s_cnt, T* s_sum, T* sum, TCUInt64* cnt, T* maxVal)
        {
            *sum = (T)0;
            *cnt = 0;

            for (TCSize_t i = 0, index = 0; i < data->m_dims[dim - 1]; ++i, index += data->m_strides[dim - 1])
            {
                TCUInt64 cnt_i = 0;
                T sum_i = (T)0;

                for (TCSize_t j = index, l = 0; l < data->m_dims[dim % 2]; j += data->m_strides[dim % 2], ++l)
                {
                    T x = GetData<T>(data, j);

                    if (!(TCError::IsErrorOrMissingImpl<T>::run(&x)))
                    {
                        ++cnt_i;
                        sum_i += x;

                        ++(*cnt);
                        *sum += x;

                        *maxVal = std::max<T>(*maxVal, fabs(x));
                    }
                    else
                    {
                        if (!data->m_isref)
                        {
                            ++cnt_i;
                            ++(*cnt);
                        }
                    }
                }

                s_cnt[i] = cnt_i;
                s_sum[i] = sum_i;
            }
        }

        template <typename T>
        static void ComputeAvgVarSS(const TCArray* data, const TCSize_t dim, const TCUInt64* s_cnt, const T* s_sum, 
                                    T* s_avg, T* s_var, T avg, T* ssb, T* ssw, bool varDistributed = false)
        {
            *ssb = (T)0;
            *ssw = (T)0;

            for (TCSize_t i = 0, index = 0; i < data->m_dims[dim - 1]; ++i, index += data->m_strides[dim - 1])
            {
                TCUInt64 cnt_i = s_cnt[i];
                T sum_i = s_sum[i];
                T avg_i = sum_i / cnt_i;

                if (cnt_i != 0)
                {
                    T tmp = sum_i - avg * cnt_i;
                    *ssb += (tmp * tmp) / cnt_i;

                    T var = 0;
                    for (TCSize_t j = index, l = 0; l < data->m_dims[dim % 2]; j += data->m_strides[dim % 2], ++l)
                    {
                        T x = GetData<T>(data, j);

                        if (!(TCError::IsErrorOrMissingImpl<T>::run(&x)))
                        {
                            var += (avg_i - x) * (avg_i - x);
                        }
                        else
                        {
                            if (!data->m_isref)
                            {
                                var += (avg_i - 0) * (avg_i - 0);
                            }
                        }
                    }
                    *ssw += var;

                    s_var[i] = ((varDistributed) ? var : var / (cnt_i - 1));
                }
                else
                {
                    s_var[i] = Utils::quiet_NaN<T>();
                }

                s_avg[i] = avg_i;
            }
        }

        template <typename T>
        static void ComputeF(const T alpha, const T ssb, const T ssw, const TCUInt64 dfb, const TCUInt64 dfw, const T maxVal, 
                             T* f, T* p, T* f_crit, T* sst, TCUInt64* dft, T* msb, T* msw)
        {
            T tolerance = ULPS * std::numeric_limits<T>::epsilon() * maxVal;

            *dft = dfw + dfb;

            *sst = ssw + ssb;
            *msb = ssb / dfb;
            *msw = ssw / dfw;

            if (fabs(ssw) < tolerance) //equals to 0
            {
                *f = ((fabs(ssb) < tolerance) ? 0 : Utils::infinity<T>());
            }
            else
            {
                *f = (ssb * dfw) / (ssw * dfb);
            }

            *p = FDistribution<T>::RT(*f, dfb, dfw);

            *f_crit = FDistribution<T>::InvRT(alpha, dfb, dfw);
        }
    };
}
