



#include "stdafx.h"
#include <OlympCore.h>
#include <TCFuncWrapper.h>
#include "ks_test.h"

using namespace Inference;

C_BEGINF(stats_inference_ks_test_f_e, TCArray* sample, TCHypothesis test,
    TCFloat* p_value, TCFloat* test_statistic)
{
    assert(sample->m_tag == TC_FLOAT);

    return stats_inference_ks_test_sequential<TCFloat>(sample, test, p_value, test_statistic);
} C_ENDF

C_BEGINF(stats_inference_ks_test_d_e, TCArray* sample, TCHypothesis test,
    TCDouble* p_value, TCDouble* test_statistic)
{
    assert(sample->m_tag == TC_DOUBLE);

    return stats_inference_ks_test_sequential<TCDouble>(sample, test, p_value, test_statistic);
} C_ENDF