



#pragma once

#include <TCCommon.h>
#include <algorithm>
#include "moving_average.h"

namespace Smoothing {
	namespace {

		enum TCEMAArg
		{
			CmaDataSeriesArg = 1,
			CmaWindowLengthArg = 2, 
			CmaExternalCmaValue = 3,
			EmaExternalCmaLength = 4,
			EmaSeriesArg = 5
		};

	}


	template<typename T>
	TCErrorCode stats_smoothing_moving_average_cumulative(
		const TCArray* data_series,
		T external_cma_value,
		TCUInt64 external_cma_length,
		TCArray* cma_series)
	{
		
		TCSize_t n = data_series->m_numelt;
		if (n == 0) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(CmaDataSeriesArg));
		}

		
		TCSize_t v_count;
		TCSize_t data_length;

		if (!dim_analysis(data_series, &data_length, &v_count)) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(CmaDataSeriesArg)); 
		} 

		
		
		ValueType externalValueType = value_type(external_cma_value);

		if (externalValueType == ErrorValue || externalValueType == InfValue)
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(CmaExternalCmaValue));
		}

		
		if (externalValueType == ActualValue && external_cma_length == 0)
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(EmaExternalCmaLength));
		}

		const T* data = (T*)data_series->m_data;
		T* cma_data = (T*) cma_series->m_data;
		
		
		for (TCSize_t v = 0; v < v_count; v++)
		{
			const T* begin = data + v * data_length;
			const T* end = begin + data_length;
			
			T* cma = cma_data + v * data_length;
			T* end_cma = cma + data_length;

			TCDouble acc;
			size_t denominator;
			if (externalValueType == ActualValue)  {
				acc = external_cma_value * external_cma_length;
				denominator = external_cma_length;
			} else {
				acc = 0;
				denominator = 0;
			}

			
			for (auto p = begin; p != end; )
			{
				ValueType type = value_type(*p);
				switch (type) 
				{
				case ActualValue:
					acc += *p;
					++denominator;
					*cma = acc / denominator;
					++cma;
					++p;
					break;
				case MissingValue:
					std::fill(cma, end_cma, _get_tc_error<T>(TCError_IllegalInput));
					p = end;
					break;
				case ErrorValue:
				case InfValue:
					return tcerror_code_new(TCError_IllegalInput, TCArgumentID(CmaDataSeriesArg));
				}
			}
		}
		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	}


}

