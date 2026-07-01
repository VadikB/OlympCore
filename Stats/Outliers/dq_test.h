//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include "dq_test_method.h"

#include <stdlib.h>		
#include <vector>		
#include <algorithm>	
#include <OlympCoreUtils.h>
#include <TCTypes.h> 

#include "TCErrorImpl.h"
#include "TCErrorCommon.h"
using std::max;
using std::partial_sort_copy;
using std::vector;

namespace Outliers
{
	template<typename T>
	TCErrorCode stats_outliers_test_dq(const TCArray* sample, TCUInt32 method,
		TCHypothesis tail, T* p_value, T* test_statistics, TCUInt64* outlier_index)
	{

		TCSize_t sample_size = sample->m_numelt;

		//Input data size is less than method minimum requirement 	
		if ( sample_size < 3 )											
		{			
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
		}

		// method is not in {0,11,12,20,21,22}
		if ( method == 0 )
		{
			if ( sample_size < 8 )
			{
				method = 10;
			}
			else if ( sample_size < 11 )
			{
				method = 11; 
			}
			else if ( sample_size < 14 )
			{
				method = 21;
			}
			else
			{
				method = 22;
			}
		}
		else if (!(( method > 9 && method < 13 ) || ( method > 19 && method < 23 )))		
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
		}
		
		div_t divresult = div(method, 10);
		TCSize_t nextrems_diam = divresult.rem; 
		TCSize_t nextrems_tail = divresult.quot; 
		
		if ( nextrems_tail + nextrems_diam + 2 > sample_size ) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
		}

		T* data = (T*)sample->m_data; 
	
		vector<TCSize_t> indexVector(sample_size);
		for (TCSize_t i = 0; i < sample_size; i++)
		{
			indexVector.at(i) = i;
		}
	
		TCSize_t valid_data_count = 0;

		CheckingComparator<T, TCSize_t> checkingComp(data); 

		try
		{
			sort(indexVector.begin(), indexVector.end(), checkingComp);
		}
		catch (...)
		{	//Error NAN, ±Inf in the sample TCError_IllegalInput 			
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));	
		}

		ValidDataWrapper<T,TCSize_t>  validData(data, &indexVector.front(), sample_size); 
		valid_data_count = validData.getValidDataCount();
		
		//Input data size after duplicates removed 
		//is less than method minimum requirement TCError_IllegalSize 
		if ( nextrems_tail + nextrems_diam + 2 > valid_data_count ) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
		}

		bool isOneTailGT = true;

		T sample_tail;
		T sample_diam;
		
		switch (tail)
		{
			case OneTailGT:		
				sample_tail = validData.Maxs[0] - validData.Maxs[nextrems_tail];
				sample_diam = validData.Maxs[0] - validData.Mins[nextrems_diam];				
				isOneTailGT = true;
				break;
			case OneTailLT:	
				sample_tail = validData.Mins[nextrems_tail] - validData.Mins[0];
				sample_diam = validData.Maxs[nextrems_diam] - validData.Mins[0];			
				isOneTailGT = false;
				break;
			case TwoTail:
				{
				sample_tail = validData.Maxs[0] - validData.Maxs[nextrems_tail];
				sample_diam = validData.Maxs[0] - validData.Mins[nextrems_diam];

				T sample_tail_alt = validData.Mins[nextrems_tail] - validData.Mins[0];
				T sample_diam_alt = validData.Maxs[nextrems_diam] - validData.Mins[0];

					if ( sample_tail * sample_diam_alt < sample_tail_alt * sample_diam )
					{
						sample_tail = sample_tail_alt;
						sample_diam = sample_diam_alt;
						isOneTailGT = false;
					}
				}
				break;
			default:				
				return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));	
		}

		if ( fabs(sample_diam) <= std::numeric_limits<T>::min() ) 
		{
			return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
		}
		
		T r = sample_tail/sample_diam;
		*test_statistics = r;
		*outlier_index =static_cast<TCUInt64>(1 + (isOneTailGT ? indexVector[valid_data_count - 1] : indexVector[0]));

		if ( valid_data_count > 30 )
		{												
			*p_value = _get_tc_error<T>(TCError_IllegalInput);
		}
		else 
		{
			double r_appr = static_cast<double>(r);
			double p_appr = p_value_approximation(valid_data_count, nextrems_tail, nextrems_diam, r_appr);		
			*p_value = static_cast<T>(p_appr);
		}
	
		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	};
}

