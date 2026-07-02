



#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "OlympCore.h"
#include <TCFuncWrapper.h>
#include "timeseries_ar.h"

using namespace timeseries_ar_internal;
using namespace TimeSeriesUtils;

C_BEGINF(stats_timeseries_ar_f_e, In TCArray* data, In TCUInt32 order, In TSEstimationMethod method, In TSExtrapolateSample presample, 
    Out TCArray* params, Out TCArray* tstats, Out TCArray* residuals, Out TCFloat* sse, Out TCFloat* r2 , Out TCArray* cov)
{
    return stats_timeseries_ar_internal<TCFloat, TimeSeriesUtils::sequential>(data, order, method, presample, params, tstats, residuals, sse, r2, cov);
} C_ENDF

C_BEGINF(stats_timeseries_ar_d_e, In TCArray* data, In TCUInt32 order, In TSEstimationMethod method, In TSExtrapolateSample presample, 
    Out TCArray* params, Out TCArray* tstats, Out TCArray* residuals, Out TCDouble* sse, Out TCDouble* r2 , Out TCArray* cov)
{
    return stats_timeseries_ar_internal<TCDouble, TimeSeriesUtils::sequential>(data, order, method, presample, params, tstats, residuals, sse, r2 , cov);
} C_ENDF

