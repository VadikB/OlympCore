//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <TCCommon.h>
#include <algorithm>
#include "moving_average.h"

namespace Smoothing {

	namespace {

		enum TCEMAArg
		{
			EmaDataSeriesArg = 1,
			EmaWindowLengthArg = 2, 
			EmaTypeArg = 3,
			EmaExternalLastvalueArg = 4,
			EmaSeriesArg = 5
		};

	}

	template <typename T>
	TCErrorCode stats_smoothing_moving_average_exponential(
		const TCArray* data_series,
		TCUInt64 window_length,
		TCEMAType ema_type,
		T external_ema_last_value,
		TCArray * ema_series)
	{
		if (window_length <= 1)
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(EmaWindowLengthArg));
		}

		ValueType externalValueType = value_type(external_ema_last_value);

		if (externalValueType == ErrorValue || externalValueType == InfValue)
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(EmaExternalLastvalueArg));
		}

		TCSize_t n = data_series->m_numelt;
		if (n == 0) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(EmaDataSeriesArg));
		}

		TCSize_t v_count;
		TCSize_t data_length;

		if (!dim_analysis(data_series, &data_length, &v_count)) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(EmaDataSeriesArg)); //Error when shape is illegal
		} 

		T alpha, beta;
		switch (ema_type)
		{
		case simple:
			alpha = 2 / T(1 + window_length);
			break;
		case modified:
			alpha = 1 / T(window_length);
			break;
		default:
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(EmaTypeArg));
		}
		beta = 1 - alpha;


		const T* data = (T*)data_series->m_data;
		T* ema_data = (T*) ema_series->m_data;
		
		//cycle through columns
		for (TCSize_t v = 0; v < v_count; v++)
		{
			const T* begin = data + v * data_length;
			const T* end = begin + data_length;
			
			T* begin_ema = ema_data + v * data_length;
			T* end_ema = begin_ema + data_length;
			T* ema = begin_ema;

			//If EMAprev is not defined then EMA1=X1, else EMA1=EMAprev
			*ema =  externalValueType == MissingValue ? *begin : external_ema_last_value;

			//process data in a single column
			for (auto p = begin + 1; p != end; )
			{
				ValueType type = value_type(*p);
				T next_value;
				switch (type) 
				{
				case ActualValue:
					next_value = alpha * *p + beta * *ema;
					++p;
					*++ema = next_value;
					break;
				case MissingValue:
					//All EMA results become NaN
					std::fill(begin_ema, end_ema, _get_tc_error<T>(TCError_IllegalInput));
					p = end;
					break;
				case ErrorValue:
				case InfValue:
					return tcerror_code_new(TCError_IllegalInput, TCArgumentID(EmaDataSeriesArg));
				}
			}
		}
		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	}


}