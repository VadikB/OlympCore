



#pragma once

#include "OlympCore.h"
#include <time.h>
#include <algorithm>

#define DEFINE_MKL_VSL
#include <TC_mkl.h>

class RNGStateManager
{
private:
    static TCUInt64 seed;
    static TCRNGStatePtr stream;
    static const int brng = VSL_BRNG_SFMT19937;
    RNGStateManager() {};

    static TCRNGStatePtr CreateState()
    {
        int res = TC_mkl_vslNewStream(&stream, brng, seed);

        if (res != VSL_ERROR_OK)
        {
            stream = NULL;
        }

        return stream;
    }

    static void DeleteState()
    {
        DeleteState(&stream);
    }

    static void DeleteState(TCRNGStatePtr* state)
    {
        if (*state != NULL)
        {
            int res = TC_mkl_vslDeleteStream(state);

            if (res != VSL_ERROR_OK)
            {
                *state = NULL;
            }
        }
    }

public:
    static TCRNGStatePtr GetState()
    {
        if (stream == NULL) 
        {
            stream = CreateState();
        }

        return stream;
    }

    static void ResetState()
    {
        ResetState(seed);
    }
    
    static TCUInt64 GetSeed()
    {
        return seed;
    }

    static void ResetState(TCUInt64 init_seed)
    {
        DeleteState();

        seed = init_seed;
    }

    static void SetState(TCRNGStatePtr state)
    {
        DeleteState();

        TC_mkl_vslCopyStream(&stream, state);
    }

    static void CopyState(TCRNGStatePtr* state)
    {
        TC_mkl_vslCopyStream(state, stream);
    }

    static int GetBRNG()
    {
        return brng;
    }
};


template <typename T, TCTypeTag tag, int array_arg_pos, class Function>
TCErrorCode stats_rand(const TCArray* values, Function&& gen_fn)
{
    if (values == NULL)
    {
        return tcerror_code_new(TCError_IllegalInput, array_arg_pos);
    }

    TCBool isempty;
    array_isempty(values, &isempty);
    if (isempty)
    {
        return tcerror_code_new(TCError_IllegalSize, array_arg_pos);
    }

    TCTypeTag arrTag;
    array_type(values, &arrTag);
    if (arrTag != tag)
    {
        return tcerror_code_new(TCError_IllegalInput, array_arg_pos);
    }

    TCRNGStatePtr state = RNGStateManager::GetState();

    if (state == NULL)
    {
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }

    TCSize_t count = values->m_numelt;
    T* ptr = (T*)values->m_data;
    while(count > 0)
    {
        TCSize_t sz = std::min<TCSize_t>(count, std::numeric_limits<int>::max());
        TCErrorCode res = gen_fn(state, (int)sz, ptr);

        if (tcerror_code_get_errorid(res) != TCError_NoError)
        {
            return res;
        }

        count -= sz;
        ptr += sz;
    }

    return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
}

template <typename T, TCTypeTag tag, int array_arg_pos, class Function>
TCErrorCode stats_rand(const TCUInt64 seed, const TCArray* values, Function&& gen_fn)
{
    if (values == NULL)
    {
        return tcerror_code_new(TCError_IllegalInput, array_arg_pos);
    }

    TCBool isempty;
    array_isempty(values, &isempty);
    if (isempty)
    {
        return tcerror_code_new(TCError_IllegalSize, array_arg_pos);
    }

    TCTypeTag arrTag;
    array_type(values, &arrTag);
    if (arrTag != tag)
    {
        return tcerror_code_new(TCError_IllegalInput, array_arg_pos);
    }

    TCRNGStatePtr state;

    int brng = RNGStateManager::GetBRNG();

    int res = TC_mkl_vslNewStream(&state, brng, seed);
    if (res != VSL_ERROR_OK)
    {
        if (res == VSL_ERROR_MEM_FAILURE)
        {
            return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
        }

        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }

    TCErrorCode result = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);

    TCSize_t count = values->m_numelt;
    T* ptr = (T*)values->m_data;
    while(count > 0)
    {
        TCSize_t sz = std::min<TCSize_t>(count, std::numeric_limits<int>::max());
        TCErrorCode ec = gen_fn(state, (int)sz, ptr);

        if (tcerror_code_get_errorid(ec) != TCError_NoError)
        {
            result = ec;
            break;
        }

        count -= sz;
        ptr += sz;
    }

    res = TC_mkl_vslDeleteStream(&state);
    if (res != VSL_ERROR_OK)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }

    return result;
}
