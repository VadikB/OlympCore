



#pragma once

namespace MLE
{
    template<typename T, TCTypeTag typeTag> TCErrorCode PoissonFit(const TCArray* data, T alpha, T* lambda, T* lower_ci_bound, T* upper_ci_bound)
    {
        if (data->m_tag != TC_UINT64)
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

        TCUInt64 valuesSum = 0;
        TCSize_t valuesCount = 0;

        for(TCSize_t i = 0; i < data->m_numelt; i++)
        {
            
            valuesSum += ((TCUInt64*)(data->m_data))[i];
            valuesCount++;
        }

        if (valuesSum == 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        *lambda = (T)valuesSum / (T)valuesCount;
        *lower_ci_bound = Dist::Chisq<T>::InvCDF(alpha / 2, 2 * valuesSum) / (2 * valuesCount);
        *upper_ci_bound = Dist::Chisq<T>::InvCDF(1 - alpha / 2, 2 * valuesSum + 2) / (2 * valuesCount);

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }
}