



#include "stdafx.h"
#include "TCArrayUtils.h"
#include "TCFuncWrapper.h"
#include <TCErrorCommon.h>
#include "dist/Hypgeom.h"

using namespace Dist;

template <typename T> 
TCBool genmissing(T)
{
    return false;
}
template <>
TCBool genmissing(TCDouble in)
{
    return TCError::IsMissingImpl<TCDouble>::run(&in);
}
template <>
TCBool genmissing(TCFloat in)
{
    return TCError::IsMissingImpl<TCFloat>::run(&in);
}

template <typename T1, typename T2>
TCErrorCode  fisher_test_body(const TCArray* sample, TCHypothesis type, T2* p_value)
{
    TCUInt64 f11;
    TCUInt64 f12;
    TCUInt64 f21;
    TCUInt64 f22;
    TCUInt64 fd1;
    TCUInt64 fd2;
    TCUInt64 f1d;
    TCUInt64 f2d;
    TCUInt64 n;
    TCUInt64 len;
    TCUInt64 i;

    T2 p0;
    T2 pres;
    T2 dTemp;
    T1* data = ((T1*)sample->m_data);
    TCTypeTag tag = sample->m_tag;

    if (sample->m_ndims != 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (sample->m_dims[0] != 2 || sample->m_dims[1] != 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (!((type == OneTailGT) || (type == OneTailLT) || (type == TwoTail)))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }
    TCBool isempty;
    array_isempty(sample, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (tag == TC_DOUBLE || tag == TC_FLOAT )
    {
        for (i = 0; i < sample->m_numelt; i++)
        {
           if (genmissing(data[i]))
            {
                data[i] = 0.;
            }
        }
        if ( ! ((data[0] == floor((T2)data[0])) && (data[1 * sample->m_strides[1]] == floor((T2)data[1 * sample->m_strides[1]])) 
            && (data[1 * sample->m_strides[0]] == floor((T2)data[1 * sample->m_strides[0]]))
            && (data[1 * sample->m_strides[0] + 1 * sample->m_strides[1]] == floor((T2)data[1 * sample->m_strides[0] + 1 * sample->m_strides[1]]))))
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }
        for (i = 0; i < sample->m_numelt; i++)
        {
            if (TCError::is_nan<T2>(data[i]) || isinf<T2>((T2&)data[i]) || (TCDouble)data[i] < 0.)
            {
                *p_value = data[i];
                return tcerror_code_new(TCError_IllegalInput, 1);
            }
        }
    }
    f11 = (TCUInt64)data[0];
    f12 = (TCUInt64)data[1 * sample->m_strides[1]];
    f21 = (TCUInt64)data[1 * sample->m_strides[0]];
    f22 = (TCUInt64)data[1 * sample->m_strides[0] + 1 * sample->m_strides[1]];

    f1d = f11 + f12;
    f2d = f21 + f22;
    fd1 = f11 + f21;
    fd2 = f12 + f22;
    n = fd1 + fd2;
    len = (( fd1 < f1d ) ? fd1 : f1d);
    if (0 == n)
    {
        *p_value = 1;
        return TCError_NoError;
    }

    if (OneTailLT == type)
    {
        if (0 == fd1)  
        {
            pres = 1.;
        }
        else
        {
            pres = Hypgeom<T2>::CDF((T2)f11, (T2)fd1, (T2)f1d, (T2)n);
        }
    }
    else if (OneTailGT == type)  
    {
        if (0 == f11 )  
        {
            pres = 1.;
        }
        else
        {
            pres = 0.;
            for (i = f11; i <= len; i++)
            {
                pres += Hypgeom<T2>::PMF((T2)i, (T2)fd1, (T2)f1d, (T2)n);
            }
        }
    }
    else if (TwoTail == type)
    {
        if (0 == fd1)  
        {
            pres = 1.;
        }
        else
        {
            p0 = Hypgeom<T2>::PMF((T2)f11, (T2)fd1, (T2)f1d, (T2)n);
            pres = p0;
            for (i = 0; i <= len; i++)
            {
                dTemp = Hypgeom<T2>::PMF((T2)i, (T2)fd1, (T2)f1d, (T2)n);
                if (dTemp <= p0 && i != f11)
                {
                    pres += dTemp;
                }
            }
        }
    }
    *p_value = pres;
    return TCError_NoError;
}

C_BEGINF(stats_inference_fisher_test_ui32_e, const TCArray* sample, TCHypothesis test, TCFloat* p_value)
{
    return fisher_test_body<TCUInt32, TCFloat> (sample, test, p_value);
} C_ENDF

C_BEGINF(stats_inference_fisher_test_ui64_e, const TCArray* sample, TCHypothesis test, TCDouble* p_value)
{
    return fisher_test_body<TCUInt64, TCDouble> (sample, test, p_value);
}C_ENDF

C_BEGINF(stats_inference_fisher_test_f_e, const TCArray* sample, TCHypothesis test, TCFloat* p_value)
{
    return fisher_test_body<TCFloat, TCFloat> (sample, test, p_value);
} C_ENDF

C_BEGINF(stats_inference_fisher_test_d_e, const TCArray* sample, TCHypothesis test, TCDouble* p_value)
{
    return fisher_test_body<TCDouble, TCDouble> (sample, test, p_value);
} C_ENDF