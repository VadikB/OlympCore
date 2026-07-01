//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "chisq_test.h"

using namespace Inference;


TCErrorCode stats_inference_chisq_test_d(const TCArray* actual_range, const TCArray* expected_range, TCDouble* p_value)
{
    return stats_inference_chisq_test<TCDouble>(actual_range, expected_range, p_value);
}

TCErrorCode stats_inference_chisq_test_f(const TCArray* actual_range, const TCArray* expected_range, TCFloat* p_value)
{
    return stats_inference_chisq_test<TCFloat>(actual_range, expected_range, p_value);
}