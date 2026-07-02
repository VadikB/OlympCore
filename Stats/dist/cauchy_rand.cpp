



#include "stdafx.h"

#include "TCRNGUtils.h"

#define CAUCHY_METHOD VSL_RNG_METHOD_CAUCHY_ICDF

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_cauchy_rand(const T location, const T scale, const TCArray* values, fcnType fcn)
{
    if (isinf(location) || isnan(location))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (isinf(scale) || isnan(scale) || scale <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [location, scale, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(CAUCHY_METHOD, state, count, ptr, location, scale);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_cauchy_rand(const TCUInt64 seed, const T location, const T scale, const TCArray* values, fcnType fcn)
{
    if (isinf(location) || isnan(location))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(scale) || isnan(scale) || scale <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [location, scale, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(CAUCHY_METHOD, state, count, ptr, location, scale);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}


TCErrorCode stats_cauchy_rand_f_e(const TCFloat location, const TCFloat scale, TCArray* values)
{
    return stats_cauchy_rand<TCFloat, TC_FLOAT>(location, scale, values, TC_mkl_vsRngCauchy_sequential);
}

TCErrorCode stats_cauchy_rand_d_e(const TCDouble location, const TCDouble scale, TCArray* values)
{
    return stats_cauchy_rand<TCDouble, TC_DOUBLE>(location, scale, values, TC_mkl_vdRngCauchy_sequential);
}

TCErrorCode stats_cauchy_rand_seed_f_e(const TCUInt64 seed, const TCFloat location, const TCFloat scale, TCArray* values)
{
    return stats_cauchy_rand<TCFloat, TC_FLOAT>(seed, location, scale, values, TC_mkl_vsRngCauchy_sequential);
}

TCErrorCode stats_cauchy_rand_seed_d_e(const TCUInt64 seed, const TCDouble location, const TCDouble scale, TCArray* values)
{
    return stats_cauchy_rand<TCDouble, TC_DOUBLE>(seed, location, scale, values, TC_mkl_vdRngCauchy_sequential);
}