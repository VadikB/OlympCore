



#include "stdafx.h"

#include "f.h"
#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define F_BETA_METHOD    VSL_RNG_METHOD_BETA_CJA_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_f_rand(const T alpha, const T beta, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha) || alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (isinf(beta) || isnan(beta) || beta <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [alpha, beta, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(F_BETA_METHOD, state, count, ptr, alpha * (T)0.5, beta * (T)0.5, (T)0, (T)1);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    if (tcerror_code_get_errorid(result) != TCError_NoError)
    {
        return result;
    }

    T* dataPtr = (T*)(values->m_data);
    for (TCSize_t i = 0; i < values->m_numelt; ++i)
    {
        dataPtr[i] = (beta * dataPtr[i]) / (alpha * ((T)1 - dataPtr[i]));
    }

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_f_rand(const TCUInt64 seed, const T alpha, const T beta, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha) || alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(beta) || isnan(beta) || beta <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [alpha, beta, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(F_BETA_METHOD, state, count, ptr, alpha * (T)0.5, beta * (T)0.5, (T)0, (T)1);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    if (tcerror_code_get_errorid(result) != TCError_NoError)
    {
        return result;
    }

    T* dataPtr = (T*)(values->m_data);
    for (TCSize_t i = 0; i < values->m_numelt; ++i)
    {
        dataPtr[i] = dataPtr[i] = (beta * dataPtr[i]) / (alpha * ((T)1 - dataPtr[i]));
    }

    return result;
}

TCErrorCode stats_f_rand_f_e(TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_f_rand<TCFloat, TC_FLOAT>(alpha, beta, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_f_rand_d_e(TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_f_rand<TCDouble, TC_DOUBLE>(alpha, beta, randomValues, TC_mkl_vdRngBeta_sequential);
}

TCErrorCode stats_f_rand_seed_f_e(TCUInt64 seed, TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_f_rand<TCFloat, TC_FLOAT>(seed, alpha, beta, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_f_rand_seed_d_e(TCUInt64 seed, TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_f_rand<TCDouble, TC_DOUBLE>(seed, alpha, beta, randomValues, TC_mkl_vdRngBeta_sequential);
}
