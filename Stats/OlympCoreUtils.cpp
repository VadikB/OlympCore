//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>


#include "stdafx.h"
#include "OlympCoreUtils.h"
#include "TCMath.h"
#include <math.h>

namespace Utils
{
    TCDouble abs(TCDouble x)
    {
        return math_abs_d(x);
    }
    TCFloat abs(TCFloat x)
    {
        return math_abs_f(x);
    }
    TCDouble abs(TCDComplex x)
    {
        return math_abs_z(x);
    }
    TCFloat abs(TCFComplex x)
    {
        return math_abs_c(x);
    }

    TCDouble combin(TCDouble a, TCDouble b)
    {
        return math_combin_d(a,b);
    }
    TCFloat combin(TCFloat a, TCFloat b)
    {
        return math_combin_f(a,b);
    }

    TCDouble eps(TCDouble x)
    {
        return math_eps_d(x);
    }
    TCFloat eps(TCFloat x)
    {
        return math_eps_f(x);
    }

    TCDouble pow(TCDouble x, TCDouble y)
    {
        return math_power_d(x, y);
    }
    TCFloat pow(TCFloat x, TCFloat y)
    {
        return math_power_f(x, y);
    }

    TCDouble exp(TCDouble x)
    {
        return math_exp_d(x);
    }
    TCFloat exp(TCFloat x)
    {
        return math_exp_f(x);
    }

    TCDouble log(TCDouble x)
    {
        return math_ln_d(x);
    }
    TCFloat log(TCFloat x)
    {
        return math_ln_f(x);
    }

    TCDouble log2(TCDouble x)
    {
        return math_log2_d(x);
    }
    TCFloat log2(TCFloat x)
    {
        return math_log2_f(x);
    }

    TCDouble sqrt(TCDouble x)
    {
        return math_sqrt_d(x);
    }
    TCFloat sqrt(TCFloat x)
    {
        return math_sqrt_f(x);
    }
    TCDComplex sqrt(TCDComplex x)
    {
        return math_sqrt_z(x);
    }
    TCFComplex sqrt(TCFComplex x)
    {
        return math_sqrt_c(x);
    }

    TCDouble tan(TCDouble x)
    {
        return math_tan_d(x);
    }
    TCFloat tan(TCFloat x)
    {
        return math_tan_f(x);
    }

    TCDouble cot(TCDouble x)
    {
        return math_cot_d(x);
    }
    TCFloat cot(TCFloat x)
    {
        return math_cot_f(x);
    }

    TCDouble cos(TCDouble x)
    {
        return math_cos_d(x);
    }
    TCFloat cos(TCFloat x)
    {
        return math_cos_f(x);
    }

    TCDouble atan(TCDouble x)
    {
        return math_atan_d(x);
    }
    TCFloat atan(TCFloat x)
    {
        return math_atan_f(x);
    }

    TCDouble acos(TCDouble x)
    {
        return math_acos_d(x);
    }
    TCFloat acos(TCFloat x)
    {
        return math_acos_f(x);
    }

    TCDouble pi()
    {
        return math_pi_d();
    }
    
    TCDouble beta(TCDouble x, TCDouble y)
    {
        return math_beta_d(x, y);
    }

    TCFloat beta(TCFloat x, TCFloat y)
    {
        return math_beta_f(x, y);
    }

    TCDouble betaln(TCDouble x, TCDouble y)
    {
        return math_betaln_d(x, y);
    }

    TCFloat betaln(TCFloat x, TCFloat y)
    {
        return math_betaln_f(x, y);
    }

    TCDouble betainc(TCDouble x, TCDouble a, TCDouble b)
    {
        return math_beta_regularized_d(x, a, b);
    }

    TCFloat betainc(TCFloat x, TCFloat a, TCFloat b)
    {
        return math_beta_regularized_f(x, a, b);
    }

    TCDouble invbetainc(TCDouble x, TCDouble a, TCDouble b)
    {
        return math_beta_regularized_inv_d(x, a, b);
    }

    TCFloat invbetainc(TCFloat x, TCFloat a, TCFloat b)
    {
        return math_beta_regularized_inv_f(x, a, b);
    }

    TCDouble gammaln(TCDouble x)
    {
        return math_gammaln_d(x);
    }

    TCFloat gammaln(TCFloat x)
    {
        return math_gammaln_f(x);
    }

	TCDouble gammaln_precise(TCDouble x)
    {
        return math_gammaln_precise_d(x);
    }

	TCFloat gammaln_precise(TCFloat x)
    {
        return math_gammaln_precise_f(x);
    }

    TCDouble gamma(TCDouble x)
    {
        return math_gamma_d(x);
    }

    TCFloat gamma(TCFloat x)
    {
        return math_gamma_f(x);
    }

    TCDouble gammainc(TCDouble a, TCDouble x)
    {
        return math_gamma_regularized_p_d(a, x);
    }

    TCFloat gammainc(TCFloat a, TCFloat x)
    {
        return math_gamma_regularized_p_f(a, x);
    }

    TCDouble gamma_regularized_p(TCDouble a, TCDouble x)
    {
        return math_gamma_regularized_p_d(a, x);
    }

    TCFloat gamma_regularized_p(TCFloat a, TCFloat x)
    {
        return math_gamma_regularized_p_f(a, x);
    }

    TCDouble gamma_regularized_q(TCDouble a, TCDouble x)
    {
        return math_gamma_regularized_q_d(a, x);
    }

