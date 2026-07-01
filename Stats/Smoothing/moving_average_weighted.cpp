//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>


#include "stdafx.h"
#include "moving_average_weighted.h"
#include "TCArrayUtilsInternal.h"

using namespace Smoothing;

TCErrorCode stats_smoothing_moving_average_weighted_f_e(const TCArray* data_series, const TCArray* weights, TCUInt64 offset, 
	const TCArray* external_data_lag, const TCArray* external_data_lead, TCArray* wma_series) 
{
	assert(data_series->m_tag == TC_FLOAT);

	return moving_average_weighted<TCFloat>(data_series, weights, offset, external_data_lag, external_data_lead, wma_series);
}


TCErrorCode stats_smoothing_moving_average_weighted_short_f_e(const TCArray* data_series, const TCArray* weights, TCUInt64 offset, TCArray* wma_series) 
{
	        TCArray* external_data_lag; 
	        TCArray* external_data_lead; 
			TCErrorCode error_code = TCArray_New_2D(TC_FLOAT, 0, 0, false, &external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_New_2D(TC_FLOAT, 0, 0, false, &external_data_lead);
            assert(error_code == TCError_NoError);
			TCErrorCode error_code_out = stats_smoothing_moving_average_weighted_f_e(data_series, weights, offset, external_data_lag, external_data_lead, wma_series);
			error_code = TCArray_Delete(&external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_Delete(&external_data_lead);
            assert(error_code == TCError_NoError);
			return error_code_out;
}

TCErrorCode stats_smoothing_moving_average_weighted_d_e(const TCArray* data_series, const TCArray* weights, TCUInt64 offset, 
	const TCArray* external_data_lag, const TCArray* external_data_lead, TCArray* wma_series) 
{
	assert(data_series->m_tag == TC_DOUBLE);

	return moving_average_weighted<TCDouble>(data_series, weights, offset, external_data_lag, external_data_lead, wma_series);
}

TCErrorCode stats_smoothing_moving_average_weighted_short_d_e(const TCArray* data_series, const TCArray* weights, TCUInt64 offset, TCArray* wma_series) 
{
	        TCArray* external_data_lag; 
	        TCArray* external_data_lead; 
			TCErrorCode error_code = TCArray_New_2D(TC_DOUBLE, 0, 0, false, &external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_New_2D(TC_DOUBLE, 0, 0, false, &external_data_lead);
            assert(error_code == TCError_NoError);
			TCErrorCode error_code_out = stats_smoothing_moving_average_weighted_d_e(data_series, weights, offset, external_data_lag, external_data_lead, wma_series);
			error_code = TCArray_Delete(&external_data_lag);
            assert(error_code == TCError_NoError);
			error_code = TCArray_Delete(&external_data_lead);
            assert(error_code == TCError_NoError);
			return error_code_out;
}


