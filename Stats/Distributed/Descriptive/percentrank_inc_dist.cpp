//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "OlympCoreDist.h"
#include "percentrank_inc_dist.h"

using namespace Descriptive;

TCErrorCode stats_redop_percentrank_inc_f_e_dist(const TCDistArray* numbers, const TCFloat* value, TCSize_t dim, TCSize_t significance, TCDistArray** percentrank_value)
{
    try
    {
        percentrank_inc_dist_t<TCFloat> op(*value, significance);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
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

TCErrorCode stats_redop_percentrank_inc_d_e_dist(const TCDistArray* numbers, const TCDouble* value, TCSize_t dim, TCSize_t significance, TCDistArray** percentrank_value)
{
    try
    {
        percentrank_inc_dist_t<TCDouble> op(*value, significance);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
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
