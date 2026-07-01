//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "slope.h"

template<typename T>
TCErrorCode  regression_slope_main(const TCArray *known_xs, const TCArray *known_ys, T *slope)
{
    try
    {
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
        return regression_slope<T>(known_xs->m_numelt, isref, 
            (T*) known_xs->m_data, (T*) known_ys->m_data,
            rowsX, colsX, rowsY, colsY, slope);
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

TCErrorCode  regression_slope_f_e(const TCArray *known_ys, const TCArray *known_xs, TCFloat *slope)
{
    return regression_slope_main<TCFloat>(known_xs, known_ys, slope);
}

TCErrorCode regression_slope_d_e(const TCArray *known_ys, const TCArray *known_xs, TCDouble *slope)
{
    return regression_slope_main<TCDouble>(known_xs, known_ys, slope);
}

TCErrorCode regression_slope_c_e(const TCArray *known_ys, const TCArray *known_xs, TCFComplex *slope)
{
    return regression_slope_main<TCFComplex>(known_xs, known_ys, slope);
}

TCErrorCode regression_slope_z_e(const TCArray *known_ys, const TCArray *known_xs, TCDComplex *slope)
{
    return regression_slope_main<TCDComplex>(known_xs, known_ys, slope);
}
