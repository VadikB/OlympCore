



#include "stdafx.h"

#include "TCRNGUtils.h"

#define WEIBULL_METHOD VSL_RNG_METHOD_WEIBULL_ICDF_ACCURATE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_weibull_rand(const T alpha, const T beta, const TCArray* values, fcnType fcn)
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
        int res = fcn(WEIBULL_METHOD, state, count, ptr, alpha, 0, beta);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_weibull_rand(const TCUInt64 seed, const T alpha, const T beta, const TCArray* values, fcnType fcn)
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
        int res = fcn(WEIBULL_METHOD, state, count, ptr, alpha, 0, beta);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}


TCErrorCode stats_weibull_rand_f_e(const TCFloat alpha, const TCFloat beta, TCArray* values)
{
    return stats_weibull_rand<TCFloat, TC_FLOAT>(alpha, beta, values, TC_mkl_vsRngWeibull_sequential);
}

TCErrorCode stats_weibull_rand_d_e(const TCDouble alpha, const TCDouble beta, TCArray* values)
{
    return stats_weibull_rand<TCDouble, TC_DOUBLE>(alpha, beta, values, TC_mkl_vdRngWeibull_sequential);
}

TCErrorCode stats_weibull_rand_seed_f_e(const TCUInt64 seed, const TCFloat alpha, const TCFloat beta, TCArray* values)
{
    return stats_weibull_rand<TCFloat, TC_FLOAT>(seed, alpha, beta, values, TC_mkl_vsRngWeibull_sequential);
}

TCErrorCode stats_weibull_rand_seed_d_e(const TCUInt64 seed, const TCDouble alpha, const TCDouble beta, TCArray* values)
{
    return stats_weibull_rand<TCDouble, TC_DOUBLE>(seed, alpha, beta, values, TC_mkl_vdRngWeibull_sequential);
}