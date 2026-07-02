



#include "stdafx.h"
#include "steyx.h"

template<typename T>
TCErrorCode  regression_steyx_main(const TCArray *known_xs, const TCArray *known_ys, T *steyx)
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
        return regression_steyx<T>(known_xs->m_numelt, isref, 
            (T*) known_xs->m_data, (T*) known_ys->m_data,
            rowsX, colsX, rowsY, colsY, steyx);
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

TCErrorCode  regression_steyx_f_e(const TCArray *known_ys, const TCArray *known_xs, TCFloat *steyx)
{
    return regression_steyx_main<TCFloat>(known_xs, known_ys, steyx);
}

TCErrorCode regression_steyx_d_e(const TCArray *known_ys, const TCArray *known_xs, TCDouble *steyx)
{
    return regression_steyx_main<TCDouble>(known_xs, known_ys, steyx);
}

TCErrorCode regression_steyx_c_e(const TCArray *known_ys, const TCArray *known_xs, TCFComplex *steyx)
{
    return regression_steyx_main<TCFComplex>(known_xs, known_ys, steyx);
}

TCErrorCode regression_steyx_z_e(const TCArray *known_ys, const TCArray *known_xs, TCDComplex *steyx)
{
    return regression_steyx_main<TCDComplex>(known_xs, known_ys, steyx);
}
