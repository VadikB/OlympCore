//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <vector>

#include "TCError.h"
#include "../Descriptive/interquartile_dist.h"
#include "qselect_utils_dist.h"
#include "qselect_dist.h"


namespace Descriptive
{

template <typename T>
inline T interquartile_dist_dist(std::vector<T>& data, TCSize_t total_elem, MPI_Comm comm)
{
    T quartile1, quartile3;
    TCSize_t q1_pos = (total_elem - 1);
    TCSize_t q3_pos = 3 * (total_elem - 1);

    switch (q1_pos & 3)
    {
    case 0:
        quartile1 = Sort::quick_select_dist<T, false, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, q1_pos / 4, comm);
        break;
    case 1:
        quartile1 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,1,4>>(data, total_elem, q1_pos / 4, comm);
        break;
    case 2:
        quartile1 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, q1_pos / 4, comm);
        break;
    case 3:
        quartile1 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,3,4>>(data, total_elem, q1_pos / 4, comm);
        break;
    }
    switch (q3_pos & 3)
    {
    case 0:
        quartile3 = Sort::quick_select_dist<T, false, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, q3_pos / 4, comm);
        break;
    case 1:
        quartile3 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,1,4>>(data, total_elem, q3_pos / 4, comm);
        break;
    case 2:
        quartile3 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,1,2>>(data, total_elem, q3_pos / 4, comm);
        break;
    case 3:
        quartile3 = Sort::quick_select_dist<T, true, TCSize_t, interpolation_t<T,3,4>>(data, total_elem, q3_pos / 4, comm);
        break;
    }

    const T Inf = Utils::infinity<T>();

    if ((quartile3 == Inf && quartile1 == Inf) || (quartile3 == -Inf && quartile1 == -Inf))
    {
        return Utils::quiet_NaN<T>();
    }

    if ((quartile1 == -Inf) && (quartile3 == Inf || !isnan(quartile3)))
    {
        return Inf;
    }

    if (!isnan(quartile1) && quartile3 == Inf)
    {
        return Inf;
    }

    return quartile3 - quartile1;
}

template <typename T>
struct interquartile_dist_dist_t : public interquartile_dist_t<T>, Sort::operator_qselect_dist_t
{
    inline TCErrorCode GetResultDist(T_state& state, TCSize_t total_elem, MPI_Comm comm, T& result)
    {
        result = interquartile_dist_dist<T>(state, total_elem, comm);
        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
};


} /* namespace Descriptive */ 
