



#include "stdafx.h"
#include "TCArrayUtils.h"
#include <TCErrorCommon.h>
#include <vector>
#include <algorithm>
#include "TimeSeriesUtils.h"

template <typename T>
TCErrorCode pacf_body(const TCArray* y, TCArray* f)
{
#if 0
    T teta;
    T temp;
    TCSize_t i;
    TCSize_t j;
    TCSize_t k;
    TCSize_t nT= y->m_numelt;
    TCSize_t n1;
    TCSize_t n2;
    TCSize_t nr;
    TCSize_t mem2alloc;
    TCSize_t tau = forecast->m_numelt;
    TCSize_t p = ar->m_numelt;
    TCSize_t q = ma->m_numelt;
    TCBool isempty;
    array_isempty(y, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    array_isempty(e, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }
    array_isempty(forecast, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 7);
    }
    array_isempty(stddev, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 8);
    }
    array_is1D(y, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    array_is1D(e, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }
    array_is1D(forecast, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 7);
    }
    array_is1D(stddev, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 8);
    }
    array_is1D(ma, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 4);
    }
    array_is1D(ar, &isempty);
    if (!isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 3);
    }
    if (nT < std::max(p + d, q) )
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (y->m_numelt != e->m_numelt)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (forecast->m_numelt != stddev->m_numelt)
    {
        return tcerror_code_new(TCError_IllegalSize, 7);
    }
    if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>((TCArray*)y))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>((TCArray*)e))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>((TCArray*)ma))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>((TCArray*)ar))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }
    mem2alloc = (2 >  p + d + 1) ? 2 : p + d + 1;
    std::vector<T> oper(mem2alloc);
    std::vector<T> operTemp(mem2alloc);
    std::vector<T> work(mem2alloc);
    std::vector<T> psi(tau);
    std::vector<T> ye(p + d + tau);
    std::vector<T> ee(q + tau);
    for (i = 0; i < p + d; i++)
    {
        ye[i] = ((T*)y->m_data)[nT - p - d + i];
    }
    for (i = 0; i < q; i++)
    {
        ee[i] = ((T*)e->m_data)[nT - q + i];
    }

    work[0] = 1.;
    work[1] = -1.;
    operTemp[0] = 1.;
    oper[0] = 1.;
    n1 = 2;
    n2 = 1;
    for (i = 0; i < d; i++)
    {
        nr = operatorMultiply(&work, &operTemp, &oper, n1, n2);
        n2 = nr;
        for (j = 0; j < nr; j++)
        {
            operTemp[j] = oper[j];
        }
    }
    for (i = 0; i < p; i++)
    {
        work[i + 1] = -((T*)ar->m_data)[i];
    }
    nr = operatorMultiply(&operTemp, &work, &oper, nr, p + 1);
    psi[0] = 1.;
    for (j = 1; j < tau; j++)
    { 
        if (j > q)
        {
            teta = 0.;
        }
        else
        {
            teta = ((T*)ma->m_data)[j - 1];
        }
        k = (j < p + d) ? j : p + d;
        psi[j] = 0.;
        for(i = 1; i <= k; i++)
        {
            psi[j] -= psi[j - i] * oper[i];
        }
        psi[j] -= teta;
    }
    for (j = 1; j <= tau; j++)
    {
        temp = 1.;
        for (i = 0; i < j - 1; i++)
        {
            temp += psi[i + 1] * psi[i + 1];
        }
        ((T*)stddev->m_data)[j - 1] = sqrt(temp) * sigmae;
    }
    for (j = 0; j < tau; j++)
    {
        ye[p + d + j] = 0.;
        for (i = 1; i <= p + d; i++)
        {
            ye[p + d + j] += -oper[i] * ye[p + d + j - i];
        }
        for (i = 1; i <= q; i++)
        {
            ye[p + d + j] -= ((T*)ma->m_data)[i - 1] * ee[q + j - i];
        }
        ((T*)forecast->m_data)[j] = ye[p + d + j];
    }
    return TCError_NoError;
#endif
}

C_BEGINF(stats_timeseries_pacf_d_e, const TCArray* y, TCArray* f)
{
    return pacf_body<TCDouble>(y, f);
} C_ENDF

C_BEGINF(stats_timeseries_pacf_f_e, const TCArray* y, TCArray* f)
{
    return pacf_body<TCFloat>(y, f);
} C_ENDF
