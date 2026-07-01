//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "ks_test_dist.h"

#include <OlympCoreDist.h>

using namespace Inference;

TCErrorCode stats_inference_ks_test_f_e_dist(TCDistArray* sample, TCHypothesis test,
    TCFloat* p_value, TCFloat* test_statistic)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_FLOAT);

    return stats_inference_ks_test_dist<TCFloat>(sample, test, p_value, test_statistic);
}

TCErrorCode stats_inference_ks_test_d_e_dist(TCDistArray* sample, TCHypothesis test,
    TCDouble* p_value, TCDouble* test_statistic)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_DOUBLE);

    return stats_inference_ks_test_dist<TCDouble>(sample, test, p_value, test_statistic);
}