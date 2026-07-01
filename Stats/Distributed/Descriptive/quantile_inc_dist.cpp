//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "OlympCoreDist.h"

#include "TCDistArray.h"
#include "TCTypes.h"
#include "TCError.h"

#include "../Distributed/Descriptive/quantile_inc_dist.h"
#include "../Distributed/Descriptive/qselect_utils_dist.h"

using namespace Descriptive;

TCErrorCode stats_redop_quantile_inc_f_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, const TCSize_t q, TCDistArray** value)
{
    if (q < 1)
    {
        return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
    }
    try
    {
        quantile_inc_dist_t<TCFloat> op(q);
        return Sort::process2vector_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redop_quantile_inc_d_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, const TCSize_t q, TCDistArray** value)
{
    if (q < 1)
    {
        return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
    }
    try
    {
        quantile_inc_dist_t<TCDouble> op(q);
        return Sort::process2vector_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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