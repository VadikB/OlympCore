


#include "TCReduceDist.h"
#include "OlympCoreDist.h"
#include "regression_dist.h"

template<typename T>
TCErrorCode math_desc_intercept(TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
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
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0), &count,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);

    if(number(count) < 2)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
    }

    reduce2arrays_op(sum2_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0), &mean_x,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);
    mean_x /= count;

    reduce2arrays_op(sum2_kahan_t<T>(), x_local->m_numelt, 
        (T*)y_local->m_data, (T*)x_local->m_data, 
        tc_static_cast<T>(0), tc_static_cast<T>(0), tc_static_cast<T>(0), &mean_y,
        (MPI_Comm)known_xs->m_Layout.m_Comm, ALL_RANKS, MPI_SUM);
    mean_y /= count;

    T xy, xx;

    reduce2arrays_op(sum2sqr_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        mean_x, tc_static_cast<T>(0), tc_static_cast<T>(0), &xx,
        (MPI_Comm)known_xs->m_Layout.m_Comm, MASTER_RANK, MPI_SUM);

    reduce2arrays_op(sum2mult_kahan_t<T>(), x_local->m_numelt, 
        (T*)x_local->m_data, (T*)y_local->m_data, 
        mean_x, mean_y, tc_static_cast<T>(0), &xy,
        (MPI_Comm)known_xs->m_Layout.m_Comm, MASTER_RANK, MPI_SUM);
            
    
    if (rank == MASTER_RANK)
    {
        T ret;
        ret = mean_y - xy / xx * mean_x;

        ((T*)(output)->m_LocalArray.m_data)[0] = ret;
    }

    return TCError_NoError;
}

TCErrorCode math_desc_intercept_d_e_dist(TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_intercept<TCDouble>(known_ys, known_xs, output);
}

TCErrorCode math_desc_intercept_f_e_dist(TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_intercept<TCFloat>(known_ys, known_xs, output);
}

TCErrorCode math_desc_intercept_z_e_dist(TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_intercept<TCDComplex>(known_ys, known_xs, output);
}

TCErrorCode math_desc_intercept_c_e_dist(TCDistArray *known_ys, TCDistArray *known_xs, TCDistArray *output)
{
    return math_desc_intercept<TCFComplex>(known_ys, known_xs, output);
}
