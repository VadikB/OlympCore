



#include "stdafx.h"
#include "OlympCoreDist.h"
#include "TCTypes.h"
#include "../Distributed/Descriptive/percentrank_exc_dist.h"
#include "../Descriptive/qselect_utils.h"
#include "qselect_utils_dist.h"

using namespace Descriptive;

TCErrorCode stats_redop_percentrank_exc_f_e_dist(const TCDistArray* numbers, const TCFloat* value, TCSize_t dim, TCSize_t significance, TCDistArray** percentrank_value)
{
    try
    {
        percentrank_exc_dist_t<TCFloat> op(*value, significance);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}

TCErrorCode stats_redop_percentrank_exc_d_e_dist(const TCDistArray* numbers, const TCDouble* value, TCSize_t dim, TCSize_t significance, TCDistArray** percentrank_value)
{
    try
    {
        percentrank_exc_dist_t<TCDouble> op(*value, significance);
        return Sort::process_all_dist<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}
