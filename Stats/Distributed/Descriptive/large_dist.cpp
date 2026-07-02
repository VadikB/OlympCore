



#include "stdafx.h"
#include "OlympCoreDist.h"
#include "../Distributed/Descriptive\large_dist.h"
#include "../Descriptive/qselect_utils.h"
#include "qselect_utils_dist.h"

using namespace Descriptive;

TCErrorCode stats_redop_large_f_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t k, TCSize_t dim, TCDistArray** value)
{
    try
    {
        large_dist_t<TCFloat> op(k);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redop_large_d_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t k, TCSize_t dim, TCDistArray** value)
{
    try
    {
        large_dist_t<TCDouble> op(k);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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