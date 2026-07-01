//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "mpi.h"


#pragma region MPITypeTraits
    template<typename T>
    class MPI_Type
    {
    public:
        inline static MPI_Datatype mpi_type() { ASSERT(0); return (MPI_Datatype)-1; }
    };

    template<>
    class MPI_Type<TCFloat>
    {
    public:
        inline static MPI_Datatype mpi_type() { return MPI_FLOAT; }
    };

    template<>
    class MPI_Type<TCDouble>
    {
    public:
        inline static MPI_Datatype mpi_type() { return MPI_DOUBLE; }
    };

#pragma endregion MPITypeTraits

#define MASTER_RANK     0
#define ALL_RANKS       -1

namespace DistUtils
{
    template <typename T>
    void GetGlobalSum(MPI_Comm comm, T *sum, int destRank = MASTER_RANK)
    {
        T sumGlobal;

        MPI_Reduce(sum, &sumGlobal, 1, MPI_Type<T>::mpi_type(), MPI_SUM, destRank, comm);

        *sum = sumGlobal;
    }
}