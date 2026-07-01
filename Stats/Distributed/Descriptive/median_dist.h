//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <vector>

#include "TCError.h"
#include "../Descriptive/median.h"
#include "qselect_utils_dist.h"
#include "qselect_dist.h"

namespace Descriptive
{

template <typename T>
inline T median_dist(std::vector<T>& data, TCSize_t total_elem, MPI_Comm comm)
{
    TCSize_t k = (total_elem - 1) / 2;

    // call different versions of quickselect for odd- and even- length arrays
    if (total_elem & 1)
    {
        return Sort::quick_select_dist<T, false, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, k, comm);
    }
    else
    {
        return Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, k, comm);
    }
}

template <typename T>
struct median_dist_t : public median_t<T>, Sort::operator_qselect_dist_t
{
    inline TCErrorCode GetResultDist(T_state& state, TCSize_t total_elem, MPI_Comm comm, T& result)
    {
        TCInt32 myRank;
        MPI_Comm_rank(comm, &myRank);

        // check for meta-numeric values
        TCInt32 errorRank = -1;
        T v;
        for (TCSize_t i = 0; i < state.size(); i++)
        {
            v = state[i];
            // don't allow qnans to qsel, missings already filtered in the outer loop
            if(TCError::IsErrorImpl<T>::run(&v))
            {
                errorRank = myRank;
                break;
            }
        }

        TCInt32 errorRankToUse = -1;
        MPI_Allreduce(&errorRank, &errorRankToUse, 1, mpi_traits<TCInt32>::mpi_type(), MPI_MAX, comm);
        if (-1 == errorRankToUse)
        {
            result = median_dist<T>(state, total_elem, comm);
        }
        else // error was found at least on one rank
        {
            MPI_Bcast(&v, 1, mpi_traits<T>::mpi_type(), errorRankToUse, comm);
            result = v; // propagate error
        }

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};


} /* namespace Descriptive */ 
