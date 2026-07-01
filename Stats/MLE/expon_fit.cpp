//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "expon_fit.h"
#include "chisq.h"
#include "TCArrayUtilsInternal.h"

using namespace MLE;

TCErrorCode stats_mle_expon_fit_a_f_e(In TCArray* data, In TCFloat alpha, Out TCFloat* lambda, Out TCFloat* lower_ci_bound, Out TCFloat* upper_ci_bound)
{
    try
    {
        return ExponFit<TCFloat>(data, alpha, lambda, lower_ci_bound, upper_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }    
}

TCErrorCode stats_mle_expon_fit_a_d_e(In TCArray* data, In TCDouble alpha, Out TCDouble* lambda, Out TCDouble* lower_ci_bound, Out TCDouble* upper_ci_bound)
{
    try
    {
        return ExponFit<TCDouble>(data, alpha, lambda, lower_ci_bound, upper_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}

TCErrorCode stats_mle_expon_fit_f_e(In TCArray* data, Out TCFloat* lambda, Out TCFloat* lower_ci_bound, Out TCFloat* upper_ci_bound)
{
    try
    {
        return ExponFit<TCFloat>(data, (TCFloat)0.05, lambda, lower_ci_bound, upper_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}

TCErrorCode stats_mle_expon_fit_d_e(In TCArray* data, Out TCDouble* lambda, Out TCDouble* lower_ci_bound, Out TCDouble* upper_ci_bound)
{
    try
    {
        return ExponFit<TCDouble>(data, (TCDouble)0.05, lambda, lower_ci_bound, upper_ci_bound);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}