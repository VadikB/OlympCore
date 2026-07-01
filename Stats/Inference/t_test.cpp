//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "t_test.h"

using namespace Inference;

TCErrorCode stats_inference_t_test_d(
    const TCArray* sample1, 
    const TCArray* sample2, 
    const TCHypothesis tails, 
    const TCTestType type,
    TCDouble* p_value)
{
    return stats_inference_t_test<TCDouble>(sample1, sample2, tails, type, p_value);
}

TCErrorCode stats_inference_t_test_f(
    const TCArray* sample1, 
    const TCArray* sample2, 
    const TCHypothesis tails, 
    const TCTestType type,
    TCFloat* p_value)
{
    return stats_inference_t_test<TCFloat>(sample1, sample2, tails, type, p_value);
}