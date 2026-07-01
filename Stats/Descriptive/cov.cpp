//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "..\..\Math\LinearAlgebra\syrk.hpp"
#include "cov.h"

using namespace Descriptive;


TCErrorCode stats_cov_d_e(const TCArray* array1, const TCArray* array2, TCDouble* p_value)
{
    return stats_cov<TCDouble>(array1, array2, p_value, sample);
}

TCErrorCode stats_cov_f_e(const TCArray* array1, const TCArray* array2, TCFloat* p_value)
{
    return stats_cov<TCFloat>(array1, array2, p_value, sample);
}

TCErrorCode stats_covmat_d_e(const TCArray* dataMatrix, const TCArray* covMatrix)
{
    return stats_covmat<TCDouble>(dataMatrix, covMatrix, sample);
}

TCErrorCode stats_covmat_f_e(const TCArray* dataMatrix, const TCArray* covMatrix)
{
    return stats_covmat<TCFloat>(dataMatrix, covMatrix, sample);
}
