



#include "stdafx.h"

#include "TCRNGUtils.h"

#define GEOMETRIC_METHOD VSL_RNG_METHOD_GEOMETRIC_ICDF

template <typename T, TCTypeTag tag, typename T2, typename fcnType>
TCErrorCode stats_geometric_rand(const T2 probability, TCArray* values, fcnType fcn)
{
    if (isinf(probability) || isnan(probability))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (probability <= 0 || probability >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [probability, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(GEOMETRIC_METHOD, state, count, ptr, probability);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename T2, typename fcnType>
TCErrorCode stats_geometric_rand(const TCUInt64 seed, const T2 probability, TCArray* values, fcnType fcn)
{
    if (isinf(probability) || isnan(probability))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (probability <= 0 || probability >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }


    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [probability, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(GEOMETRIC_METHOD, state, count, ptr, probability);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_geometric_rand_f_e(const TCFloat probability, TCArray* vals)
{
    return stats_geometric_rand<TCInt32, TC_INT32, TCFloat>(probability, vals, TC_mkl_viRngGeometric_sequential);
}

TCErrorCode stats_geometric_rand_d_e(const TCDouble probability, TCArray* vals)
{
    return stats_geometric_rand<TCInt32, TC_INT32, TCDouble>(probability, vals, TC_mkl_viRngGeometric_sequential);
}

TCErrorCode stats_geometric_rand_seed_f_e(const TCUInt64 seed, const TCFloat probability, TCArray* vals)
{
    return stats_geometric_rand<TCInt32, TC_INT32, TCFloat>(seed, probability, vals, TC_mkl_viRngGeometric_sequential);
}

TCErrorCode stats_geometric_rand_seed_d_e(const TCUInt64 seed, const TCDouble probability, TCArray* vals)
{
    return stats_geometric_rand<TCInt32, TC_INT32, TCDouble>(seed, probability, vals, TC_mkl_viRngGeometric_sequential);
}
