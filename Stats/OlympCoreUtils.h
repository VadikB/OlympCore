



#pragma once


typedef enum
{
    stats_enum_dist_pdf = 0,
    stats_enum_dist_cdf = 1,
    stats_enum_dist_rcdf = 2,
}
STATS_DIST_TYPE;

namespace Utils
{
    TCDouble abs(TCDouble x);
    TCFloat abs(TCFloat x);
    TCDouble abs(TCDComplex x);
    TCFloat abs(TCFComplex x);

    TCDouble combin(TCDouble a, TCDouble b);
    TCFloat combin(TCFloat a, TCFloat b);

    TCDouble eps(TCDouble x);
    TCFloat eps(TCFloat x);

    TCDouble pow(TCDouble x, TCDouble y);
    TCFloat pow(TCFloat x, TCFloat y);

    TCDouble exp(TCDouble x);
    TCFloat exp(TCFloat x);

    TCDouble log(TCDouble x);
    TCFloat log(TCFloat x);

    TCDouble log2(TCDouble x);
    TCFloat log2(TCFloat x);

    TCDouble sqrt(TCDouble x);
    TCFloat sqrt(TCFloat x);
    TCDComplex sqrt(TCDComplex x);
    TCFComplex sqrt(TCFComplex x);

    TCDouble tan(TCDouble x);
    TCFloat tan(TCFloat x);

    TCDouble cot(TCDouble x);
    TCFloat cot(TCFloat x);

    TCDouble cos(TCDouble x);
    TCFloat cos(TCFloat x);
    
    TCDouble atan(TCDouble x);
    TCFloat atan(TCFloat x);
    
    TCDouble acos(TCDouble x);
    TCFloat acos(TCFloat x);

    TCDouble pi();
    
    TCDouble beta(TCDouble x, TCDouble y);
    TCFloat beta(TCFloat x, TCFloat y);

    TCDouble betaln(TCDouble x, TCDouble y);
    TCFloat betaln(TCFloat x, TCFloat y);

    TCDouble betainc(TCDouble x, TCDouble a, TCDouble b);
    TCFloat betainc(TCFloat x, TCFloat a, TCFloat b);

    TCDouble invbetainc(TCDouble x, TCDouble a, TCDouble b);
    TCFloat invbetainc(TCFloat x, TCFloat a, TCFloat b);

    TCDouble gammaln(TCDouble x);
    TCFloat gammaln(TCFloat x);

    TCDouble gammaln_pecise(TCDouble x);
    TCFloat gammaln_precise(TCFloat x);

    TCDouble gamma(TCDouble x);
    TCFloat gamma(TCFloat x);

    TCDouble gammainc(TCDouble a, TCDouble x);
    TCFloat gammainc(TCFloat a, TCFloat x);

    TCDouble gamma_regularized_p(TCDouble a, TCDouble x);
    TCFloat gamma_regularized_p(TCFloat a, TCFloat x);

    TCDouble gamma_regularized_q(TCDouble a, TCDouble x);
    TCFloat gamma_regularized_q(TCFloat a, TCFloat x);

    TCFloat gamma_regularized_p_inv(TCFloat a, TCFloat x);
    TCDouble gamma_regularized_p_inv(TCDouble a, TCDouble x);

    TCFloat gamma_regularized_q_inv(TCFloat a, TCFloat x);
    TCDouble gamma_regularized_q_inv(TCDouble a, TCDouble x);

    TCFloat expm1(TCFloat x);
    TCDouble expm1(TCDouble x);

    TCFloat log1p(TCFloat x);
    TCDouble log1p(TCDouble x);

    TCFloat erf(TCFloat x);
    TCDouble erf(TCDouble x);

    TCFloat erfc(TCFloat x);
    TCDouble erfc(TCDouble x);

    TCFloat erfinv(TCFloat x);
    TCDouble erfinv(TCDouble x);

    TCFloat erfcinv(TCFloat x);
    TCDouble erfcinv(TCDouble x);

    TCFloat floor(TCFloat x);
    TCDouble floor(TCDouble x);

    TCFloat ceil(TCFloat x);
    TCDouble ceil(TCDouble x);

    TCFloat isinf(TCFloat x);
    TCDouble isinf(TCDouble x);

    TCFloat isnan(TCFloat x);
    TCDouble isnan(TCDouble x);

    TCErrorCode  redop_s_sum(TCSize_t numelt, TCBool isref, TCDouble* data, TCDouble* y);
    TCErrorCode  redop_s_sum(TCSize_t numelt, TCBool isref, TCFloat* data, TCFloat* y);
    TCErrorCode  redop_s_sum(TCSize_t numelt, TCBool isref, TCFComplex* data, TCFComplex* y);
    TCErrorCode  redop_s_sum(TCSize_t numelt, TCBool isref, TCDComplex* data, TCDComplex* y);

    template <typename T>
    TCErrorCode redop_sum(const TCArray* in_arrays, TCSize_t narrays, TCSize_t dim, TCArray** out_array);

    TCErrorCode  redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCFloat* datax, TCFloat* datay,
        TCFloat valuex, TCFloat valuey, TCFloat* y);
    TCErrorCode  redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCDouble* datax, TCDouble* datay,
        TCDouble valuex, TCDouble valuey, TCDouble* y);
    TCErrorCode  redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCFComplex* datax, TCFComplex* datay,
        TCFComplex valuex, TCFComplex valuey, TCFComplex* y);
    TCErrorCode  redop_s_sum2sqr(TCSize_t numelt, TCBool isref, TCDComplex* datax, TCDComplex* datay,
        TCDComplex valuex, TCDComplex valuey, TCDComplex* y);


    template<typename T> T infinity();

    template<> TCFloat infinity()
    {
        __int32 K = 0x7f800000;
        return *(reinterpret_cast<TCFloat *>(&K));
    }

    template<> TCDouble infinity()
    {
        __int64 K = 0x7ff0000000000000;
        return *(reinterpret_cast<TCDouble *>(&K));
    }

    template<typename T> T quiet_NaN(const TCErrorCode& ec=TCError_NoError);

    template<> TCFloat quiet_NaN(const TCErrorCode& ec)
    {
        __int32 K = 0x7fc00000 | ec;
        return *(reinterpret_cast<TCFloat *>(&K));
    }

    template<> TCDouble quiet_NaN(const TCErrorCode& ec)
    {
        __int64 K = 0x7ff8000000000000 | ec;
        return *(reinterpret_cast<TCDouble *>(&K));
    }
}
