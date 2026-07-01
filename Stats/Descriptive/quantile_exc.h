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
struct quantile_exc_t : public Sort::operator_temp_copy_t<T>
{
    TCSize_t q;
    quantile_exc_t(TCSize_t q)
    {
        this->q = q;
    }
    inline TCErrorCode GetResult(T_state &state, TCSize_t nelem, T *output, TCSize_t output_stride)
    {
        if (nelem == 0)
        {
            TCErrorCode err = tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
            T result = TCError::NewErrorImpl<T>::run(err);
            for (TCSize_t j = 1; j < q; j++)
            {
                output[(j-1)*output_stride] = result;
            }
            return err;
        }
        T* data = state.data();
        // check for meta-numeric values
        for (TCSize_t i = 0; i < nelem; i++ )
        {
            T v = data[i];
            // don't allow qnans to qsel, missings already filtered in the outer loop
            if(TCError::IsErrorImpl<T>::run(&v))
            {
                for (TCSize_t j = 1; j < q; j++)
                {
                    output[(j-1)*output_stride] = v; // propagate error
                }
                return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
            }
        }

		TCSize_t p_entier = (nelem + 1) / q;
		TCSize_t p_excess = (nelem + 1) % q;

        for (TCSize_t i = 1; i < q; i++)
        {
            T v;
			TCSize_t nexcess = p_excess * i;
			TCSize_t m = p_entier * i + nexcess / q;
			TCSize_t excess = nexcess % q;
			T d = (T)excess / q;

            if (m == 0 || m > nelem)
            {
                v = TCError::quiet_NaN<T>();
            }
            else
            {
                if (nelem == 1)
                {
                    v = data[0];
                }
                else
                {
                    if (m == nelem)
                    {
                        v = quick_select<T, false, TCSize_t, interpolation_t<T,1,2>>(data, nelem, m - 1);
                    }
                    else
                    {
                        interpolation_with_d_t<T> mean(d);
                        v = quick_select<T, true, TCSize_t>(data, nelem, m - 1, mean);
                    }
                }
            }
            output[(i-1)*output_stride] = v;
        }
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};

}