



#include "stdafx.h"

#include "TCArrayUtils.h"
#include "qselect_utils.h"
#include "qselect.h"
#include "small.h"

using namespace Descriptive;

TCErrorCode stats_redop_small_f_e(TCArray* numbers, TCSize_t narrays, TCSize_t k, TCSize_t dim, TCArray** value)
{
    try
    {
        small_t<TCFloat> op(k);
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redop_small_d_e(TCArray* numbers, TCSize_t narrays, TCSize_t k, TCSize_t dim, TCArray** value)
{
    try
    {
        small_t<TCDouble> op(k);
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, value);
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

TCErrorCode stats_redall_small_f_e(TCArray* numbers, TCSize_t k, TCFloat* value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_f(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_small_f_e(numbers, 1, k, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*value = *(TCFloat*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_small_f_e(TCArray* numbers, TCSize_t dim, TCSize_t k, TCArray* p_array)
{

	TCErrorCode err;
	err = stats_redop_small_f_e(numbers, 1, k, dim, &p_array);
	return err;
}

TCErrorCode stats_redall_small_d_e(TCArray* numbers, TCSize_t k, TCDouble* value)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_d(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_small_d_e(numbers, 1, k, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*value = *(TCDouble*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_small_d_e(TCArray* numbers, TCSize_t dim, TCSize_t k, TCArray* p_array)
{

	TCErrorCode err;
	err = stats_redop_small_d_e(numbers, 1, k, dim, &p_array);
	return err;
}
