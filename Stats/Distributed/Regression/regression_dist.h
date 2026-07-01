//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

void GetRankWithLocal(TCDistArray *known, TCSize_t *rank, TCSize_t *ir);
MPI_Datatype GetType(TCDistArray *known_xs);

template<typename T>
TCErrorID reshape(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<T> &tmpMem, TCSize_t rank);

TCSize_t number(TCDouble num);
TCSize_t number(TCFloat num);
TCSize_t number(TCDComplex num);
TCSize_t number(TCFComplex num);