    TCFloat gamma_regularized_q(TCFloat a, TCFloat x)
    {
        return math_gamma_regularized_q_f(a, x);
    }

    TCFloat gamma_regularized_p_inv(const TCFloat a, const TCFloat x)
    {
        return math_gamma_regularized_p_inv_f(a, x);
    }

    TCDouble gamma_regularized_p_inv(const TCDouble a, const TCDouble x)
    {
        return math_gamma_regularized_p_inv_d(a, x);
    }

    TCFloat gamma_regularized_q_inv(const TCFloat a, const TCFloat x)
    {
        return math_gamma_regularized_q_inv_f(a, x);
    }

    TCDouble gamma_regularized_q_inv(const TCDouble a, const TCDouble x)
    {
        return math_gamma_regularized_q_inv_d(a, x);
    }

    TCFloat expm1(TCFloat x) 
    { 
        return math_expm1_f(x);  
    }

    TCDouble expm1(TCDouble x)  
    { 
        return math_expm1_d(x); 
    }

    TCFloat log1p(TCFloat x) 
    { 
        return math_log1p_f(x);  
    }

    TCDouble log1p(TCDouble x)  
    { 
        return math_log1p_d(x); 
    }

    TCFloat erf(TCFloat x) 
    { 
        return math_erf_f(x);  
    }

    TCDouble erf(TCDouble x)  
    { 
        return math_erf_d(x); 
    }

    TCFloat erfc(TCFloat x) 
    { 
        return math_erfc_f(x);  
    }

    TCDouble erfc(TCDouble x)  
    { 
        return math_erfc_d(x); 
    }

    TCFloat erfinv(TCFloat x) 
    { 
        return math_erfinv_f(x);  
    }

    TCDouble erfinv(TCDouble x)  
    { 
        return math_erfinv_d(x); 
    }

    TCFloat erfcinv(TCFloat x) 
    { 
        return math_erfcinv_f(x);  
    }

    TCDouble erfcinv(TCDouble x)  
    { 
        return math_erfcinv_d(x); 
    }

    TCFloat floor(const TCFloat x)
    {
        return std::floorf(x);
    }

    TCDouble floor(const TCDouble x)
    {
        return std::floor(x);
    }

    TCFloat ceil(const TCFloat x)
    {
        return std::ceilf(x);
    }

    TCDouble ceil(const TCDouble x)
    {
        return std::ceil(x);
    }

   
    TCFloat isinf(TCFloat x)
    {
        TCBoolError val = math_isinf_f(x);
        if (!tcerror_iserror_b(&val))
        {
            return tcerror_getvalue_b(&val);
        }
        else
        {
            return 0;
        }
    }

    TCDouble isinf(TCDouble x)
    {
        TCBoolError val = math_isinf_d(x);
        if (!tcerror_iserror_b(&val))
        {
            return tcerror_getvalue_b(&val);
        }
        else
        {
            return 0;
        }
    }

    TCFloat isnan(TCFloat x)
    {
        return math_isnan_f(x);
    }

    TCDouble isnan(TCDouble x)
    {
        return math_isnan_d(x);
    }

    TCErrorCode redop_s_sum(TCSize_t numelt, TCBool isref, TCDouble* data, TCDouble* y)
    {
        return math_redop_s_sum_d(numelt, isref, data, y);
    }
    TCErrorCode redop_s_sum(TCSize_t numelt, TCBool isref, TCFloat* data, TCFloat* y)
    {
        return math_redop_s_sum_f(numelt, isref, data, y);
    }
    TCErrorCode redop_s_sum(TCSize_t numelt, TCBool isref, TCFComplex* data, TCFComplex* y)
    {
        return math_redop_s_sum_c(numelt, isref, data, y);
    }
    TCErrorCode redop_s_sum(TCSize_t numelt, TCBool isref, TCDComplex* data, TCDComplex* y)
    {
        return math_redop_s_sum_z(numelt, isref, data, y);
    }

    template <>
    TCErrorCode redop_sum<TCFloat>(const TCArray* in_arrays, TCSize_t narrays, TCSize_t dim, TCArray** out_array)
    {
        return math_redop_sum_f_e(in_arrays, narrays, dim, out_array);
    }

    template <>
    TCErrorCode redop_sum<TCDouble>(const TCArray* in_arrays, TCSize_t narrays, TCSize_t dim, TCArray** out_array)
    {
        return math_redop_sum_d_e(in_arrays, narrays, dim, out_array);
    }

    TCErrorCode redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCFloat* datax, TCFloat* datay,
        TCFloat valuex, TCFloat valuey, TCFloat* y)
    {
        return math_redop_s_sum2sqr_f(numelt, isref, datax, datay, valuex, valuey, y);
    }
    TCErrorCode redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCDouble* datax, TCDouble* datay,
        TCDouble valuex, TCDouble valuey, TCDouble* y)
    {
        return math_redop_s_sum2sqr_d(numelt, isref, datax, datay, valuex, valuey, y);
    }
    TCErrorCode redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCFComplex* datax, TCFComplex* datay,
        TCFComplex valuex, TCFComplex valuey, TCFComplex* y)
    {
        return math_redop_s_sum2sqr_c(numelt, isref, datax, datay, valuex, valuey, y);
    }
    TCErrorCode redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCDComplex* datax, TCDComplex* datay,
        TCDComplex valuex, TCDComplex valuey, TCDComplex* y)
    {
        return math_redop_s_sum2sqr_z(numelt, isref, datax, datay, valuex, valuey, y);
    }
}
