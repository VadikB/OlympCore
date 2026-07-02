



#include "stdafx.h"
#include "mw_test.h"
#include <TCMath.h>
#include <TCFuncWrapper.h>

using namespace Inference;

using namespace Dist;

template<typename T>
static TCErrorCode mw_do_sort(const TCArray* sample)
{
    static_assert(false, "mw_test math_sort is specialized by wrong type");
}
template<>
static TCErrorCode mw_do_sort<TCFloat>(const TCArray* sample)
{
    return math_sort_f_e(sample, true, 1, const_cast<TCArray*>(sample));
}
template<>
static TCErrorCode mw_do_sort<TCDouble>(const TCArray* sample)
{
    return math_sort_d_e(sample, true, 1, const_cast<TCArray*>(sample));
}

template<typename T>
TCErrorCode stats_inference_mw_test(TCArray* sample1, TCArray* sample2, TCHypothesis testtype, T* p_value, T* u_statistic)
{
    T* xvec;
    T* yvec; 
    TCErrorCode code_return;
    TCErrorCode code_ok = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);

    TCSize_t x_len;
    TCSize_t y_len;

    x_len = sample1->m_numelt;
    if (x_len == 0)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    y_len = sample2->m_numelt;
    if (y_len == 0)
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }
    
    code_return = mw_do_sort<T>(sample1);
    if (code_return != code_ok)
    {
        return tcerror_code_new(TCError_Internal, TCArgumentID(1));
    }
    code_return = mw_do_sort<T>(sample2);
    if (code_return != code_ok)
    {
        return tcerror_code_new(TCError_Internal, TCArgumentID(2));
    }

    xvec = (T*)sample1->m_data;
    yvec = (T*)sample2->m_data;

    code_return = mw_check_input<T>(xvec, x_len, 1);
    if (code_return != TCError_NoError)
    {
        return code_return;
    }
    code_return = mw_check_input<T>(yvec, y_len, 2);
    if (code_return != TCError_NoError)
    {
        return code_return;
    }
    
    if (x_len <= y_len)
    {
        code_return = stats_mw_test<T>(xvec, x_len, yvec, y_len, testtype, p_value, u_statistic);
    }
    else
    {
        code_return = stats_mw_test<T>(yvec, y_len, xvec, x_len, testtype, p_value, u_statistic);
    }
    return code_return;
}

C_BEGINF(stats_inference_mw_test_f_e, TCArray* sample1, TCArray* sample2, TCHypothesis testtype, TCFloat* p_value, TCFloat* u_statistic)
{
    assert(sample1->m_tag == TC_FLOAT);
    assert(sample2->m_tag == TC_FLOAT);

    return stats_inference_mw_test<TCFloat>(sample1, sample2, testtype, p_value, u_statistic);
} C_ENDF

C_BEGINF(stats_inference_mw_test_d_e, TCArray* sample1, TCArray* sample2, TCHypothesis testtype, TCDouble* p_value, TCDouble* u_statistic)
{	
    assert(sample1->m_tag == TC_DOUBLE);
    assert(sample2->m_tag == TC_DOUBLE);

    return stats_inference_mw_test<TCDouble>(sample1, sample2, testtype, p_value, u_statistic);
} C_ENDF