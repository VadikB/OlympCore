//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "intercept.h"

template<typename T>
TCErrorCode  regression_intercept_main(const TCArray *known_xs, const TCArray *known_ys, T *intercept)
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
        return regression_intercept<T>(known_xs->m_numelt, isref, 
            (T*) known_xs->m_data, (T*) known_ys->m_data,
            rowsX, colsX, rowsY, colsY, intercept);
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

TCErrorCode  regression_intercept_f_e(const TCArray *known_ys, const TCArray *known_xs, TCFloat *intercept)
{
    return regression_intercept_main<TCFloat>(known_xs, known_ys, intercept);
}

TCErrorCode regression_intercept_d_e(const TCArray *known_ys, const TCArray *known_xs, TCDouble *intercept)
{
    return regression_intercept_main<TCDouble>(known_xs, known_ys, intercept);
}

TCErrorCode regression_intercept_c_e(const TCArray *known_ys, const TCArray *known_xs, TCFComplex *intercept)
{
    return regression_intercept_main<TCFComplex>(known_xs, known_ys, intercept);
}

TCErrorCode regression_intercept_z_e(const TCArray *known_ys, const TCArray *known_xs, TCDComplex *intercept)
{
    return regression_intercept_main<TCDComplex>(known_xs, known_ys, intercept);
}
