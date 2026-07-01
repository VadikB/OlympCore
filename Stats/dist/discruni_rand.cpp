//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"

#define DISCRUNI_METHOD VSL_RNG_METHOD_UNIFORM_STD

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_discruni_rand(const TCInt32 lb, const TCInt32 rb, TCArray* values, fcnType fcn)
{
    if (rb <= lb)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [lb, rb, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(DISCRUNI_METHOD, state, count, ptr, lb, rb);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_discruni_rand(const TCUInt64 seed, const TCInt32 lb, const TCInt32 rb, TCArray* values, fcnType fcn)
{
    if (rb <= lb)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [lb, rb, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(DISCRUNI_METHOD, state, count, ptr, lb, rb);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_discruni_rand_e(const TCInt32 lb, const TCInt32 rb, TCArray* vals)
{
    return stats_discruni_rand<TCInt32, TC_INT32>(lb, rb, vals, TC_mkl_viRngUniform_sequential);
}

TCErrorCode stats_discruni_rand_seed_e(const TCUInt64 seed, const TCInt32 lb, const TCInt32 rb, TCArray* vals)
{
    return stats_discruni_rand<TCInt32, TC_INT32>(seed, lb, rb, vals, TC_mkl_viRngUniform_sequential);
}
