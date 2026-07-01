//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define GAMMA_METHOD    VSL_RNG_METHOD_GAMMA_GNORM_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_chisq_rand(const T alpha, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [alpha, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(GAMMA_METHOD, state, count, ptr, alpha / 2, 0, 2);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_chisq_rand(const TCUInt64 seed, const T alpha, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [alpha, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(GAMMA_METHOD, state, count, ptr, alpha / 2, 0, 2);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_chisq_rand_f_e(const TCFloat alpha, TCArray* randomValues)
{
    return stats_chisq_rand<TCFloat, TC_FLOAT>(alpha, randomValues, TC_mkl_vsRngGamma_sequential);
}

TCErrorCode stats_chisq_rand_d_e(const TCDouble alpha, TCArray* randomValues)
{
    return stats_chisq_rand<TCDouble, TC_DOUBLE>(alpha, randomValues, TC_mkl_vdRngGamma_sequential);
}

TCErrorCode stats_chisq_rand_seed_f_e(const TCUInt64 seed, const TCFloat alpha, TCArray* randomValues)
{
    return stats_chisq_rand<TCFloat, TC_FLOAT>(seed, alpha, randomValues, TC_mkl_vsRngGamma_sequential);
}

TCErrorCode stats_chisq_rand_seed_d_e(const TCUInt64 seed, const TCDouble alpha, TCArray* randomValues)
{
    return stats_chisq_rand<TCDouble, TC_DOUBLE>(seed, alpha, randomValues, TC_mkl_vdRngGamma_sequential);
}
