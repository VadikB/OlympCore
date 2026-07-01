//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define BINOM_METHOD VSL_RNG_METHOD_BINOMIAL_BTPE

template <typename T,  TCTypeTag tag, typename fcnType>
TCErrorCode stats_binom_rand(const TCDouble p, const TCUInt64 trials, const TCArray* values, fcnType fcn)
{
    if (isinf(p) || isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (p < 0 || p > 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (trials == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [p, trials, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(BINOM_METHOD, state, count, ptr, trials, p);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_binom_rand(const TCUInt64 seed, const TCDouble p, const TCUInt64 trials, const TCArray* values, fcnType fcn)
{
    if (isinf(p) || isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (p < 0 || p > 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (trials == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [p, trials, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(BINOM_METHOD, state, count, ptr, trials, p);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_binom_rand_f_e(const TCFloat probability, const TCUInt64 trials, TCArray* values)
{
    return stats_binom_rand<TCInt32, TC_INT32>(probability, trials, values, TC_mkl_viRngBinomial_sequential); 
}

TCErrorCode stats_binom_rand_d_e(const TCDouble probability, const TCUInt64 trials, TCArray* values)
{
    return stats_binom_rand<TCInt32, TC_INT32>(probability, trials, values, TC_mkl_viRngBinomial_sequential); 
}

TCErrorCode stats_binom_rand_seed_f_e(const TCUInt64 seed, const TCFloat probability, const TCUInt64 trials, TCArray* values)
{
    return stats_binom_rand<TCInt32, TC_INT32>(seed, probability, trials, values, TC_mkl_viRngBinomial_sequential); 
}

TCErrorCode stats_binom_rand_seed_d_e(const TCUInt64 seed, const TCDouble probability, const TCUInt64 trials, TCArray* values)
{
    return stats_binom_rand<TCInt32, TC_INT32>(seed, probability, trials, values, TC_mkl_viRngBinomial_sequential); 
}