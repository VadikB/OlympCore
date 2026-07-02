



#pragma once

#include "stdafx.h"
#include "TCTypes.h"
#include "TCArray.h"
#include "TCMath.h"

#include "qselect_utils.h"

namespace Descriptive
{

template <typename T>
struct interquartile_dist_t : public Sort::operator_temp_copy_t<T>
{
    inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
    {
        if (nelem == 0)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
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
        if (nelem == 1)
        {
            result = 0;
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        }
        
        T quartile1, quartile3;
        TCSize_t q1_pos = (nelem - 1); 
        TCSize_t q3_pos = 3 * (nelem - 1); 
        switch (q1_pos & 3)
        {
        case 0:
            quartile1 = quick_select<T, false, TCSize_t, interpolation_t<T,1,2>>(data, nelem, q1_pos / 4);
            break;
        case 1:
            quartile1 = quick_select<T, true, TCSize_t, interpolation_t<T,1,4>>(data, nelem, q1_pos / 4);
            break;
        case 2:
            quartile1 = quick_select<T, true, TCSize_t, interpolation_t<T,1,2>>(data, nelem, q1_pos / 4);
            break;
        case 3:
            quartile1 = quick_select<T, true, TCSize_t, interpolation_t<T,3,4>>(data, nelem, q1_pos / 4);
            break;
        }
        switch (q3_pos & 3)
        {
        case 0:
            quartile3 = quick_select<T, false, TCSize_t, interpolation_t<T,1,2>>(data, nelem, q3_pos / 4);
            break;
        case 1:
            quartile3 = quick_select<T, true, TCSize_t, interpolation_t<T,1,4>>(data, nelem, q3_pos / 4);
            break;
        case 2:
            quartile3 = quick_select<T, true, TCSize_t, interpolation_t<T,1,2>>(data, nelem, q3_pos / 4);
            break;
        case 3:
            quartile3 = quick_select<T, true, TCSize_t, interpolation_t<T,3,4>>(data, nelem, q3_pos / 4);
            break;
        }
        result = quartile3 - quartile1;
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}