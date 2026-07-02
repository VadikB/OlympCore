



#pragma once

#include "stdafx.h"
#include "moving_average.h"

namespace Smoothing {
	enum TCTMAArg
	{
		DataSeriesArg = 1,
		WindowLengthArg = 2, 
		OffsetArg = 3, 
		ExternalDataLagArg = 4,
		ExternalDataLeadArg = 5,
		SmaSeriesArg = 6
	};

	TCUInt64 get_weight(TCUInt64 k, TCUInt64 window_length, TCUInt64 offset, bool two_peaks)
	{
		if (two_peaks)
		{
			if (k < window_length - offset - 1)
			{
				return k + 1;
			}
			return window_length - k;
		}

		if (k < window_length - offset)
		{
			return (offset + 1) * (k + 1);
		}
		return (window_length - k) * (window_length - offset);
	}

	template <typename T>
	TCBool moving_average_single_part(T* x, TCUInt64 l, TCUInt64 r, TCInt64 w_offset, 
		TCUInt64 window_length, TCUInt64 offset, bool two_peaks, TCUInt64* weights_sum, T* s) 
	{
		for (TCUInt64 i = l; i <= r; i++) {
			if (TCError::IsMissingImpl<T>::run(&x[i])) 
			{
				*s = _get_tc_error<T>(TCError_IllegalInput); 
				return 0;
			}

			TCUInt64 weight = get_weight(w_offset + i - l, window_length, offset, two_peaks);
			*s += x[i] * weight;
			*weights_sum += weight;
		}
		return 1;
	}

	template <typename T>
	void check_mis_inf_nan(T* x, TCSize_t len, TCBool* is_inf_nan, TCBool* is_missing)
	{
		*is_inf_nan = 0;
		*is_missing = 0;
		for (TCSize_t i = 0; i < len; i++)
		{
			if (TCError::IsMissingImpl<T>::run(&x[i])) 
			{
				*is_missing = 1;
				return;
			}
			
			
			if (Utils::isinf(x[i]) || Utils::isnan(x[i])) 
			{
				*is_inf_nan = 1;
				return;
			}
		}
	}


	template <typename T>
	void moving_average_triangular_single(T* data_series, TCUInt64 data_length, TCUInt64 window_length, TCUInt64 offset, bool two_peaks,
		T* external_data_lag, TCUInt64 lag_size, T* external_data_lead, TCUInt64 lead_size, TCUInt64 n, T* tma) 
	{
		TCUInt64 weights_sum = 0;
		if (n + offset + lag_size + 1 < window_length || n >= data_length - offset + lead_size) 
		{
			*tma = _get_tc_error<T>(TCError_IllegalInput); 
			return;
		}

		*tma = 0;
		TCInt64 left = -window_length + offset + n + 1;
		TCInt64 right = offset + n;
		TCInt64 w_left = 0;
		if (window_length - offset >= n + 2)
		{
			if (!moving_average_single_part(external_data_lag, lag_size - window_length + offset + n + 1, lag_size - 1, 
				0, window_length, offset, two_peaks, &weights_sum, tma))
			{
				return;
			}
			w_left = window_length - offset - n - 1;
			left = 0;  
		}

		if (data_length - n <= offset) 
		{
			if (!moving_average_single_part(external_data_lead, 0, offset + n - data_length,
				window_length - 1 - offset - n + data_length, window_length, offset, two_peaks, &weights_sum, tma))
			{
				return;
			}
			right = data_length - 1;
		}

		if(!moving_average_single_part(data_series, left, right, w_left, window_length, offset, two_peaks, &weights_sum, tma))
		{
			return;
		}
		*tma /= weights_sum;
	} 

	template <typename T>
	TCErrorCode moving_average_triangular(const TCArray* data_series, TCUInt64 window_length, T foffset, 
		const TCArray* external_data_lag, const TCArray* external_data_lead, TCArray* tma_series) 
	{
		T epsilon = std::numeric_limits<T>::epsilon();
		bool two_peaks = false;
		TCUInt64 offset;
		
		TCSize_t n = data_series->m_numelt;
		
		if (n == 0) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(DataSeriesArg));
		}

		
		if (window_length == 0)
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(WindowLengthArg));
		}

		if (TCError::IsMissingImpl<T>::run(&foffset))
		{
			foffset = 0.0;
		} else if (Utils::isinf(foffset) || Utils::isinf(foffset) || foffset < 0)
		{
			
			
			
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(OffsetArg));
		}

		
		if (fabs(foffset - (window_length - 1) / 2.0) <= epsilon)
		{
			two_peaks = true;
		}

		offset = (TCUInt64) foffset;

		
		if (offset >= window_length) 
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(OffsetArg));
		}

		TCSize_t v_count;
		TCSize_t data_length;

		
		if (!dim_analysis(data_series, &data_length, &v_count)) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(DataSeriesArg)); 
		} 

		
		if (window_length > data_length) 
		{
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(WindowLengthArg));
		}

		TCSize_t lag_v_count;
		TCSize_t lag_length;

		
		if (!dim_analysis(external_data_lag, data_series->m_ndims, &lag_length, &lag_v_count) && external_data_lag->m_numelt > 0) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(ExternalDataLagArg));
		} 

		
		if (external_data_lag->m_numelt > 0 && v_count != lag_v_count)
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(ExternalDataLagArg));
		}

		TCSize_t lead_v_count;
		TCSize_t lead_length;

		
		if (!dim_analysis(external_data_lead, data_series->m_ndims, &lead_length, &lead_v_count) && external_data_lead->m_numelt > 0) 
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(ExternalDataLeadArg)); 
		} 

		
		if (external_data_lead->m_numelt > 0 && v_count != lead_v_count)
		{
			return tcerror_code_new(TCError_IllegalSize, TCArgumentID(ExternalDataLeadArg));
		}

		
		T* data = (T*) data_series->m_data;
		T* lag = (T*) external_data_lag->m_data;
		T* lead = (T*) external_data_lead->m_data;
		T* tma = (T*) tma_series->m_data;
		TCBool is_inf_nan;
		TCBool is_missing;

		
		check_mis_inf_nan<T>(data, data_series->m_numelt, &is_inf_nan, &is_missing);
		if (is_inf_nan) {
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(DataSeriesArg));
		}

		check_mis_inf_nan(lag, external_data_lag->m_numelt, &is_inf_nan, &is_missing);
		if (is_inf_nan) {
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(ExternalDataLagArg));
		}

		check_mis_inf_nan(lead, external_data_lead->m_numelt, &is_inf_nan, &is_missing);
		if (is_inf_nan) {
			return tcerror_code_new(TCError_IllegalInput, TCArgumentID(ExternalDataLeadArg));
		}

		for (TCUInt64 v = 0; v < v_count; v++)
		{
			for (TCUInt64 n = 0; n < data_length; n++)
			{
				moving_average_triangular_single(data + v*data_length, data_length, window_length, offset, two_peaks,
					lag + v*lag_length, lag_length, 
					lead + v*lead_length, lead_length, n, 
					tma + v*data_length + n);
			}
		}
		return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
	}
}