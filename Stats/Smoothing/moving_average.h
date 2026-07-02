



#pragma once

#include "stdafx.h"

namespace Smoothing {
	template <typename T>
	T _get_tc_error(TCErrorCode errCode = TCError_NoError);

	template <>
	TCFloat _get_tc_error(TCErrorCode errCode)
	{
		return tcerror_newerror_f(errCode);
	};

	template <>
	TCDouble _get_tc_error(TCErrorCode errCode)
	{
		return tcerror_newerror_d(errCode);
	};

	inline TCBool dim_analysis(const TCArray* x, TCSize_t* rows, TCSize_t* columns)
	{
		if (x->m_ndims == 2) 
		{
			*columns = x->m_dims[1];
			*rows = x->m_dims[0];
		} else if (x->m_ndims == 1) 
		{
			*columns = 1;
			*rows = x->m_numelt;
		} else
		{
			return 0;
		}
		return 1;
	}

	inline TCBool dim_analysis(const TCArray* x, TCSize_t compare_ndims, TCSize_t* rows, TCSize_t* columns)
	{
		if (x->m_ndims == 2) 
		{
			*columns = x->m_dims[1];
			*rows = x->m_dims[0];
		} else if (x->m_ndims == 1 && compare_ndims != 2) 
		{
			*columns = 1;
			*rows = x->m_numelt;
		} else if (x->m_ndims == 1 && compare_ndims == 2)
		{
			*columns = x->m_numelt;
			*rows = x->m_numelt > 0 ? 1 : 0;
			return 1;
		}
		
		if (x->m_ndims != compare_ndims)
		{
			return 0;
		}
		return 1;
	}

	enum ValueType
	{
		ActualValue,
		MissingValue,
		ErrorValue,
		InfValue
	};

	template<typename T>
	inline ValueType value_type(T value)
	{
		if (TCError::IsMissing(&value))
			return MissingValue;
		else if (isnan(value))
			return ErrorValue;
		else if (isinf(value))
			return InfValue;
		else
			return ActualValue;
	}
}