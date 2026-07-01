//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>


#include "stdafx.h"
#include "moving_average_simple.h"
#include "TCArrayUtilsInternal.h"


using namespace Smoothing;

TCErrorCode stats_smoothing_moving_average_simple_f_e(const TCArray* data_series, TCUInt64 window_length, TCUInt64 offset, 
	const TCArray* external_data_lag, const TCArray* external_data_lead, TCArray* sma_series) 
{
	assert(data_series->m_tag == TC_FLOAT);

	return moving_average_simple<TCFloat>(data_series, window_length, offset, external_data_lag, external_data_lead, sma_series);
}

TCErrorCode stats_smoothing_moving_average_simple_short_f_e(const TCArray* data_series, TCUInt64 window_length, TCUInt64 offset, TCArray* sma_series) 
{
	        TCArray* external_data_lag; 
	        TCArray* external_data_lead; 
			TCErrorCode error_code = TCArray_New_2D(TC_FLOAT, 0, 0, false, &external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_New_2D(TC_FLOAT, 0, 0, false, &external_data_lead);
            assert(error_code == TCError_NoError);
			TCErrorCode error_code_out = stats_smoothing_moving_average_simple_f_e(data_series, window_length, offset, external_data_lag, external_data_lead, sma_series);
			error_code = TCArray_Delete(&external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_Delete(&external_data_lead);
            assert(error_code == TCError_NoError);
			return error_code_out;
}


TCErrorCode stats_smoothing_moving_average_simple_d_e(const TCArray* data_series, TCUInt64 window_length, TCUInt64 offset, 
	const TCArray* external_data_lag, const TCArray* external_data_lead, TCArray* sma_series) 
{
	assert(data_series->m_tag == TC_DOUBLE);

	return moving_average_simple<TCDouble>(data_series, window_length, offset, external_data_lag, external_data_lead, sma_series);
}

TCErrorCode stats_smoothing_moving_average_simple_short_d_e(const TCArray* data_series, TCUInt64 window_length, TCUInt64 offset, TCArray* sma_series) 
{
	        TCArray* external_data_lag; 
	        TCArray* external_data_lead; 
			TCErrorCode error_code = TCArray_New_2D(TC_DOUBLE, 0, 0, false, &external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_New_2D(TC_DOUBLE, 0, 0, false, &external_data_lead);
            assert(error_code == TCError_NoError);
			TCErrorCode error_code_out = stats_smoothing_moving_average_simple_d_e(data_series, window_length, offset, external_data_lag, external_data_lead, sma_series);
			error_code = TCArray_Delete(&external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_Delete(&external_data_lead);
            assert(error_code == TCError_NoError);
			return error_code_out;
}


