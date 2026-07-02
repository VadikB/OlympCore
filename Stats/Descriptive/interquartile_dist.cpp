



#include "stdafx.h"
#include "TCTypes.h"
#include "TCArray.h"
#include "TCMath.h"
#include "TCArrayUtils.h"

#include "qselect_utils.h"
#include "qselect.h"
#include "interquartile_dist.h"

using namespace Descriptive;

OLYMPCORE_API TCErrorCode stats_redop_interquartile_dist_f_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** dist)
{
    try
    {
        interquartile_dist_t<TCFloat> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, dist);
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

OLYMPCORE_API TCErrorCode stats_redop_interquartile_dist_d_e(TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** dist)
{
    try
    {
        interquartile_dist_t<TCDouble> op;
        return Sort::process_all<Sort::SkipMissingValues, Sort::ConverMissingValuesTo0>(op, numbers, narrays, dim, dist);
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

TCErrorCode stats_redall_interquartile_dist_f_e(TCArray* numbers, TCSize_t narrays, TCFloat* dist)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_f(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_interquartile_dist_f_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*dist = *(TCFloat*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_interquartile_dist_f_e(TCArray* numbers, TCSize_t dim, TCArray* dist)
{

	TCErrorCode err;
	err = stats_redop_interquartile_dist_f_e(numbers, 1, dim, &dist);
	return err;
}

TCErrorCode stats_redall_interquartile_dist_d_e(TCArray* numbers, TCSize_t narrays, TCDouble* dist)
{

	TCArray* p_array = NULL;
	TCSize_t dims[1] = {1};
	TCErrorCode err;
	err = array_allocate_d(1, dims, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}
	err = stats_redop_interquartile_dist_d_e(numbers, narrays, 0, &p_array);
	if (TCError_NoError != tcerror_code_get_errorid(err))
	{
		return err;
	}

	*dist = *(TCDouble*)(p_array->m_data);
	err = array_release(&p_array);
	return err;
}

TCErrorCode stats_reddim_interquartile_dist_d_e(TCArray* numbers, TCSize_t dim, TCArray* dist)
{

	TCErrorCode err;
	err = stats_redop_interquartile_dist_d_e(numbers, 1, dim, &dist);
	return err;
}
