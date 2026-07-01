//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include <OlympCore.h>
#include <TCFuncWrapper.h>
#include "lvcf.h"

using namespace Imputation;

C_BEGINF(stats_imputation_lvcf_f_e, TCArray* dataset, TCUInt64* not_replaced)
{
    assert(dataset->m_tag == TC_FLOAT);

    return stats_imputation_lvcf<TCFloat>(dataset, not_replaced);
} C_ENDF

C_BEGINF(stats_imputation_lvcf_d_e, TCArray* dataset, TCUInt64* not_replaced)
{
    assert(dataset->m_tag == TC_DOUBLE);

    return stats_imputation_lvcf<TCDouble>(dataset, not_replaced);
} C_ENDF
