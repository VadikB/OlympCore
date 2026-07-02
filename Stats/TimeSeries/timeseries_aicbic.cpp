



#include "stdafx.h"
#include "TCArrayUtils.h"
#include "TCErrorCommon.h"
#include "TimeSeriesUtils.h"

template <typename T>
TCErrorCode aicbic_body(T loss, TCUInt32 p, TCSize_t T0, TSEstimationMethod method, T* aic, T* bic)
{
    if (!((method == MaxLikelihood) || (method == LeastSquare) || (method == ForwardBackward) || (method == YuleWalker)))
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }
    if (TCError::IsMissingImpl<T>::run(&loss))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (loss < 0 && ((method == LeastSquare) || (method == ForwardBackward) || (method == YuleWalker)))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (0 == T0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }
    if (TCError::is_nan<T>(loss))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (loss == -std::numeric_limits<T>::infinity() && ((method == LeastSquare) || (method == ForwardBackward) || (method == YuleWalker)))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    switch (method)
    {
    case MaxLikelihood:
        if (NULL != aic)
        {
            *aic = 2. * p - 2. * loss;
        }
        if (NULL != bic)
        {
            *bic = p * log((T)T0) - 2. * loss;
        }
        break;

    case LeastSquare:
    case ForwardBackward:
    case YuleWalker:
        if (NULL != aic)
        {
            *aic = 2. * p + (T)T0 * log(loss / (T)T0);
        }
        if (NULL != bic)
        {
            *bic = ((T)p - T0)* log((T)T0) + (T)T0 * log(loss);
        }
        break;
    }
    return TCError_NoError;
}

TCErrorCode stats_timeseries_aicbic_d_e(TCDouble loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method, TCDouble* aic, TCDouble* bic)
{
    return aicbic_body<TCDouble>(loss, p, T, method, aic, bic);
}
TCErrorCode stats_timeseries_aicbic_f_e(TCFloat loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method, TCFloat* aic, TCFloat* bic)
{
    return aicbic_body<TCFloat>(loss, p, T, method, aic, bic);
}

TCErrorCode stats_timeseries_aic_d_e(TCDouble loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method,  TCDouble* aic)
{
    return aicbic_body<TCDouble>(loss, p, T, method, aic, NULL);
}
TCErrorCode stats_timeseries_aic_f_e(TCFloat loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method, TCFloat* aic)
{
    return aicbic_body<TCFloat>(loss, p, T, method, aic, NULL);
}

TCErrorCode stats_timeseries_bic_d_e(TCDouble loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method,  TCDouble* bic)
{
    return aicbic_body<TCDouble>(loss, p, T, method, NULL, bic);
}
TCErrorCode stats_timeseries_bic_f_e(TCFloat loss, TCUInt32 p, TCSize_t T, TSEstimationMethod method, TCFloat* bic)
{
    return aicbic_body<TCFloat>(loss, p, T, method, NULL, bic);
}
