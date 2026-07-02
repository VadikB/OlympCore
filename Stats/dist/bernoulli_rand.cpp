



#include "stdafx.h"

#include "TCRNGUtils.h"
#include "TCArrayUtilsInternal.h"

#define BERNOULLI_METHOD    VSL_RNG_METHOD_BERNOULLI_ICDF

template <typename T,  TCTypeTag tag, typename fcnType>
TCErrorCode stats_bernoulli_rand(const TCDouble p, const TCArray* values, fcnType fcn)
{
    if (isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (p <= 0 || p >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (values == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    const TCInt32 blockSize = 16000;
    TCInt32 size = (TCInt32)std::min<TCSize_t>((TCSize_t)blockSize, values->m_numelt);

    TCInt32 * localArr = new TCInt32[size];
    if (localArr == 0)
    {
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [localArr, size, p, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        while (count > 0)
        {
            TCInt32 sz = std::min<TCInt32>(count, size);
            int res = fcn(BERNOULLI_METHOD, state, sz, localArr, p);
            if (res != VSL_ERROR_OK)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }

            for (TCInt32 i = 0; i < sz; ++i)
            {
                ptr[i] = (TCBool)(localArr[i]);
            }

            count -= sz;
            ptr += sz;
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    delete [] localArr;

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_bernoulli_rand(const TCUInt64 seed, const TCDouble p, const TCArray* values, fcnType fcn)
{
    if (isnan(p))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (p <= 0 || p >= 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (values == 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    const TCInt32 blockSize = 16000;
    TCInt32 size = (TCInt32)std::min<TCSize_t>((TCSize_t)blockSize, values->m_numelt);

    TCInt32 * localArr = new TCInt32[size];
    if (localArr == 0)
    {
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [localArr, size, p, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        while (count > 0)
        {
            TCInt32 sz = std::min<TCInt32>(count, size);
            int res = fcn(BERNOULLI_METHOD, state, sz, localArr, p);
            if (res != VSL_ERROR_OK)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }

            for (TCInt32 i = 0; i < sz; ++i)
            {
                ptr[i] = (TCBool)(localArr[i]);
            }

            count -= sz;
            ptr += sz;
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    delete [] localArr;

    return result;
}

TCErrorCode stats_bernoulli_rand_f_e(const TCFloat probability, TCArray* randomValues)
{
    return stats_bernoulli_rand<TCBool, TC_BOOL>((TCDouble)probability, randomValues, TC_mkl_viRngBernoulli_sequential);
}

TCErrorCode stats_bernoulli_rand_d_e(const TCDouble probability, TCArray* randomValues)
{
    return stats_bernoulli_rand<TCBool, TC_BOOL>(probability, randomValues, TC_mkl_viRngBernoulli_sequential);
}

TCErrorCode stats_bernoulli_rand_seed_f_e(const TCUInt64 seed, const TCFloat probability, TCArray* randomValues)
{
    return stats_bernoulli_rand<TCBool, TC_BOOL>(seed, (TCDouble)probability, randomValues, TC_mkl_viRngBernoulli_sequential);
}

TCErrorCode stats_bernoulli_rand_seed_d_e(const TCUInt64 seed, const TCDouble probability, TCArray* randomValues)
{
    return stats_bernoulli_rand<TCBool, TC_BOOL>(seed, probability, randomValues, TC_mkl_viRngBernoulli_sequential);
}
