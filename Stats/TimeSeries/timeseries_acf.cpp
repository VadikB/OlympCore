//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "TCArrayUtils.h"
#include "TCArrayWrapper.h"
#include "TCErrorCommon.h"
#include "TimeSeriesUtils.h"
#include "ipp.h"

#define CHECKIPPSTATUS(expr) if ((expr)!=ippStsNoErr) { return tcerror_code_new(TCError_Internal,TC_UNSPECIFIED_ARGID);}

template <typename T>
TCErrorCode acf_body(In TCArray* y_, TCArray* r, TCArray* stddev, T* q, T* mq)
{
    TCSize_t i;
    TCSize_t m;
    using namespace TCArrayWrapperUtils;
    TCErrorCode noError = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);

    TCArrayWrapper yWrapper(y_, 1, passIn, layoutContiguousFirstDimension);
    TCArray* y;
    TCErrorCode errorCode = yWrapper.GetTCArray(&y);
    if (errorCode != noError)
    {
        return errorCode;
    }
#if 0
    TCArrayWrapper rWrapper(r_, 2, passOut, layoutContiguousFirstDimension);
    TCArray* r;
    errorCode = yWrapper.GetTCArray(&r);
    if (errorCode != noError)
    {
        return errorCode;
    }
    TCArrayWrapper stddevWrapper(stddev_, 3, passOut, layoutContiguousFirstDimension);
    TCArray* stddev;
    errorCode = stddevWrapper.GetTCArray(&stddev);
    if (errorCode != noError)
    {
        return errorCode;
    }
#endif
    TCSize_t ny = y->m_numelt;
    TCSize_t nr = r->m_numelt;
    T  tref;
//    IppStatus ippstatus;
    T* py = ((T*)y->m_data);
    T* pr = ((T*)r->m_data);
    T* pdev = ((T*)stddev->m_data);
    TCTypeTag tag = y->m_tag;
    TCBool isempty;
    array_isempty(y, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    array_isempty(r, &isempty);
    if (isempty) 
    {
        return TCError_NoError;
    }
    array_is1D(y, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    array_is1D(r, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }
    // count leading missings
    m = 0;
    for (i = 0; i < ny; i++)
    {
        tref = py[i];
        if (TCError::IsMissingImpl<T>::run(&tref) ) m++;
    }
    py += m;
    ny -= m;
    if (ny - m < nr )
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
#if 0
    if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>(py, ny, 1))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
#endif
    T exp = 0.;
    for (i=0; i < ny; i++)
    {
        exp += py[i];
    }
    exp /= ny;
    for (i=0; i < ny; i++)
    {
        py[i] -= exp;
    }
    switch (tag)
    {
    case TC_FLOAT:
        CHECKIPPSTATUS(ippsAutoCorr_32f((Ipp32f*)py, (int)ny, (Ipp32f*)pr, (int)nr));
        break;
    case TC_DOUBLE:
        CHECKIPPSTATUS(ippsAutoCorr_64f((Ipp64f*)py, (int)ny, (Ipp64f*)pr, (int)nr));
        break;
    }
    for (i = 0; i < nr; i++)
    {
        pr[nr-i-1] = pr[nr-i-1]/pr[0];
    }
    T dCum = 1.;
    pdev[0] = 1./sqrt((T)ny);
    for (i = 1; i < nr; i++)
    {
        dCum += 2. * pr[i] * pr[i];
        pdev[i] = pdev[0] * sqrt(dCum);
    }
    dCum = (dCum - 1.) / 2.;
    if (NULL != q)
    {
        *q = dCum * ny;
    }
    if (NULL != mq)
    {
        dCum = 0.;
        for (i = 1; i < nr; i++)
        {
            dCum += pr[i] * pr[i] / ((T)ny - i);
        }
        *mq = ny * (ny + 2) * dCum;
    }
    return TCError_NoError;
}

TCErrorCode stats_timeseries_acf_d_e(In TCArray* y, TCArray* r, TCArray* stddev)
{
    return acf_body<TCDouble>(y, r, stddev, NULL, NULL);
}

TCErrorCode stats_timeseries_acf_f_e(In TCArray* y, TCArray* r, TCArray* stddev)
{
    return acf_body<TCFloat>(y, r, stddev, NULL, NULL);
}

TCErrorCode stats_timeseries_acf_qstat_d_e(In TCArray* y, TCArray* r, TCArray* stddev, TCDouble* q, TCDouble* mq)
{
    return acf_body<TCDouble>((TCArray*)y, r, stddev, q, mq);
}

TCErrorCode stats_timeseries_acf_qstat_f_e(In TCArray* y, TCArray* r, TCArray* stddev, TCFloat* q, TCFloat* mq)
{
    return acf_body<TCFloat>((TCArray*)y, r, stddev, q, mq);
}
