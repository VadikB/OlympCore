



#pragma once

#include "TCTypes.h"

#include "t.h"
#include "chisq.h"

namespace MLE
{
	template <typename T, TCTypeTag Tag>
    TCErrorCode norm_fit(const TCArray* data, T alpha,	T* mean, T* stdev, T* lb_ci_mean, T* ub_ci_mean, T* lb_ci_stdev, T* ub_ci_stdev)
    {
		
		if(data == NULL) 
		{
			return tcerror_code_new(TCError_IllegalInput, 1);
		}

		
		if((mean == NULL) || (stdev == NULL) || (lb_ci_mean == NULL) || (ub_ci_mean == NULL) || (lb_ci_stdev == NULL) || (ub_ci_stdev == NULL))
		{
			return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
		}

		
		if((data->m_tag != Tag))
		{
			return tcerror_code_new(TCError_IllegalInput, 1);
		}

		
		if(data->m_ndims != 1)
		{
			return tcerror_code_new(TCError_IllegalSize, 1);
		}

		
		if(data->m_numelt < 2)
		{
			return tcerror_code_new(TCError_IllegalSize, 1);
		}

		T* m_data = static_cast<T*>(data->m_data);
		T sum = 0;
		TCSize_t nelem = 0;
		T p_inf_val = std::numeric_limits<T>::infinity();
		T n_inf_val = -p_inf_val;

		for(TCSize_t i = 0; i < data->m_numelt; i++)
		{
			
			if((TCError::IsErrorImpl<T>::run(m_data + i)) || (m_data[i] == p_inf_val) || (m_data[i] == n_inf_val))
			{
				return tcerror_code_new(TCError_IllegalInput, 1);
			}

			if(!TCError::IsMissing<T>(m_data + i))
			{
				sum += m_data[i];
				nelem++;
			}
		}

		
		T m_mean = sum / nelem;
		
		sum = 0;
		if(nelem == data->m_numelt)
		{
			for(TCSize_t i = 0; i < nelem; i++)
			{
				sum += pow(m_data[i] - m_mean, 2);
			}
		}
		else
		{
			for(TCSize_t i = 0; i < data->m_numelt; i++)
			{
				if(!TCError::IsMissing<T>(m_data + i))
				{
					sum += pow(m_data[i] - m_mean, 2);
				}
			}
		}

		
		T usigma = Utils::sqrt(sum / (nelem - 1));
		
		T bsigma = Utils::sqrt(sum / nelem);
		
		T sigma = (usigma < bsigma)? usigma: bsigma;

		*mean = m_mean;
		*stdev = usigma;

		
		T t = Dist::StudentT<T>::InvCDF(1-alpha/2, nelem - 1);
		T eps = t * sigma / Utils::sqrt((T)(nelem - 1));
		*lb_ci_mean = m_mean - eps;
		*ub_ci_mean = m_mean + eps;

		
		*lb_ci_stdev = usigma * Utils::sqrt((T)(nelem - 1) / Dist::Chisq<T>::InvCDF(1 - alpha / 2, nelem - 1));
		*ub_ci_stdev = usigma * Utils::sqrt((T)(nelem - 1) / Dist::Chisq<T>::InvCDF(alpha / 2, nelem - 1));

		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	}
}