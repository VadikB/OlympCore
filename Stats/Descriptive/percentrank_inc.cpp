//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCArrayUtils.h"
#include "qselect_utils.h"
#include "qselect.h"
#include "percentrank_inc.h"

using namespace Descriptive;

TCErrorCode stats_redop_percentrank_inc_f_e(const TCArray* numbers, const TCFloat* value, TCSize_t dim, TCSize_t significance, TCArray** percentrank_value)
{
    try
    {
        percentrank_inc_t<TCFloat> op(*value, significance);
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
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

TCErrorCode stats_redop_percentrank_inc_d_e(const TCArray* numbers, const TCDouble* value, TCSize_t dim, TCSize_t significance, TCArray** percentrank_value)
{
    try
    {
        percentrank_inc_t<TCDouble> op(*value, significance);
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, 1, dim, percentrank_value);
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

TCErrorCode stats_redall_percentrank_inc_f_e(const TCArray* numbers, const TCFloat value, TCSize_t significance, TCFloat* percentrank_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_f(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_percentrank_inc_f_e(numbers, &value, 0, significance, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*percentrank_value = *(TCFloat*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_percentrank_inc_f_e(const TCArray* numbers, TCSize_t dim, const TCFloat value, TCSize_t significance, TCArray* p_array)
{

	TCErrorCode err;
	err = stats_redop_percentrank_inc_f_e(numbers, &value, dim, significance, &p_array);
	return err;
}

TCErrorCode stats_redall_percentrank_inc_d_e(const TCArray* numbers, const TCDouble value, TCSize_t significance, TCDouble* percentrank_value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_d(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}


	err = stats_redop_percentrank_inc_d_e(numbers, &value, 0, significance, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*percentrank_value = *(TCDouble*)(p_array->m_data);

	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_percentrank_inc_d_e(const TCArray* numbers, TCSize_t dim, const TCDouble value, TCSize_t significance, TCArray* p_array)
{

	TCErrorCode err;
	err = stats_redop_percentrank_inc_d_e(numbers, &value, dim, significance, &p_array);
	return err;
}



