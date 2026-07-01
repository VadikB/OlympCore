//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include <TCArrayUtilsInternal.h>
#include <TCCommonDist.h>
#include "../../Inference/ks_test.h"
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
    TCErrorCode stats_inference_ks_test_dist(TCDistArray* sample, TCHypothesis test,
        T* p_value, T* test_statistic)
    {
        TCSize_t n = TCDistArray_Get_Size(*sample);
        if (n == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(1));
        }

        TCSize_t ndims = TCDistArray_Get_NDims(*sample);
        if (ndims != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
        }

        TCErrorCode res;
        TCErrorCode resOK = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        TCSSize_t rank = TCDistArray_Get_Rank(*sample);
        TCSSize_t master = TCDistArray_Get_MasterRank(*sample);
        TCTypeTag tag = TCDistArray_Get_TypeTag(*sample);

        res = math_sort_dist<T>(sample, true, 1, sample);
        // Assumption: sort result is the same on each node.
        // We use collective operations below, so we need to exit synchronously.
        if (res != resOK)
        {
            return res;
        }

        TCSize_t* shape;
        TCDistArray_Get_Shape(sample, &shape);

        MPI_Comm comm = (MPI_Comm)sample->m_Layout.m_Comm;

        TCArray* localArray = NULL;
        if (rank == master)
        {
            res = TCArray_New_ND(tag, ndims, shape, false, &localArray);
        }
        // We use collective operations below, so we need to exit synchronously.
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);
        if (res != resOK)
        {
            return res;
        }

        res = TCDistArray_Gather(sample, master, localArray);
        // Assumption: TCDistArray_Gather result is the same on each node.
        // We use collective operations below, so we need to exit synchronously.
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray);
            }
            return res;
        }

        if (rank == master)
        {
            T* data = (T*)localArray->m_data;

            res = skip_missings<T>(data, n, 1);

            if (res == resOK)
            {
                skip_ties<T>(data, n);

                res = stats_inference_ks_test<T>(data, n, test, p_value, test_statistic);
            }
        }
        // We use collective operations below, so we need to exit synchronously.
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);
        if (res != resOK)
        {
            if (rank == master)
            {
                TCArray_Delete(&localArray);
            }
            return res;
        }

        // Assumption: function result must be the same on all ranks.
        MPI_Bcast(p_value, 1, mpi_traits<T>::mpi_type(), master, comm);
        MPI_Bcast(test_statistic, 1, mpi_traits<T>::mpi_type(), master, comm);

        if (rank == master)
        {
            res = TCArray_Delete(&localArray);
        }
        MPI_Bcast(&res, 1, MPI_UNSIGNED_SHORT, master, comm);

        return res;
    }
}
