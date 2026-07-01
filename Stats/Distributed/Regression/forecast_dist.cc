//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>
#include "TCReduceDist.h"
#include "OlympCoreDist.h"
#include "regression_dist.h"

template<typename T>
TCErrorCode math_desc_forecast(TCDistArray *x, TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    std::vector<T> tmpMem;

    TCInt32 rank;
    rank = TCDistArray_Get_Rank(*known_ys);

    TCDistArray known_zs;
    known_zs.m_LocalArray.m_data = known_ys->m_LocalArray.m_data;
    TCErrorID ret = reshape<T>(known_xs, known_ys, &known_zs, tmpMem, rank);
    if(ret != TCError_NoError)
    {
        return tcerror_code_new(ret, TCArgumentID(0));
    }

    if(known_ys->m_Layout.m_GlobalSize != known_xs->m_Layout.m_GlobalSize)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(0));
    }

    TCArray *y_local = &(known_zs.m_LocalArray);
    TCArray *x_local = &(known_xs->m_LocalArray);

    T mean_x, mean_y, count;

    reduce2arrays_op(count_t<T>(), x_local->m_numelt,
        (T*)x_local->m_data, (T*)y_local->m_data, 
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0)/*value_init*/, &count,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);

    if(number(count) < 2)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
    }

    reduce2arrays_op(sum2_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0)/*value_init*/, &mean_x,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);
    mean_x /= count;

    reduce2arrays_op(sum2_kahan_t<T>(), x_local->m_numelt, 
        (T*)y_local->m_data, (T*)x_local->m_data, 
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0)/*value_init*/, &mean_y,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);
    mean_y /= count;

    T xy, xx;

    reduce2arrays_op(sum2sqr_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        mean_x, tc_static_cast<T>(0), tc_static_cast<T>(0)/*value_init*/, &xx,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);

    reduce2arrays_op(sum2mult_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        mean_x, mean_y, tc_static_cast<T>(0)/*value_init*/, &xy,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);
            
    T a, b;
    b = xy / xx;
    a = mean_y - b * mean_x;

    for(TCSize_t i = 0; i < x->m_LocalArray.m_numelt; i++)
    {
        ((T*)output->m_LocalArray.m_data)[i] = a + b * ((T*)x->m_LocalArray.m_data)[i];
    }

    return TCError_NoError;
}

TCErrorCode math_desc_forecast_d_e_dist(TCDistArray *x, TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_forecast<TCDouble>(x, known_ys, known_xs, output);
}

TCErrorCode math_desc_forecast_f_e_dist(TCDistArray *x, TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_forecast<TCFloat>(x, known_ys, known_xs, output);
}

TCErrorCode math_desc_forecast_z_e_dist(TCDistArray *x, TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_forecast<TCDComplex>(x, known_ys, known_xs, output);
}

TCErrorCode math_desc_forecast_c_e_dist(TCDistArray *x, TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_forecast<TCFComplex>(x, known_ys, known_xs, output);
}
