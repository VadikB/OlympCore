//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define BETA_METHOD    VSL_RNG_METHOD_BETA_CJA_ACCURATE

template <typename T, TCTypeTag tag, int arrPos, typename fcnType>
TCErrorCode stats_beta_rand(const T alpha, const T beta, const T A, const T B, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha) || alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (isinf(beta) || isnan(beta) || beta <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(A) || isnan(A))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (isinf(B) || isnan(B))
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }

    if (B <= A)
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }

    TCErrorCode result = stats_rand<T, tag, arrPos>(values, [alpha, beta, A, B, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(BETA_METHOD, state, count, ptr, alpha, beta, A, B);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, int arrPos, typename fcnType>
TCErrorCode stats_beta_rand(const TCUInt64 seed, const T alpha, const T beta, const T A, const T B, const TCArray* values, fcnType fcn)
{
    if (isinf(alpha) || isnan(alpha) || alpha <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(beta) || isnan(beta) || beta <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (isinf(A) || isnan(A))
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }

    if (isinf(B) || isnan(B))
    {
        return tcerror_code_new(TCError_IllegalInput, 5);
    }

    if (B <= A)
    {
        return tcerror_code_new(TCError_IllegalInput, 5);
    }

    TCErrorCode result = stats_rand<T, tag, arrPos>(seed, values, [alpha, beta, A, B, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(BETA_METHOD, state, count, ptr, alpha, beta, A, B);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}


TCErrorCode stats_beta_rand_gen_f_e(TCFloat alpha, TCFloat beta, TCFloat A, TCFloat B, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCFloat, TC_FLOAT, 5>(alpha, beta, A, B, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_beta_rand_gen_d_e(TCDouble alpha, TCDouble beta, TCDouble A, TCDouble B, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCDouble, TC_DOUBLE, 5>(alpha, beta, A, B, randomValues, TC_mkl_vdRngBeta_sequential);
}

TCErrorCode stats_beta_rand_gen_seed_f_e(TCUInt64 seed, TCFloat alpha, TCFloat beta, TCFloat A, TCFloat B, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCFloat, TC_FLOAT, 6>(seed, alpha, beta, A, B, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_beta_rand_gen_seed_d_e(TCUInt64 seed, TCDouble alpha, TCDouble beta, TCDouble A, TCDouble B, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCDouble, TC_DOUBLE, 6>(seed, alpha, beta, A, B, randomValues, TC_mkl_vdRngBeta_sequential);
}


TCErrorCode stats_beta_rand_f_e(TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCFloat, TC_FLOAT, 3>(alpha, beta, 0, 1, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_beta_rand_d_e(TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCDouble, TC_DOUBLE, 3>(alpha, beta, 0, 1, randomValues, TC_mkl_vdRngBeta_sequential);
}

TCErrorCode stats_beta_rand_seed_f_e(TCUInt64 seed, TCFloat alpha, TCFloat beta, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCFloat, TC_FLOAT, 4>(seed, alpha, beta, 0, 1, randomValues, TC_mkl_vsRngBeta_sequential);
}

TCErrorCode stats_beta_rand_seed_d_e(TCUInt64 seed, TCDouble alpha, TCDouble beta, InOut TCArray* randomValues)
{
    return stats_beta_rand<TCDouble, TC_DOUBLE, 4>(seed, alpha, beta, 0, 1, randomValues, TC_mkl_vdRngBeta_sequential);
}
