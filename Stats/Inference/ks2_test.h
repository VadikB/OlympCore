//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include "ks2_dist.h"
#include <utility>
#include "ks_helpers.h"

namespace Inference
{
    template <typename T>
    static TCErrorCode stats_inference_ks2_test(T* data1, TCSize_t n, T* data2, TCSize_t m,
        TCHypothesis test, T* p_value, T* test_statistic)
    {
        TCSize_t rank1 = 0;
        TCSize_t rank2 = 0;
        TCInt64 Dn = 0;
        TCInt64 S = 0;
        TCSSize_t ns = (TCSSize_t)n;
        TCSSize_t ms = (TCSSize_t)m;
        switch (test)
        {
        case TwoTail:
            while (rank1 < n && rank2 < m)
            {
                T x = data1[rank1];
                T y = data2[rank2];

                if (x == y)
                {
                    rank1++;
                    rank2++;
                    S += ms - ns;
                }
                else if (x < y)
                {
                    rank1++;
                    S += ms;
                }
                else
                {
                    rank2++;
                    S -= ns;
                }

                Dn = std::max<TCInt64>(Dn, std::abs(S));
            }
            break;
        case OneTailGT:
            while (rank1 < n && rank2 < m)
            {
                T x = data1[rank1];
                T y = data2[rank2];

                if (x == y)
                {
                    rank1++;
                    rank2++;
                    S += ms - ns;
                }
                else if (x < y)
                {
                    rank1++;
                    S += ms;
                }
                else
                {
                    rank2++;
                    S -= ns;
                }

                Dn = std::max<TCInt64>(Dn, S);
            }
            break;
        case OneTailLT:
            while(rank1 < n && rank2 < m)
            {
                T x = data1[rank1];
                T y = data2[rank2];

                if (x == y)
                {
                    rank1++;
                    rank2++;
                    S += ns - ms;
                }
                else if (x < y)
                {
                    rank1++;
                    S -= ms;
                }
                else
                {
                    rank2++;
                    S += ns;
                }

                Dn = std::max<TCInt64>(Dn, S);
            }
            break;
        default:
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
            break;
        }

        *test_statistic = (T)Dn / (ms * ns);
        *p_value = Dist::KS2Samples<T>::CDF(*test_statistic, n, m, test);

        if (TCError::IsErrorImpl<T>::run(p_value))
        {
            return TCError::GetTCErrorImpl<T>::run(p_value);
        }

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }

    template <typename T>
    static TCErrorCode stats_inference_ks2_test_sequential(TCArray* sample1, TCArray* sample2,
        TCHypothesis test, T* p_value, T* test_statistic)
    {
        TCSize_t n = sample1->m_numelt;
        TCSize_t m = sample2->m_numelt;
        if (n == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
        }
        if (m == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(2));
        }

        if (sample1->m_ndims != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }
        if (sample2->m_ndims != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
        }

        TCErrorCode res;
        TCErrorCode resOK = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);

        math_sort_sequential<T>(sample1, true, 1, sample1);
        math_sort_sequential<T>(sample2, true, 1, sample2);

        T* data1 = (T*)sample1->m_data;
        T* data2 = (T*)sample2->m_data;

        res = skip_missings<T>(data1, n, 1);
        if (res != resOK)
        {
            return res;
        }
        res = skip_missings<T>(data2, m, 2);
        if (res != resOK)
        {
            return res;
        }

        skip_ties<T>(data1, n);
        skip_ties<T>(data2, m);

        return stats_inference_ks2_test<T>(data1, n, data2, m, test, p_value, test_statistic);
    }
}