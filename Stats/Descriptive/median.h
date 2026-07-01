//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCCommon.h"
#include "TCArray.h"
#include "TCMath.h"

#include "qselect.h"
#include "qselect_utils.h"

namespace Descriptive
{

template <typename T>
inline T median(T* data, TCSize_t nelem)
{
    // check for meta-numeric values
    for (TCSize_t i = 0; i < nelem; i++ )
    {
        T v = data[i];
        // don't allow qnans to qsel, missings already filtered in the outer loop
        if(TCError::IsErrorImpl<T>::run(&v))
        {
            return v; // propagate error
        }
    }
    // call different versions of quickselect for odd- and even- length arrays
    if (nelem & 1)
    {
        return quick_select<T, false, TCSize_t, interpolation_t<T,1,2>>(data, nelem, (nelem - 1) / 2);
    }
    else
    {
        return quick_select<T, true, TCSize_t, interpolation_t<T,1,2>>(data, nelem, (nelem - 1) / 2);
    }
}

template <typename T>
struct median_t : public Sort::operator_temp_copy_t<T>
{
    inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
    {
        if (nelem == 0)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
            result = TCError::NewErrorImpl<T>::run(err);
            return err;
        }
        result = median<T>((T*)state.data(), nelem);
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}