//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

namespace MLE
{
    template<typename T> TCErrorCode ExponFit(const TCArray* data, T alpha, T* lambda, T* lower_ci_bound, T* upper_ci_bound)
    {
        if ((data->m_tag != TC_FLOAT) && (data->m_tag != TC_DOUBLE))
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        if ((data == 0) || (lambda == 0) || (lower_ci_bound == 0) || (upper_ci_bound == 0))
        {
            return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
        }

        if ((data->m_ndims != 1) || (data->m_numelt == 0))
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        if ((alpha <= 0) || (alpha >= 1) || (isnan(alpha)))
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        T valuesSum = 0;
        TCSize_t valuesCount = 0;
        T currentValue = 0;

        for(TCSize_t i = 0; i < data->m_numelt; i++)
        {
            currentValue = ((T*)(data->m_data))[i];

            if (currentValue < 0)
            {
                return tcerror_code_new(TCError_IllegalInput, 1);
            }

            // checking inf, nan or error
            if ((isnan(&currentValue)) || (std::numeric_limits<T>::infinity() == currentValue) || (TCError::IsErrorImpl<T>::run(&currentValue)))
            {
                return tcerror_code_new(TCError_IllegalInput, 1);
            }
            else
            {
                // skipping missings
                if (!(TCError::IsMissing<T>(const_cast<T*>(&currentValue))))
                {
                    valuesSum += currentValue;
                    valuesCount++;
                }
            }
        }

        if (valuesSum < 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        *lambda = valuesSum / valuesCount;

        T u = 0;
        T l = 0;

        l = 2 * valuesCount * (*lambda) / Dist::Chisq<T>::InvCDF(1 - alpha / 2, 2 * valuesCount);
        u = 2 * valuesCount * (*lambda) / Dist::Chisq<T>::InvCDF(alpha / 2, 2 * valuesCount);

        u = 1 / u;
        l = 1 / l;
        if (u > l)
        {
            *upper_ci_bound = u;
            *lower_ci_bound = l;
        }
        else
        {
            *upper_ci_bound = l;
            *lower_ci_bound = u;
        }

        *lambda = 1 / (*lambda);

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }
}