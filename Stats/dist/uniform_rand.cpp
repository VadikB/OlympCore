



#include "stdafx.h"

#include "TCRNGUtils.h"

#define UNIFORM_METHOD VSL_RNG_METHOD_UNIFORM_STD_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_uniform_rand(const T lb, const T rb, const TCArray* values, fcnType fcn)
{
    if (isinf(lb) || isnan(lb))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (isinf(rb) || isnan(rb))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (rb <= lb)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [lb, rb, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(UNIFORM_METHOD, state, count, ptr, lb, rb);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_uniform_rand(const TCUInt64 seed, const T lb, const T rb, const TCArray* values, fcnType fcn)
{
    if (isinf(lb) || isnan(lb))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(rb) || isnan(rb))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (rb <= lb)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [lb, rb, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(UNIFORM_METHOD, state, count, ptr, lb, rb);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}


TCErrorCode stats_uniform_rand_f_e(const TCFloat lb, const TCFloat rb, TCArray* values)
{
    return stats_uniform_rand<TCFloat, TC_FLOAT>(lb, rb, values, TC_mkl_vsRngUniform_sequential);
}

TCErrorCode stats_uniform_rand_d_e(const TCDouble lb, const TCDouble rb, TCArray* values)
{
    return stats_uniform_rand<TCDouble, TC_DOUBLE>(lb, rb, values, TC_mkl_vdRngUniform_sequential);
}

TCErrorCode stats_uniform_rand_seed_f_e(const TCUInt64 seed, const TCFloat lb, const TCFloat rb, TCArray* values)
{
    return stats_uniform_rand<TCFloat, TC_FLOAT>(seed, lb, rb, values, TC_mkl_vsRngUniform_sequential);
}

TCErrorCode stats_uniform_rand_seed_d_e(const TCUInt64 seed, const TCDouble lb, const TCDouble rb, TCArray* values)
{
    return stats_uniform_rand<TCDouble, TC_DOUBLE>(seed, lb, rb, values, TC_mkl_vdRngUniform_sequential);
}