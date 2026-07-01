#include "stdafx.h"
#include "esd.h"

using namespace Outliers;

TCErrorCode stats_outlier_test_esd_f_e(const TCArray *samples, const TCUInt64 outliers_amount, const TCFloat alpha,TCArray** indexes,TCArray** statistics, TCArray** critical_values)
{
	return ESD<TCFloat>::esd(samples,outliers_amount,alpha,indexes,statistics,critical_values);
}


TCErrorCode stats_outlier_test_esd_d_e(const TCArray *samples, const TCUInt64 outliers_amount, const TCDouble alpha,TCArray** indexes,TCArray** statistics, TCArray** critical_values)
{
	return ESD<TCDouble>::esd(samples,outliers_amount,alpha,indexes,statistics,critical_values);
}

