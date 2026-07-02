



#include "stdafx.h"
#include "poisson_fit.h"
#include "chisq.h"
#include "TCArrayUtilsInternal.h"

using namespace MLE;

TCErrorCode stats_mle_poisson_fit_a_f_e(In TCArray* data, In TCFloat alpha, Out TCFloat* lambda, Out TCFloat* lower_ci_bound, Out TCFloat* upper_ci_bound)
{
    try
    {
        return PoissonFit<TCFloat, TC_FLOAT>(data, alpha, lambda, lower_ci_bound, upper_ci_bound);
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

TCErrorCode stats_mle_poisson_fit_a_d_e(In TCArray* data, TCDouble alpha, Out TCDouble* lambda, Out TCDouble* lower_ci_bound, Out TCDouble* upper_ci_bound)
{
    try
    {
        return PoissonFit<TCDouble, TC_DOUBLE>(data, alpha, lambda, lower_ci_bound, upper_ci_bound);
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

TCErrorCode stats_mle_poisson_fit_f_e(In TCArray* data, Out TCFloat* lambda, Out TCFloat* lower_ci_bound, Out TCFloat* upper_ci_bound)
{
    try
    {
        return PoissonFit<TCFloat, TC_FLOAT>(data, (TCFloat)0.05, lambda, lower_ci_bound, upper_ci_bound);
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

TCErrorCode stats_mle_poisson_fit_d_e(In TCArray* data, Out TCDouble* lambda, Out TCDouble* lower_ci_bound, Out TCDouble* upper_ci_bound)
{
    try
    {
        return PoissonFit<TCDouble, TC_DOUBLE>(data, (TCDouble)0.05, lambda, lower_ci_bound, upper_ci_bound);
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

