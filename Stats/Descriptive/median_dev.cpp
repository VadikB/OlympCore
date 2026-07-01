//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCArrayUtils.h"
#include <OlympCoreUtils.h>

#include "qselect_utils.h"

#include "median_dev.h"

using namespace Descriptive;

TCErrorCode stats_redop_median_dev_f_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** median_dev_value)
{
    try
    {
        median_dev_t<TCFloat> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_dev_value);
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

TCErrorCode stats_redop_median_dev_d_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** median_dev_value)
{
    try
    {
        median_dev_t<TCDouble> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, median_dev_value);
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

TCErrorCode stats_redall_median_dev_f_e(TCArray* numbers, TCSize_t narrays, TCFloat* median_dev_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_f(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_median_dev_f_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*median_dev_value = *(TCFloat*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_median_dev_f_e(TCArray* numbers, TCSize_t dim, TCArray* median_dev_value)
{

	TCErrorCode err;
	err = stats_redop_median_dev_f_e(numbers, 1, dim, &median_dev_value);
	return err;
}

TCErrorCode stats_redall_median_dev_d_e(TCArray* numbers, TCSize_t narrays, TCDouble* median_dev_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_d(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_median_dev_d_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*median_dev_value = *(TCDouble*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_median_dev_d_e(TCArray* numbers, TCSize_t dim, TCArray* median_dev_value)
{

	TCErrorCode err;
	err = stats_redop_median_dev_d_e(numbers, 1, dim, &median_dev_value);
	return err;
}
