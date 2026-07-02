



#include "stdafx.h"
#include <OlympCore.h>
#include <TCFuncWrapper.h>
#include "ks2_test.h"

using namespace Inference;

C_BEGINF(stats_inference_ks2_test_f_e, TCArray* sample1, TCArray* sample2,
    TCHypothesis test, TCFloat* p_value, TCFloat* test_statistic)
{
    assert(sample1->m_tag == TC_FLOAT);
    assert(sample2->m_tag == TC_FLOAT);

    return stats_inference_ks2_test_sequential<TCFloat>(sample1, sample2, test, p_value, test_statistic);
} C_ENDF

C_BEGINF(stats_inference_ks2_test_d_e, TCArray* sample1, TCArray* sample2,
    TCHypothesis test, TCDouble* p_value, TCDouble* test_statistic)
{
    assert(sample1->m_tag == TC_DOUBLE);
    assert(sample2->m_tag == TC_DOUBLE);

    return stats_inference_ks2_test_sequential<TCDouble>(sample1, sample2, test, p_value, test_statistic);
} C_ENDF