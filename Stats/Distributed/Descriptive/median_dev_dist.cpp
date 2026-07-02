



#include "stdafx.h"
#include "OlympCoreDist.h"
#include "../Distributed/Descriptive\median_dev_dist.h"
#include "../Descriptive/qselect_utils.h"
#include "qselect_utils_dist.h"

using namespace Descriptive;

TCErrorCode stats_redop_median_dev_f_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** median_dev_value)
{
    try
    {
        median_dev_dist_t<TCFloat> op;
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_dev_value);
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

TCErrorCode stats_redop_median_dev_d_e_dist(const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** median_dev_value)
{
    try
    {
        median_dev_dist_t<TCDouble> op;
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_dev_value);
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