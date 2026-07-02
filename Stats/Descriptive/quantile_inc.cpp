



#include "stdafx.h"

#include "qselect_utils.h"

#include "quantile_inc.h"

using namespace Descriptive;

TCErrorCode stats_redop_quantile_inc_f_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, const TCSize_t q, TCArray** value)
{
    if (q < 1)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
    }
    try
    {
        quantile_inc_t<TCFloat> op(q);
        return Sort::process2vector_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redop_quantile_inc_d_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, const TCSize_t q, TCArray** value)
{
    if (q < 1)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
    }
    try
    {
        quantile_inc_t<TCDouble> op(q);
        return Sort::process2vector_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redall_quantile_inc_f_e(TCArray* numbers, const TCSize_t q, TCArray* value)
{

	TCErrorCode err;
	err = stats_redop_quantile_inc_f_e(numbers, 1, 0, q, &value);
	return err;
}

TCErrorCode stats_reddim_quantile_inc_f_e(TCArray* numbers, TCSize_t dim, const TCSize_t q, TCArray* value)
{

	TCErrorCode err;
	err = stats_redop_quantile_inc_f_e(numbers, 1, dim, q, &value);
	return err;
}

TCErrorCode stats_redall_quantile_inc_d_e(TCArray* numbers, const TCSize_t q, TCArray* value)
{

	TCErrorCode err;
	err = stats_redop_quantile_inc_d_e(numbers, 1, 0, q, &value);
	return err;
}

TCErrorCode stats_reddim_quantile_inc_d_e(TCArray* numbers, TCSize_t dim, const TCSize_t q, TCArray* value)
{

	TCErrorCode err;
	err = stats_redop_quantile_inc_d_e(numbers, 1, dim, q, &value);
	return err;
}
