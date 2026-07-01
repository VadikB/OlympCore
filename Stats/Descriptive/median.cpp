//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCArrayUtils.h"
#include "qselect_utils.h"

#include "median.h"

using namespace Descriptive;

TCErrorCode stats_redop_median_f_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** median_value)
{
    try
    {
        median_t<TCFloat> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_value);
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

TCErrorCode stats_redop_median_d_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** median_value)
{
    try
    {
        median_t<TCDouble> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_value);
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

TCErrorCode stats_redall_median_f_e(TCArray* numbers, TCSize_t narrays, TCFloat* median_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_f(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_median_f_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*median_value = *(TCFloat*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_median_f_e(TCArray* numbers, TCSize_t dim, TCArray* median_value)
{

	TCErrorCode err;
	err = stats_redop_median_f_e(numbers, 1, dim, &median_value);
	return err;
}

TCErrorCode stats_redall_median_d_e(TCArray* numbers, TCSize_t narrays, TCDouble* median_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_d(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_median_d_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*median_value = *(TCDouble*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_median_d_e(TCArray* numbers, TCSize_t dim, TCArray* median_value)
{

	TCErrorCode err;
	err = stats_redop_median_d_e(numbers, 1, dim, &median_value);
	return err;
}
