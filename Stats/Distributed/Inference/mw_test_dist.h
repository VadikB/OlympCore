



#pragma once

#include "stdafx.h"
#include <TCArrayUtilsInternal.h>
#include <TCCommonDist.h>
#include "../../Inference/mw_test.h"
#include <TCMathDist.h>

namespace Inference
{
    template<typename T>
    static TCErrorCode math_sort_dist(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        static_assert(false, "mw_test math_sort is specialized by wrong type");
    }
    template<>
    static TCErrorCode math_sort_dist<TCFloat>(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        return math_sort_f_e_dist(in_array, ascend, dim, sorted_array);
    }
    template<>
    static TCErrorCode math_sort_dist<TCDouble>(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        return math_sort_d_e_dist(in_array, ascend, dim, sorted_array);
    }

    template<typename T>
    TCErrorCode stats_inference_mw_test_dist(TCDistArray* sample1, TCDistArray* sample2, 
        TCHypothesis testtype, T* p_value, T* u_statistic)
    {
        TCSize_t x_len = TCDistArray_Get_Size(*sample1);
        TCSize_t y_len = TCDistArray_Get_Size(*sample2);

        if (x_len == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
        }
        if (y_len == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(2));
        }

        TCSize_t ndims1 = TCDistArray_Get_NDims(*sample1);
        TCSize_t ndims2 = TCDistArray_Get_NDims(*sample2);
        if (ndims1 != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }
        if (ndims2 != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
        }

        TCErrorCode resInternal = tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
        TCErrorCode res;
        TCErrorCode res1;
        TCErrorCode res2;
        TCErrorCode resOK = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        TCSSize_t rank = TCDistArray_Get_Rank(*sample1);
        TCSSize_t master = TCDistArray_Get_MasterRank(*sample1);
        TCTypeTag tag = TCDistArray_Get_TypeTag(*sample1);

        res = math_sort_dist<T>(sample1, true, 1, sample1);
        if (res != resOK)
        {
            return resInternal;
        }
        res = math_sort_dist<T>(sample2, true, 1, sample2);
        if (res != resOK)
        {
            return resInternal;
        }

        TCSize_t* shape1;
        TCSize_t* shape2;
        TCDistArray_Get_Shape(sample1, &shape1);
        TCDistArray_Get_Shape(sample2, &shape2);

        MPI_Comm comm = (MPI_Comm)sample1->m_Layout.m_Comm;

        TCArray* localArray1 = NULL;
        TCArray* localArray2 = NULL;
        if (rank == master)
        {
            res1 = TCArray_New_ND(tag, ndims1, shape1, false, &localArray1);
            res2 = TCArray_New_ND(tag, ndims2, shape2, false, &localArray2);

            if (res1 != resOK || res2 != resOK)
            {
                res = tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
                if (res1 == resOK)
                {
                    TCArray_Delete(&localArray1);
                }
                if (res2 == resOK)
                {
                    TCArray_Delete(&localArray2);
                }
            }
        }
        
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);
        if (res != resOK)
        {
            return res;
        }

        res = TCDistArray_Gather(sample1, master, localArray1);
        
        
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray2);
                TCArray_Delete(&localArray1);
            }
            return resInternal;
        }
        res = TCDistArray_Gather(sample2, master, localArray2);
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray2);
                TCArray_Delete(&localArray1);
            }
            return resInternal;
        }

        if (rank == master)
        {
            T* xvec = (T*)localArray1->m_data;
            T* yvec = (T*)localArray2->m_data;

            res = mw_check_input<T>(xvec, x_len, 1);
            if (res == resOK)
            {
                res = mw_check_input<T>(yvec, y_len, 2);
            }
            if (res == resOK)
            {
                if (x_len <= y_len)
                {
                    res = stats_mw_test<T>(xvec, x_len, yvec, y_len, testtype, p_value, u_statistic);
                }
                else
                {
                    res = stats_mw_test<T>(yvec, y_len, xvec, x_len, testtype, p_value, u_statistic);
                }
            }
        }
        
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray2);
                TCArray_Delete(&localArray1);
            }
            return res;
        }

        
        MPI_Bcast(p_value, 1, mpi_traits<T>::mpi_type(), master, comm);
        MPI_Bcast(u_statistic, 1, mpi_traits<T>::mpi_type(), master, comm);

        if (rank == master)
        {
            TCArray_Delete(&localArray2);
            TCArray_Delete(&localArray1);
        }
        return resOK;
    }
}
