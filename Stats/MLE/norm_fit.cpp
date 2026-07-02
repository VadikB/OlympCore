



#include "stdafx.h"

#include "norm_fit.h"
#include <iostream>

using namespace MLE;

TCErrorCode stats_mle_norm_fit_a_f_e(
	In TCArray* data,
	In TCFloat alpha,
	Out TCFloat* mean,
	Out TCFloat* stdev,
	Out TCFloat* lower_mean_ci_bound,
	Out TCFloat* upper_mean_ci_bound,
	Out TCFloat* lower_stdev_ci_bound,
	Out TCFloat* upper_stdev_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCFloat>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    try
    {
        return norm_fit<TCFloat, TC_FLOAT>(data, alpha, mean, stdev, lower_mean_ci_bound, upper_mean_ci_bound, lower_stdev_ci_bound, upper_stdev_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}

TCErrorCode stats_mle_norm_fit_a_d_e(
	In TCArray* data,
	In TCDouble alpha,
	Out TCDouble* mean,
	Out TCDouble* stdev,
	Out TCDouble* lower_mean_ci_bound,
	Out TCDouble* upper_mean_ci_bound,
	Out TCDouble* lower_stdev_ci_bound,
	Out TCDouble* upper_stdev_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCDouble>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    try
    {
        return norm_fit<TCDouble, TC_DOUBLE>(data, alpha, mean, stdev, lower_mean_ci_bound, upper_mean_ci_bound, lower_stdev_ci_bound, upper_stdev_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}

TCErrorCode stats_mle_norm_fit_f_e(
	In TCArray* data,
	Out TCFloat* mean,
	Out TCFloat* stdev,
	Out TCFloat* lower_mean_ci_bound,
	Out TCFloat* upper_mean_ci_bound,
	Out TCFloat* lower_stdev_ci_bound,
	Out TCFloat* upper_stdev_ci_bound)
{
	try
    {
        return norm_fit<TCFloat, TC_FLOAT>(data, 0.05, mean, stdev, lower_mean_ci_bound, upper_mean_ci_bound, lower_stdev_ci_bound, upper_stdev_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}

TCErrorCode stats_mle_norm_fit_d_e(
	In TCArray* data,
	Out TCDouble* mean,
	Out TCDouble* stdev,
	Out TCDouble* lower_mean_ci_bound,
	Out TCDouble* upper_mean_ci_bound,
	Out TCDouble* lower_stdev_ci_bound,
	Out TCDouble* upper_stdev_ci_bound)
{
	try
    {
        return norm_fit<TCDouble, TC_DOUBLE>(data, 0.05, mean, stdev, lower_mean_ci_bound, upper_mean_ci_bound, lower_stdev_ci_bound, upper_stdev_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}