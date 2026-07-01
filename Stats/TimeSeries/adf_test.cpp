//<copyright>
// Copyright (c) Microsoft Corporation. All rights reserved.
//</copyright>

#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "OlympCore.h"
#include "TCFuncWrapper.h"

#include "adf_test.h"

using namespace adf_test_internal;
using namespace TimeSeriesUtils;

C_BEGINF(stats_timeseries_adf_test_ar_f_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCFloat* pvalue)
{
    return adf_test_ar_internal<TCFloat, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF

C_BEGINF(stats_timeseries_adf_test_ar_d_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCDouble* pvalue)
{
    return adf_test_ar_internal<TCDouble, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF

C_BEGINF(stats_timeseries_adf_test_ard_f_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCFloat* pvalue)
{
    return adf_test_ard_internal<TCFloat, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF

C_BEGINF(stats_timeseries_adf_test_ard_d_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCDouble* pvalue)
{
    return adf_test_ard_internal<TCDouble, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF

C_BEGINF(stats_timeseries_adf_test_ts_f_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCFloat* pvalue)
{
    return adf_test_ts_internal<TCFloat, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF

C_BEGINF(stats_timeseries_adf_test_ts_d_e, In TCArray* x, In TCUInt32 p, In TCInt32 testType, Out TCDouble* pvalue)
{
    return adf_test_ts_internal<TCDouble, TimeSeriesUtils::sequential>(x, p, testType, pvalue);
} C_ENDF