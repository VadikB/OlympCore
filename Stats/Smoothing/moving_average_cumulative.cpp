#include "stdafx.h"
#include "moving_average_cumulative.h"

using namespace Smoothing;

TCErrorCode stats_smoothing_moving_average_cumulative_f_e(
	const TCArray* data_series,
	TCFloat external_cma_value,
	TCUInt64 external_cma_length,
	TCArray* cma_series)
{
	assert(data_series->m_tag == TC_FLOAT);

	return stats_smoothing_moving_average_cumulative<TCFloat>(data_series, external_cma_value, external_cma_length, cma_series);
}

TCErrorCode stats_smoothing_moving_average_cumulative_short_f_e(const TCArray* data_series, TCArray* cma_series)
{
	TCUInt32 missing = TCERROR_FLOAT_MISSING;
	return stats_smoothing_moving_average_cumulative_f_e(data_series, (*(TCFloat*)(&missing)), 0, cma_series);
}


TCErrorCode stats_smoothing_moving_average_cumulative_d_e(
	const TCArray* data_series,
	TCDouble external_cma_value,
	TCUInt64 external_cma_length,
	TCArray* cma_series)
{
	assert(data_series->m_tag == TC_DOUBLE);

	return stats_smoothing_moving_average_cumulative<TCDouble>(data_series, external_cma_value, external_cma_length, cma_series);
}

TCErrorCode stats_smoothing_moving_average_cumulative_short_d_e(const TCArray* data_series, TCArray* cma_series)
{
	TCUInt64 missing = TCERROR_DOUBLE_MISSING;
	return stats_smoothing_moving_average_cumulative_d_e(data_series, (*(TCDouble*)(&missing)), 0, cma_series);
}

