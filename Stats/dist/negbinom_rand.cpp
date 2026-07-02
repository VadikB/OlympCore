



#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define NEGBINOM_METHOD VSL_RNG_METHOD_NEGBINOMIAL_NBAR

template <typename T,  TCTypeTag tag, typename fcnType>
TCErrorCode stats_negbinom_rand(const TCDouble p, const TCUInt32 number_s, const TCArray* values, fcnType fcn)
{
    if (isinf(p) || isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (p <= 0 || p >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (number_s == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [p, number_s, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(NEGBINOM_METHOD, state, count, ptr, number_s, p);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_negbinom_rand(const TCUInt64 seed, const TCDouble p, const TCUInt32 number_s, const TCArray* values, fcnType fcn)
{
    if (isinf(p) || isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (p <= 0 || p >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (number_s == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [p, number_s, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(NEGBINOM_METHOD, state, count, ptr, number_s, p);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_negbinom_rand_f_e(const TCFloat probability_s, const TCUInt32 number_s, TCArray* values)
{
    return stats_negbinom_rand<TCInt32, TC_UINT32>(probability_s, number_s, values, TC_mkl_viRngNegBinomial_sequential); 
}

TCErrorCode stats_negbinom_rand_d_e(const TCDouble probability_s, const TCUInt32 number_s, TCArray* values)
{
    return stats_negbinom_rand<TCInt32, TC_UINT32>(probability_s, number_s, values, TC_mkl_viRngNegBinomial_sequential); 
}

TCErrorCode stats_negbinom_rand_seed_f_e(const TCUInt64 seed, const TCFloat probability_s, const TCUInt32 number_s, TCArray* values)
{
    return stats_negbinom_rand<TCInt32, TC_UINT32>(seed, probability_s, number_s, values, TC_mkl_viRngNegBinomial_sequential); 
}

TCErrorCode stats_negbinom_rand_seed_d_e(const TCUInt64 seed, const TCDouble probability_s, const TCUInt32 number_s, TCArray* values)
{
    return stats_negbinom_rand<TCInt32, TC_UINT32>(seed, probability_s, number_s, values, TC_mkl_viRngNegBinomial_sequential); 
}