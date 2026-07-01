//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "forecast.h"

template<typename T>
TCErrorCode  regression_forecast_main(const TCArray *x, const TCArray *known_xs, const TCArray *known_ys, TCArray *forecast)
{
    try
    {
        if((forecast->m_ndims != x->m_ndims) || (forecast->m_numelt != x->m_numelt))
        {
            return TCError_IllegalInput;
        }

        if(known_xs->m_ndims > 2 || known_ys->m_ndims > 2)
        {
            return TCError_IllegalInput;
        }

        TCSize_t rowsX, colsX, rowsY, colsY;
        SetRowsCols(rowsX, colsX, known_xs);
        SetRowsCols(rowsY, colsY, known_ys);

        if(rowsX * colsX != rowsY * colsY)
        {
            return TCError_IllegalInput;
        }

        TCBool isref = known_xs->m_isref || known_ys->m_isref;
        return regression_forecast<T>(x->m_numelt, known_xs->m_numelt, isref, 
            (T*) x->m_data,
            (T*) known_xs->m_data, (T*) known_ys->m_data,
            rowsX, colsX, rowsY, colsY, (T*)forecast->m_data);
    }
    catch(std::bad_alloc)
    {
        return TCError_OutOfMemory;
    }
    catch(...)
    {
        return TCError_Internal;
    }
}

TCErrorCode  regression_forecast_e(const TCArray *x, const TCArray *known_ys, const TCArray *known_xs, TCArray *forecast)
{
    switch(x->m_tag)
    {
    case TC_FLOAT:
        return regression_forecast_main<TCFloat>(x, known_xs, known_ys, forecast);
    case TC_DOUBLE:
        return regression_forecast_main<TCDouble>(x, known_xs, known_ys, forecast);
    case TC_FCOMPLEX:
        return regression_forecast_main<TCFComplex>(x, known_xs, known_ys, forecast);
    case TC_DCOMPLEX:
        return regression_forecast_main<TCDComplex>(x, known_xs, known_ys, forecast);
    }

    return TCError_Internal;
}

TCErrorCode  regression_forecast_d_e(const TCArray *x, const TCArray *known_ys, const TCArray *known_xs, TCArray *forecast)
{
    return regression_forecast_main<TCDouble>(x, known_xs, known_ys, forecast);
}
TCErrorCode  regression_forecast_f_e(const TCArray *x, const TCArray *known_ys, const TCArray *known_xs, TCArray *forecast)
{
    return regression_forecast_main<TCFloat>(x, known_xs, known_ys, forecast);
}
TCErrorCode  regression_forecast_z_e(const TCArray *x, const TCArray *known_ys, const TCArray *known_xs, TCArray *forecast)
{
    return regression_forecast_main<TCDComplex>(x, known_xs, known_ys, forecast);
}
TCErrorCode  regression_forecast_c_e(const TCArray *x, const TCArray *known_ys, const TCArray *known_xs, TCArray *forecast)
{
    return regression_forecast_main<TCFComplex>(x, known_xs, known_ys, forecast);
}
