//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"

#define POISSON_METHOD VSL_RNG_METHOD_POISSON_POISNORM

template <typename T, TCTypeTag tag, typename T2, typename fcnType>
TCErrorCode stats_poisson_rand(const T2 mean, TCArray* values, fcnType fcn)
{
    if (isinf(mean) || isnan(mean) || mean <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [mean, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(POISSON_METHOD, state, count, ptr, mean);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename T2, typename fcnType>
TCErrorCode stats_poisson_rand(const TCUInt64 seed, const T2 mean, TCArray* values, fcnType fcn)
{
    if (isinf(mean) || isnan(mean) || mean <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [mean, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(POISSON_METHOD, state, count, ptr, mean);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_poisson_rand_f_e(const TCFloat mean, TCArray* vals)
{
    return stats_poisson_rand<TCInt32, TC_INT32, TCFloat>(mean, vals, TC_mkl_viRngPoisson_sequential);
}

TCErrorCode stats_poisson_rand_d_e(const TCDouble mean, TCArray* vals)
{
    return stats_poisson_rand<TCInt32, TC_INT32, TCDouble>(mean, vals, TC_mkl_viRngPoisson_sequential);
}

TCErrorCode stats_poisson_rand_seed_f_e(const TCUInt64 seed, const TCFloat mean, TCArray* vals)
{
    return stats_poisson_rand<TCInt32, TC_INT32, TCFloat>(seed, mean, vals, TC_mkl_viRngPoisson_sequential);
}

TCErrorCode stats_poisson_rand_seed_d_e(const TCUInt64 seed, const TCDouble mean, TCArray* vals)
{
    return stats_poisson_rand<TCInt32, TC_INT32, TCDouble>(seed, mean, vals, TC_mkl_viRngPoisson_sequential);
}
