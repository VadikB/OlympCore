//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCCommon.h"
#include "TCArray.h"
#include "TCMath.h"

namespace Descriptive
{

template <typename T>
struct percentrank_exc_t
{
    T value;
    TCSize_t significance;
    percentrank_exc_t(T value, TCSize_t significance)
    {
        this->value = value;
        this->significance = significance;
    }

    typedef T    in_t;
    typedef T    ou_t;

    typedef struct
    {
        TCSize_t below_count;   // count(y): y < x
        TCSize_t above_count;   // count(y): y > x
        T below_value;          // max(y): y < x
        T above_value;          // min(y): y >= x
        T error;
    }
    T_state;

    inline TCErrorID InitState(T_state& state, TCSize_t nelem)
    {
        state; // unreferenced parameter
        nelem; // unreferenced parameter
        return TCError_NoError;
    }
    inline void ProcessFirst(T_state& state, T& x)
    {
        state.below_count = 0;
        state.above_count = 0;
        state.below_value = TCError::quiet_NaN<T>();
        state.above_value = TCError::quiet_NaN<T>();
        state.error = 0;
        Process(state, x, 0);
    }
    inline void Process(T_state& state, T& x, TCSize_t index)
    {
        index; // unreferenced parameter
        if (!(state.error == 0))
        {
            return;
        }
        if (TCError::IsErrorImpl<T>::run(&x))
        {
            state.error = x;
        }
        if (x < value)
        {
            state.below_count++;
            if (TCError::is_qnan(state.below_value) || x > state.below_value)
            {
                state.below_value = x;
            }
        }
        else
        {
            if (x > value)
            {
                state.above_count++;
            }
            if (TCError::is_qnan(state.above_value) || x < state.above_value)
            {
                state.above_value = x;
            }
        }
    }

    inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
    {
        if (nelem == 0)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
            result = TCError::NewErrorImpl<T>::run(err);
            return err;
        }
        if (state.below_count == nelem || state.above_count == nelem)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
            result = TCError::NewErrorImpl<T>::run(err);
            return err;
        }
        if (state.error != 0)
        {
            result = state.error;
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        }
        T d = 1; // 1...nelem
        if (state.below_count > 0)
        {
            if (value == state.above_value)
            {
                d = (T)(state.below_count + 1);
            }
            else
            {
                d = (T)state.below_count + (value - state.below_value) / (state.above_value - state.below_value);
            }
        }
        T ret_value = d / (T)(nelem + 1);
        if (significance > 0)
        {
            ret_value = floor(ret_value*pow((T)10, (int)significance)) * pow((T)0.1, (int)significance);
        }
        result = ret_value;
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}
