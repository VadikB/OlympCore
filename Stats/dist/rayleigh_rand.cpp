



#include "stdafx.h"

#include "TCRNGUtils.h"
#include <TCConsts.h>

#define RAYLEIGH_METHOD VSL_RNG_METHOD_RAYLEIGH_ICDF_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_rayleigh_rand(const T scale, const TCArray* values, fcnType fcn)
{
    if (isinf(scale) || isnan(scale) || scale <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [scale, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(RAYLEIGH_METHOD, state, count, ptr, 0, scale * SQRT_TWO);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_rayleigh_rand(const TCUInt64 seed, const T scale, const TCArray* values, fcnType fcn)
{
    if (isinf(scale) || isnan(scale) || scale <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [scale, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(RAYLEIGH_METHOD, state, count, ptr, 0, scale * SQRT_TWO);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}


TCErrorCode stats_rayleigh_rand_f_e(const TCFloat scale, TCArray* values)
{
    return stats_rayleigh_rand<TCFloat, TC_FLOAT>(scale, values, TC_mkl_vsRngRayleigh_sequential);
}

TCErrorCode stats_rayleigh_rand_d_e(const TCDouble scale, TCArray* values)
{
    return stats_rayleigh_rand<TCDouble, TC_DOUBLE>(scale, values, TC_mkl_vdRngRayleigh_sequential);
}

TCErrorCode stats_rayleigh_rand_seed_f_e(const TCUInt64 seed, const TCFloat scale, TCArray* values)
{
    return stats_rayleigh_rand<TCFloat, TC_FLOAT>(seed, scale, values, TC_mkl_vsRngRayleigh_sequential);
}

TCErrorCode stats_rayleigh_rand_seed_d_e(const TCUInt64 seed, const TCDouble scale, TCArray* values)
{
    return stats_rayleigh_rand<TCDouble, TC_DOUBLE>(seed, scale, values, TC_mkl_vdRngRayleigh_sequential);
}