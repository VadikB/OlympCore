



#pragma once

#include "TCTypes.h"
#include "TCArrayUtilsInternal.h"
#include "TCErrorCommon.h"
#include "TCErrorImpl.h"
#include <math.h>
#include <vector>
#include <set>
#include <memory>
#include <algorithm>  
#include <iterator>
#include <cmath>
#include "t.h"


using namespace std;

namespace Outliers
{

	template <typename T>
	class ESD
    {
    public:
		static TCErrorCode esd(const TCArray *samples, const TCUInt64 outliers_amount, T alpha, TCArray** indexes,TCArray** statistics_output,TCArray** critical_values_output)
		{
			
			*indexes = NULL;
			*statistics_output = NULL;
			*critical_values_output = NULL;

			if(outliers_amount <= 0)
			{
				return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
			}

			if(samples->m_ndims != 1 ||
				samples->m_numelt <= outliers_amount + 1)
			{
				return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
			}

			if(alpha <= 0.0 || alpha >= 1.0 
				|| Utils::isnan(alpha)
				|| TCError::IsMissingImpl<T>::run(&alpha))
			{
				return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
			}
			
			vector<TCUInt64> outliers_indexes(outliers_amount);
			vector<T> statistics(outliers_amount);
			vector<T> critical_values(outliers_amount);
			set<TCSize_t> skiped_values;

			TCSize_t last_outliers = 0;
			TCSize_t missing_values_count = 0;
			T sum = 0;
			T* data = (T*) samples->m_data;

			
			for(TCSize_t i = 0; i < samples->m_numelt; i++)
			{
				if(Utils::isinf(data[i]) && last_outliers < outliers_amount)
				{
					outliers_indexes[last_outliers] = i;
					statistics[last_outliers] = TCError::NewErrorImpl<T>::run(TCError_IllegalInput);
					critical_values[last_outliers] = TCError::NewErrorImpl<T>::run(TCError_IllegalInput);
					skiped_values.insert(i);
					last_outliers++;
					continue;
				}

				if(TCError::IsMissingImpl<T>::run(&data[i]))
				{
					skiped_values.insert(i);
					missing_values_count++;
					continue;
				}

				if(Utils::isnan(data[i]))
				{	
					return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
				}	

				sum += data[i];
			}
			
			if(samples->m_numelt  <= outliers_amount + missing_values_count + 1)
			{
				return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
			}

			for(TCSize_t i = last_outliers; i < outliers_amount; i++)
			{
				TCUInt64 elements_count = samples->m_numelt - missing_values_count - i;
				critical_values[i] = lambda(alpha,elements_count - 1);
				
				TCUInt64 outlier_index = 0;
				bool is_first = true;
				
				T mean = sum/(elements_count);
				T square_maen = 0.0;
				sum = 0.0;
				
				for(TCSize_t j = 0; j < samples->m_numelt; j++)
				{
					if(skiped_values.count(j) > 0) 
					{
						continue;
					}
					
					square_maen += (data[j] - mean)*(data[j] - mean);
					sum += data[j];

					if(is_first 
						|| fabs(data[j] - mean) > fabs(data[outlier_index] - mean)) 
					{
						is_first = false;
						outlier_index = j;
					}
				}
				
				outliers_indexes[i] = outlier_index;
				statistics[i] = fabs(data[outlier_index] - mean) / sqrt(square_maen / (elements_count - 1));

				if(statistics[i] >= critical_values[i])
				{ 
					last_outliers = i + 1;
				}

				sum -= data[outlier_index];
				skiped_values.insert(outlier_index);
				
			}

			TCErrorCode code = TCError_Internal;
			code = TCArray_New_1D(TC_UINT64, last_outliers, false, indexes);
			if (code != TCError_NoError)
			{
				return code;
			}
			
			code = TCArray_New_1D(samples->m_tag, last_outliers, false, statistics_output);
			if (code != TCError_NoError)
			{
				return code;
			}

			code = TCArray_New_1D(samples->m_tag, last_outliers, false, critical_values_output);
			if (code != TCError_NoError)
			{
				return code;
			}

			for(TCSize_t i = 0; i < last_outliers; i++)
			{
				((TCUInt64*)((*indexes)->m_data))[i] = outliers_indexes[i]+1;
				((T*)((*critical_values_output)->m_data))[i] = critical_values[i];
				((T*)((*statistics_output)->m_data))[i] = statistics[i];
			}

			return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
		}

	private:
		static T lambda(T alpha,int n)
		{
			double t = Dist::StudentT<T>::InvCDF(1.0-1.0*alpha/(2.0*(n+1)),n-1);
			return t*n/sqrt((n-1+t*t)*(n+1));
		}

    };
}