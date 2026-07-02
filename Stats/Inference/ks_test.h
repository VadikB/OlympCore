



#pragma once

#include "stdafx.h"
#include "ks_dist.h"
#include <utility>
#include "../dist/norm.h"
#include "ks_helpers.h"

namespace Inference
{
    template <typename T>
    static TCErrorCode stats_inference_ks_test(T* data, TCSize_t n, TCHypothesis test,
        T* p_value, T* statistic)
    {
        TCSize_t rank = 1;
        T Dn = 0.;
        switch(test)
        {
        case TwoTail:
            for (TCSize_t i = 0; i < n; i++, rank++)
            {
                T x = data[i];
                T Nx = Dist::Norm<T>::CDF(x, 0., 1.);
                Dn = std::max<T>(Dn,
                    std::max<T>(((T)rank) / n - Nx, Nx - (T)(rank - 1) / n));
            }
            *statistic = Dn;
            *p_value = Dist::KSTwoSided<T>::CDF(Dn, n);
            break;
        case OneTailGT:
            for (TCSize_t i = 0; i < n; i++, rank++)
            {
                T x = data[i];
                T Nx = Dist::Norm<T>::CDF(x, 0., 1.);
                Dn = std::max<T>(Dn, Nx - (T)(rank - 1) / n);
            }
            *statistic = Dn;
            *p_value = Dist::KSOneSided<T>::CDF(Dn, n);
            break;
        case OneTailLT:
            for (TCSize_t i = 0; i < n; i++, rank++)
            {
                T x = data[i];
                T Nx = Dist::Norm<T>::CDF(x, 0., 1.);
                Dn = std::max<T>(Dn, ((T)rank) / n - Nx);
            }
            *statistic = Dn;
            *p_value = Dist::KSOneSided<T>::CDF(Dn, n);
            break;
        default:
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
            break;
        }

        if (TCError::IsErrorImpl<T>::run(p_value))
        {
            return TCError::GetTCErrorImpl<T>::run(p_value);
        }

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }

    template <typename T>
    static TCErrorCode stats_inference_ks_test_sequential(TCArray* sample, TCHypothesis test,
        T* p_value, T* test_statistic)
    {
        TCSize_t n = sample->m_numelt;
        if (n == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
        }

        if (sample->m_ndims != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }

        TCErrorCode res;
        TCErrorCode resOK = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);

        math_sort_sequential<T>(sample, true, 1, sample);

        T* data = (T*)sample->m_data;

        res = skip_missings<T>(data, n, 1);
        if (res != resOK)
        {
            return res;
        }

        skip_ties<T>(data, n);

        return stats_inference_ks_test<T>(data, n, test, p_value, test_statistic);
    }
}
