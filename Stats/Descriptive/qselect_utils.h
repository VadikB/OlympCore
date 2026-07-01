//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCArray.h"
#include <vector>

namespace Descriptive
{
namespace Sort
{

//----------------------------------------------------------------------  
template <typename T>
struct operator_temp_copy_t
{
    typedef T    in_t;
    typedef T    ou_t;

    typedef std::vector<T> T_state;

    inline TCErrorID InitState(T_state& state, TCSize_t nelem)
    {
        state.resize(nelem);
        return TCError_NoError;
    }
    inline void ProcessFirst(T_state& state, T& x)
    {
        state[0] = x;
    }
    inline void Process(T_state& state, T& x, TCSize_t index)
    {
        state[index] = x;
    }
    //inline TCErrorCode GetResult(T_state& state, TCSize_t nelem, T& result)
};

template <class T> struct SkipMissingValues
{
    static inline bool ismissing(T& in)
    {
        return TCError::IsMissing<T>(&in);
    }
};

template <class T> struct ConverMissingValuesTo0
{
    static inline bool ismissing(T& in)
    {
        if (TCError::IsMissing<T>(&in))
        {
            in = 0;
        }
        return false;
    }
};

template <template <class> class MissingPolicy, class OP>
TCErrorID process_dim(OP &op, const TCArray *num, TCArray* value, TCSize_t dim)
{
    TCSize_t max_nelem = num->m_dims[dim - 1];
    OP::T_state state;
    TCErrorID err = op.InitState(state, max_nelem);
    if (err != TCError_NoError)
    {
        return err;
    }

    TCSize_t value_nelem = value->m_numelt;

    TCSize_t stride_low = num->m_strides[dim - 1];

    TCSize_t chunks_outer = value_nelem / stride_low;
    for (TCSize_t chunk_outer = 0; chunk_outer < chunks_outer; chunk_outer++)
    {
        TCSize_t base_element = chunk_outer * max_nelem * stride_low;
        TCSize_t value_element = chunk_outer * stride_low;
        for (TCSize_t chunk_inner = 0; chunk_inner < stride_low; chunk_inner++, value_element++, base_element++)
        {
            const OP::in_t* base = ((OP::in_t*)num->m_data) + base_element;
            OP::ou_t* result = ((OP::ou_t*)value->m_data) + value_element;
            TCSize_t i = 0;
            TCSize_t nelem = 0;
            if (max_nelem > 0)
            {
                OP::in_t v = base[i * stride_low];
                if (MissingPolicy<OP::in_t>::ismissing(v))
                {
                    i++;
                    while (i < max_nelem)
                    {
                        v = base[i * stride_low];
                        if (!MissingPolicy<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                if (i < max_nelem)
                {
                    op.ProcessFirst(state, v);
                    i++; nelem++;
                }
                for (; i < max_nelem; i++)
                {
                    OP::in_t v = base[i * stride_low];
                    if (!MissingPolicy<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
            op.GetResult(state, nelem, *result);
        }
    }

    return TCError_NoError;
}

// process reduce operation
template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process_all(OP &op, const TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** value)
{
    // reduce array according to its dimensions
    if (0 == dim)
    {
        // reduce all elements
        size_t telem = 0;
        for (TCSize_t i = 0 ; i < narrays; i++)
        {
            telem += numbers[i].m_numelt;
        }
        // if the data is empty then returns error
        if (telem == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }
        assert(value[0]->m_numelt == 1);
        // collect all data
        OP::T_state state;
        TCErrorID err = op.InitState(state, telem);
        if (err != TCError_NoError)
        {
            return tcerror_code_new(err, TCArgPosition_UnKnown);
        }
        TCSize_t nelem = 0;
        TCSize_t k = 0;
        do {
            while(k < narrays && numbers[k].m_numelt == 0) k++;
            if (k < narrays)
            {
                const OP::in_t* base = (OP::in_t*)numbers[k].m_data;
                TCSize_t nelem_array = numbers[k].m_numelt;
                bool isref = numbers[k].m_isref;
                TCSize_t i = 0;
                OP::in_t v;
                if (isref)
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                else
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                if (i < nelem_array)
                {
                    op.ProcessFirst(state, v);
                    i++; nelem++;
                }
                else
                {
                    k++;
                    continue;
                }
                if (isref)
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                else
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                k++;
            }
        } while (0);
        for (; k < narrays; k++)
        {
            const OP::in_t* base = (OP::in_t*)numbers[k].m_data;
            TCSize_t nelem_array = numbers[k].m_numelt;
            if (numbers[k].m_isref)
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
            else
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
        }
        OP::ou_t* result = (OP::ou_t*)(value[0]->m_data);
        return op.GetResult(state, nelem, *result);
    } else {
        // reduce data in 'dim' direction
        if (narrays != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (dim > numbers[0].m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (numbers[0].m_numelt == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }
        assert(value[0]->m_ndims == numbers[0].m_ndims - 1);
        TCErrorID err;
        if (numbers[0].m_isref)
        {
            err = process_dim<MissingPolicyRef, OP>(op, numbers, value[0], dim);
        }
        else
        {
            err = process_dim<MissingPolicyNotRef, OP>(op, numbers, value[0], dim);
        }
        return tcerror_code_new(err, TCArgPosition_UnKnown);
    } // 0 == dim
}


template <template <class> class MissingPolicy, class OP>
TCErrorID process2vector_dim(OP &op, const TCArray *num, TCArray* value, TCSize_t dim)
{
    TCSize_t max_nelem = num->m_dims[dim - 1];
    OP::T_state state;
    TCErrorID err = op.InitState(state, max_nelem);
    if (err != TCError_NoError)
    { 
        return err;
    }

    TCSize_t out_vector_nelem = value->m_dims[dim - 1];
    TCSize_t value_nelem = value->m_numelt / out_vector_nelem;

    TCSize_t stride_low = num->m_strides[dim - 1];

    TCSize_t chunks_outer = value_nelem / stride_low;
    for (TCSize_t chunk_outer = 0; chunk_outer < chunks_outer; chunk_outer++)
    {
        TCSize_t value_element = chunk_outer * out_vector_nelem * stride_low;
        TCSize_t base_element = chunk_outer * max_nelem * stride_low;
        for (TCSize_t chunk_inner = 0; chunk_inner < stride_low; chunk_inner++, value_element++, base_element++)
        {
            const OP::in_t* base = ((OP::in_t*)num->m_data) + base_element;
            OP::ou_t* result = ((OP::ou_t*)value->m_data) + value_element;
            TCSize_t i = 0;
            TCSize_t nelem = 0;
            if (max_nelem > 0)
            {
                OP::in_t v = base[i * stride_low];
                if (MissingPolicy<OP::in_t>::ismissing(v))
                {
                    i++;
                    while (i < max_nelem)
                    {
                        v = base[i * stride_low];
                        if (!MissingPolicy<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                if (i < max_nelem)
                {
                    op.ProcessFirst(state, v);
                    i++; nelem++;
                }
                for (; i < max_nelem; i++)
                {
                    OP::in_t v = base[i * stride_low];
                    if (!MissingPolicy<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
            op.GetResult(state, nelem, result, stride_low);
        }
    }

    return TCError_NoError;
}

// process reduce operation
template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process2vector_all(OP &op, const TCArray* numbers, TCSize_t narrays, TCSize_t dim, TCArray** value)
{
    // reduce array according to its dimensions
    if (0 == dim)
    {
        // reduce all elements
        size_t telem = 0;
        for (TCSize_t i = 0 ; i < narrays; i++)
        {
            telem += numbers[i].m_numelt;
        }
        // if the data is empty then returns error
        if (telem == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }
        // collect all data
        OP::T_state state;
        TCErrorID err = op.InitState(state, telem);
        if (err != TCError_NoError)
        {
            return tcerror_code_new(err, TCArgPosition_UnKnown);
        }
        TCSize_t nelem = 0;
        TCSize_t k = 0;
        do {
            while(k < narrays && numbers[k].m_numelt == 0) k++;
            if (k < narrays)
            {
                const OP::in_t* base = (OP::in_t*)numbers[k].m_data;
                TCSize_t nelem_array = numbers[k].m_numelt;
                bool isref = numbers[k].m_isref;
                TCSize_t i = 0;
                OP::in_t v;
                if (isref)
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                else
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                if (i < nelem_array)
                {
                    op.ProcessFirst(state, v);
                    i++; nelem++;
                }
                else
                {
                    k++;
                    continue;
                }
                if (isref)
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                else
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                k++;
            }
        } while (0);
        for (; k < narrays; k++)
        {
            const OP::in_t* base = (OP::in_t*)numbers[k].m_data;
            TCSize_t nelem_array = numbers[k].m_numelt;
            if (numbers[k].m_isref)
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
            else
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
        }
        // save results
        return op.GetResult(state, nelem, (OP::ou_t*)(value[0]->m_data), 1);
    } else {
        // reduce data in 'dim' direction
        if (narrays != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (dim > numbers[0].m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (numbers[0].m_numelt == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }
        assert(value[0]->m_ndims == numbers[0].m_ndims);
        TCErrorID err;
        if (numbers[0].m_isref)
        {
            err = process2vector_dim<MissingPolicyRef, OP>(op, numbers, value[0], dim);
        }
        else
        {
            err = process2vector_dim<MissingPolicyNotRef, OP>(op, numbers, value[0], dim);
        }
        return tcerror_code_new(err, TCArgPosition_UnKnown);
    } // 0 == dim
}

} // namespace Sort
} // namespace Descriptive