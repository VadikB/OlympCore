//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "z_test.h"

using namespace Inference;

TCErrorCode stats_inference_z_test_std_d(const TCArray* sample, TCDouble mean, TCDouble standard_deviation, TCDouble* p_value)
{
    assert(sample->m_tag == TC_DOUBLE);

    return stats_inference_z_test<TCDouble>(sample, mean, standard_deviation, p_value);
}

TCErrorCode stats_inference_z_test_std_f(const TCArray* sample, TCFloat mean, TCFloat standard_deviation, TCFloat* p_value)
{
    assert(sample->m_tag == TC_FLOAT);

    return stats_inference_z_test<TCFloat>(sample, mean, standard_deviation, p_value);
}

TCErrorCode stats_inference_z_test_d(const TCArray* sample, TCDouble mean, TCDouble* p_value)
{
    assert(sample->m_tag == TC_DOUBLE);

    return stats_inference_z_test<TCDouble>(sample, mean, p_value);
}

TCErrorCode stats_inference_z_test_f(const TCArray* sample, TCFloat mean, TCFloat* p_value)
{
    assert(sample->m_tag == TC_FLOAT);

    return stats_inference_z_test<TCFloat>(sample, mean, p_value);
}
