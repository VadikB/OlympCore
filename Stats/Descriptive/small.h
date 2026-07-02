



#pragma once

#include "stdafx.h"
#include "TCTypes.h"
#include "TCArray.h"
#include "TCMath.h"

#include "qselect_utils.h"

namespace Descriptive
{

template <typename T>
struct small_t : public Sort::operator_temp_copy_t<T>
{
    TCSize_t k;
    small_t(TCSize_t k)
    {
        this->k = k;
    }
    inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
    {
        if (k <= 0 || k > nelem)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
            result = TCError::NewErrorImpl<T>::run(err);
            return err;
        }
        
        T* data = state.data();
        for (TCSize_t i = 0; i < nelem; i++ )
        {
            T v = data[i];
            
            if(TCError::IsErrorImpl<T>::run(&v))
            {
                result = v; 
                return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
            }
        }
        result = quick_select<T, false, TCSize_t, interpolation_t<T,1,2>>(data, nelem, k - 1);
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}
