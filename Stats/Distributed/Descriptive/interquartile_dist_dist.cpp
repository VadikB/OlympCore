//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "OlympCoreDist.h"
#include "../../Distributed/Descriptive\interquartile_dist_dist.h"
#include "../../Descriptive/qselect_utils.h"
#include "qselect_utils_dist.h"

using namespace Descriptive;

OLYMPCORE_API TCErrorCode stats_redop_interquartile_dist_f_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** dist)
{
    try
    {
        interquartile_dist_dist_t<TCFloat> op;
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, dist);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}

OLYMPCORE_API TCErrorCode stats_redop_interquartile_dist_d_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** dist)
{
    try
    {
        interquartile_dist_dist_t<TCDouble> op;
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, dist);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
    }
}
