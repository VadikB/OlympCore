//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define GAMMA_METHOD    VSL_RNG_METHOD_GAMMA_GNORM_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_gamma_rand(const T alpha, const T beta, const TCArray* values, fcnType fcn)
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
        int res = fcn(GAMMA_METHOD, state, count, ptr, alpha, 0, beta);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_gamma_rand(const TCUInt64 seed, const T alpha, const T beta,const TCArray* values, fcnType fcn)
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
        int res = fcn(GAMMA_METHOD, state, count, ptr, alpha, 0, beta);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_gamma_rand_f_e(TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_gamma_rand<TCFloat, TC_FLOAT>(alpha, beta, randomValues, TC_mkl_vsRngGamma_sequential);
}

TCErrorCode stats_gamma_rand_d_e(TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_gamma_rand<TCDouble, TC_DOUBLE>(alpha, beta, randomValues, TC_mkl_vdRngGamma_sequential);
}

TCErrorCode stats_gamma_rand_seed_f_e(TCUInt64 seed, TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_gamma_rand<TCFloat, TC_FLOAT>(seed, alpha, beta, randomValues, TC_mkl_vsRngGamma_sequential);
}

TCErrorCode stats_gamma_rand_seed_d_e(TCUInt64 seed, TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_gamma_rand<TCDouble, TC_DOUBLE>(seed, alpha, beta, randomValues, TC_mkl_vdRngGamma_sequential);
}
