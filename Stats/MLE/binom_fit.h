//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCConsts.h"

#include "f.h"

namespace MLE
{
	template <typename T>
    TCErrorCode binom_fit(const TCArray* data, T alpha, T* p, T* lb_ci_p, T* ub_ci_p)
    {
		//Input array is not initialized
		if(data == NULL)
		{
			return tcerror_code_new(TCError_IllegalInput, 1);
		}

		//Type of input array is not correct
		if((data->m_tag != TC_BOOL))
		{
			return tcerror_code_new(TCError_IllegalInput, 1);
		}

		//Input array is not 1-D array
		if(data->m_ndims != 1)
		{
			return tcerror_code_new(TCError_IllegalSize, 1);
		}

		//Input array is empty or length one
		if(data->m_numelt == 0)
		{
			return tcerror_code_new(TCError_IllegalSize, 1);
		}

		TCBool* m_data = static_cast<TCBool*>(data->m_data);
		TCUInt64 trails = static_cast<TCUInt64>(data->m_numelt);
		TCUInt64 number_s = 0;
		
		//Computation of a number of successes and size of a sequence of independent yes/no experiments is given data.
		for(TCUInt64 i = 0; i < trails; i++)
		{
			if(m_data[i] == TRUE)
			{
				number_s++;
			}
		}

		return binom_fit<T>(number_s, trails, alpha, p, lb_ci_p, ub_ci_p, FALSE);
	}

	template <typename T>
    TCErrorCode binom_fit(TCUInt64 number_s, TCUInt64 trials, T alpha, T* p, T* lb_ci_p, T* ub_ci_p, TCBool primary_call = TRUE)
    {
		//Output parameters is not initialized
		if((p == NULL) || (lb_ci_p == NULL) || (ub_ci_p == NULL))
		{
			return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
		}

		//Input parameter trials is not positive
		if(trials == 0)
		{
			return primary_call? tcerror_code_new(TCError_IllegalInput, 2) : tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
		}

		//Input parameter number_s is negative or more than trials
		if(number_s > trials)
		{
			return primary_call? tcerror_code_new(TCError_IllegalInput, 1) : tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
		}

		//Estimate of the probability of the binomial distribution
		*p = (T)number_s / trials;

		//Confidence interval for the probability is calculated using the Clopper-Pearson method
		T v1, v2, f;

		if(number_s == 0)
		{
			*lb_ci_p = 0.f;
		}
		else
		{
			v1 = 2 * number_s;
			v2 = 2 * (trials - number_s + 1);
			f = Dist::FDistribution<T>::InvCDF(alpha/2, v1, v2); 

			*lb_ci_p = v1 * f / (v2 + v1 * f);
		}

		if(number_s == trials)
		{
			*ub_ci_p = 1.f;
		}
		else
		{
			v1 = 2 * (number_s + 1);
			v2 = 2 * (trials - number_s);
			f = Dist::FDistribution<T>::InvCDF(1 - alpha/2, v1, v2); 

			*ub_ci_p = v1 * f / (v2 + v1 * f);
		}

		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	}
}//MLE