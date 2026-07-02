



#include "stdafx.h" 
#include "dq_test.h"

using namespace Outliers;

TCErrorCode stats_outliers_test_dq_f_e(const TCArray* sample, TCUInt32 method,
	TCHypothesis tail, TCFloat* p_value, TCFloat* test_statistics, TCUInt64* outlier_index)
{
	assert(sample->m_tag == TC_FLOAT);

	return stats_outliers_test_dq<TCFloat>(sample, method, tail, p_value, test_statistics, outlier_index);
}

TCErrorCode stats_outliers_test_dq_d_e(const TCArray* sample, TCUInt32 method,
	TCHypothesis tail, TCDouble* p_value, TCDouble* test_statistics, TCUInt64* outlier_index)
{
	assert(sample->m_tag == TC_DOUBLE);

	return stats_outliers_test_dq<TCDouble>(sample, method, tail, p_value, test_statistics, outlier_index);
}
