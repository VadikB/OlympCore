//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "ks2_test_dist.h"

#include <OlympCoreDist.h>

using namespace Inference;

TCErrorCode stats_inference_ks2_test_f_e_dist(TCDistArray* sample1, TCDistArray* sample2,
    TCHypothesis test, TCFloat* p_value,
    TCFloat* test_statistic)
{
    assert(TCDistArray_Get_TypeTag(*sample1) == TC_FLOAT);
    assert(TCDistArray_Get_TypeTag(*sample2) == TC_FLOAT);

    return stats_inference_ks2_test_dist<TCFloat>(sample1, sample2, test,
        p_value, test_statistic);
}

TCErrorCode stats_inference_ks2_test_d_e_dist(TCDistArray* sample1, TCDistArray* sample2,
    TCHypothesis test, TCDouble* p_value,
    TCDouble* test_statistic)
{
    assert(TCDistArray_Get_TypeTag(*sample1) == TC_DOUBLE);
    assert(TCDistArray_Get_TypeTag(*sample2) == TC_DOUBLE);

    return stats_inference_ks2_test_dist<TCDouble>(sample1, sample2, test,
        p_value, test_statistic);
}