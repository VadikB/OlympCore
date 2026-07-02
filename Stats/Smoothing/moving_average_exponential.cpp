




#include "stdafx.h"
#include "moving_average_exponential.h"

using namespace Smoothing;

TCErrorCode stats_smoothing_moving_average_exponential_f_e(
	const TCArray* data_series,
	TCUInt64 window_length,
	TCEMAType ema_type,
	TCFloat prev_ema,
	TCArray * ema_series) 
{
	assert(data_series->m_tag == TC_FLOAT);

	return stats_smoothing_moving_average_exponential<TCFloat>(data_series, window_length, ema_type, prev_ema, ema_series);
}

TCErrorCode stats_smoothing_moving_average_exponential_short_f_e(
	const TCArray* data_series,
	TCUInt64 window_length,
	TCEMAType ema_type,
	TCArray * ema_series) 
{
	TCUInt32 missing = TCERROR_FLOAT_MISSING;
	return stats_smoothing_moving_average_exponential_f_e(data_series, window_length, ema_type, (*(TCFloat*)(&missing)), ema_series);
}


TCErrorCode stats_smoothing_moving_average_exponential_d_e(
	const TCArray* data_series,
	TCUInt64 window_length,
	TCEMAType ema_type,
	TCDouble prev_ema,
	TCArray * ema_series) 
{
	assert(data_series->m_tag == TC_DOUBLE);

	return stats_smoothing_moving_average_exponential<TCDouble>(data_series, window_length, ema_type, prev_ema, ema_series);
}

TCErrorCode stats_smoothing_moving_average_exponential_short_d_e(
	const TCArray* data_series,
	TCUInt64 window_length,
	TCEMAType ema_type,
	TCArray * ema_series) 
{
	TCUInt64 missing = TCERROR_DOUBLE_MISSING;
	return stats_smoothing_moving_average_exponential_d_e(data_series, window_length, ema_type, (*(TCDouble*)(&missing)), ema_series);
}


