//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <vector>

#include "TCTypes.h"
#include "../Descriptive/percentrank_exc.h"
#include "qselect_utils_dist.h"

namespace Descriptive
{

template <typename T>
struct percentrank_exc_dist_t : public percentrank_exc_t<T>
{
    percentrank_exc_dist_t(T value, TCSize_t significance) : percentrank_exc_t(value, significance)
    {
    }

    template <class OP>
    inline TCErrorCode getDistributedResult(OP& op, T_state& state, TCSize_t &nelem, MPI_Comm comm, TCInt32 rank, TCInt32 destRank, OP::ou_t *out)
    {
        typedef OP::in_t T;

        TCInt32 total_ranks;
        MPI_Comm_size(comm, &total_ranks);

        TCInt32 masterRank = destRank == ALL_RANKS ? MASTER_RANK : destRank;
        if (rank == masterRank)
        {
            MPI_Status status;
            for (int i = 0; i < total_ranks; i++)
            {
                if (i == masterRank)
                {
                    continue;
                }
                int sz = 0;
                MPI_Recv(&sz, 1, MPI_INT, i, 1, comm, &status);
                T_state state2;
                MPI_Recv(&state2, sizeof(state), MPI_BYTE, i, 2, comm, &status);

                // operator specific
                nelem += sz;
                if (sz > 0)
                {
                    if (state.error == 0)
                    {
                        state.error = state2.error;
                        if (state.error == 0)
                        {
                            state.below_count += state2.below_count;
                            state.above_count += state2.above_count;
                            if (!TCError::is_qnan(state2.below_value))
                            {
                                T x = state2.below_value;
                                if (TCError::is_qnan(state.below_value) || x > state.below_value)
                                {
                                    state.below_value = x;
                                }
                            }
                            if (!TCError::is_qnan(state2.above_value))
                            {
                                T x = state2.above_value;
                                if (TCError::is_qnan(state.above_value) || x < state.above_value)
                                {
                                    state.above_value = x;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!(state2.error == 0) && i < masterRank)
                        {
                            // proparage "the first error"
                            state.error = state2.error;
                        }
                    }
                }
            }
        }
        else
        {
            int sz = (int)nelem;
            MPI_Send(&sz, 1, MPI_INT, masterRank, 1, comm);
            MPI_Send(&state, sizeof(state), MPI_BYTE, masterRank, 2, comm);
            nelem = -1;
        }
        TCErrorCode err = 0;
        if (rank == masterRank)
        {
            err = op.GetResult(state, nelem, *out);
        }
        if (destRank == ALL_RANKS)
        {
            _ASSERT(0);
            MPI_Bcast(out, 1, mpi_traits<T>::mpi_type(), masterRank, comm);
        }
        MPI_Bcast(&err, 1, MPI_UNSIGNED_SHORT, masterRank, comm);
        return err;
    }
};

} /* namespace Descriptive */ 
