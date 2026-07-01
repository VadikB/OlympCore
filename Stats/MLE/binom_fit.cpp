//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "binom_fit.h"
#include <iostream>
#include <windows.h>

using namespace MLE;

TCErrorCode stats_mle_binom_fit_a_f_e(In TCArray* data, In TCFloat alpha, Out TCFloat* p, Out TCFloat* lower_p_ci_bound, Out TCFloat* upper_p_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCFloat>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    try
    {
        return binom_fit<TCFloat>(data, alpha, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_a_d_e(In TCArray* data, In TCDouble alpha, Out TCDouble* p, Out TCDouble* lower_p_ci_bound, Out TCDouble* upper_p_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCDouble>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    try
    {
        return binom_fit<TCDouble>(data, alpha, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_f_e(In TCArray* data, Out TCFloat* p, Out TCFloat* lower_p_ci_bound, Out TCFloat* upper_p_ci_bound)
{
    try
    {
        return binom_fit<TCFloat>(data, 0.05, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_d_e(In TCArray* data, Out TCDouble* p, Out TCDouble* lower_p_ci_bound, Out TCDouble* upper_p_ci_bound)
{
    try
    {
        return binom_fit<TCDouble>(data, 0.05, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_short_a_f_e(In TCUInt64 number_s, In TCUInt64 trials, In TCFloat alpha, Out TCFloat* p, Out TCFloat* lower_p_ci_bound, Out TCFloat* upper_p_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCFloat>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    try
    {
        return binom_fit<TCFloat>(number_s, trials, alpha, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_short_a_d_e(In TCUInt64 number_s, In TCUInt64 trials, In TCDouble alpha, Out TCDouble* p, Out TCDouble* lower_p_ci_bound, Out TCDouble* upper_p_ci_bound)
{
	if ((alpha <= 0) || (alpha >= 1) || isnan<TCDouble>(alpha))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    try
    {
        return binom_fit<TCDouble>(number_s, trials, alpha, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_short_f_e(In TCUInt64 number_s, In TCUInt64 trials, Out TCFloat* p, Out TCFloat* lower_p_ci_bound, Out TCFloat* upper_p_ci_bound)
{
    try
    {
        return binom_fit<TCFloat>(number_s, trials, 0.05, p, lower_p_ci_bound, upper_p_ci_bound);
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

TCErrorCode stats_mle_binom_fit_short_d_e(In TCUInt64 number_s, In TCUInt64 trials, Out TCDouble* p, Out TCDouble* lower_p_ci_bound, Out TCDouble* upper_p_ci_bound)
{
    try
    {
        return binom_fit<TCDouble>(number_s, trials, 0.05, p, lower_p_ci_bound, upper_p_ci_bound);
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