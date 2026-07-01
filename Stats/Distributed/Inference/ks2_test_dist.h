//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include <TCArrayUtilsInternal.h>
#include <TCCommonDist.h>
#include "../../Inference/ks2_test.h"
#include <TCMathDist.h>

namespace Inference
{
    template <typename T>
    static TCErrorCode math_sort_dist(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        static_assert("math_sort_sequential is specialized by wrong type.");
    }
    template <>
    static TCErrorCode math_sort_dist<TCFloat>(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        return math_sort_f_e_dist(in_array, ascend, dim, sorted_array);
    }
    template <>
    static TCErrorCode math_sort_dist<TCDouble>(const TCDistArray* in_array, TCBool ascend,
        TCSize_t dim, TCDistArray* sorted_array)
    {
        return math_sort_d_e_dist(in_array, ascend, dim, sorted_array);
    }

    template <typename T>
    TCErrorCode stats_inference_ks2_test_dist(TCDistArray* sample1, TCDistArray* sample2,
        TCHypothesis test, T* p_value, T* test_statistic)
    {
        TCSize_t n = TCDistArray_Get_Size(*sample1);
        TCSize_t m = TCDistArray_Get_Size(*sample2);
        if (n == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
        }
        if (m == 0)
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

        TCErrorCode res, res1, res2;
        TCErrorCode resOK = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        TCErrorCode resOOM = tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
        TCErrorCode resInt = tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
        TCSSize_t rank = TCDistArray_Get_Rank(*sample1);
        TCSSize_t master = TCDistArray_Get_MasterRank(*sample1);
        TCTypeTag tag = TCDistArray_Get_TypeTag(*sample1);

        res = math_sort_dist<T>(sample1, true, 1, sample1);
        // Assumption: sort result is the same on each node.
        // We use collective operations below, so we need to exit synchronously.
        if (res != resOK)
        {
            return resInt;
        }
        res = math_sort_dist<T>(sample2, true, 1, sample2);
        if (res != resOK)
        {
            return resInt;
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

            if ((res1 != resOK) || (res2 != resOK))
            {
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
        // We use collective operations below, so we need to exit synchronously.
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);
        if (res != resOK)
        {
            return resOOM;
        }

        res = TCDistArray_Gather(sample1, master, localArray1);
        // Assumption: TCDistArray_Gather result is the same on each node.
        // We use collective operations below, so we need to exit synchronously.
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray2);
                TCArray_Delete(&localArray1);
            }
            return resInt;
        }
        res = TCDistArray_Gather(sample2, master, localArray2);
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray2);
                TCArray_Delete(&localArray1);
            }
            return resInt;
        }

        if (rank == master)
        {
            T* data1 = (T*)localArray1->m_data;
            T* data2 = (T*)localArray2->m_data;

            res = skip_missings<T>(data1, n, 1);

            if (res == resOK)
            {
                res = skip_missings<T>(data2, m, 2);
            }

            if (res == resOK)
            {
                skip_ties<T>(data1, n);
                skip_ties<T>(data2, m);

                res = stats_inference_ks2_test<T>(data1, n, data2, m, test, p_value, test_statistic);
            }
        }
        // We use collective operations below, so we need to exit synchronously.
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

        // Assumption: function result must be the same on all ranks.
        MPI_Bcast(p_value, 1, mpi_traits<T>::mpi_type(), master, comm);
        MPI_Bcast(test_statistic, 1, mpi_traits<T>::mpi_type(), master, comm);

        if (rank == master)
        {
            TCArray_Delete(&localArray2);
            TCArray_Delete(&localArray1);
        }
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);

        return res;
    }
}
