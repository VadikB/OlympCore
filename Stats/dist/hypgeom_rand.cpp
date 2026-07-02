



#include "stdafx.h"

#include "TCRNGUtils.h"

#define HYPGEOM_METHOD VSL_RNG_METHOD_HYPERGEOMETRIC_H2PE

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_hypgeom_rand(const TCUInt32 population, const TCUInt32 marked, const TCUInt32 samples, TCArray* values, fcnType fcn)
{
    if (marked > population)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (samples > population)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(values, [population, marked, samples, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(HYPGEOM_METHOD, state, count, ptr, population, samples, marked);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_hypgeom_rand(const TCUInt64 seed, const TCUInt32 population, const TCUInt32 marked, const TCUInt32 samples, TCArray* values, fcnType fcn)
{
    if (marked > population)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (samples > population)
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }

    TCErrorCode result = stats_rand<T, tag, 5>(seed, values, [population, marked, samples, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(HYPGEOM_METHOD, state, count, ptr, population, samples, marked);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_hypgeom_rand_e(const TCUInt32 population, const TCUInt32 marked, const TCUInt32 samples, TCArray* vals)
{
    return stats_hypgeom_rand<TCInt32, TC_INT32>(population, marked, samples, vals, TC_mkl_viRngHypergeometric_sequential);
}

TCErrorCode stats_hypgeom_rand_seed_e(const TCUInt64 seed, const TCUInt32 population, const TCUInt32 marked, const TCUInt32 samples, TCArray* vals)
{
    return stats_hypgeom_rand<TCInt32, TC_INT32>(seed, population, marked, samples, vals, TC_mkl_viRngHypergeometric_sequential);
}
