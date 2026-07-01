//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "pearson.h"

using namespace Descriptive;


TCErrorCode stats_pearson_d_e(const TCArray* array1, const TCArray* array2, TCDouble* r_value)
{
    return stats_pearson<TCDouble>(array1, array2, r_value);
}

TCErrorCode stats_pearson_f_e(const TCArray* array1, const TCArray* array2, TCFloat* r_value)
{
    return stats_pearson<TCFloat>(array1, array2, r_value);
}

TCErrorCode stats_pearsonmat_d_e(const TCArray* data, TCArray* r_matrix)
{
    return stats_pearsonmat<TCDouble>(data, r_matrix);
}

TCErrorCode stats_pearsonmat_f_e(const TCArray* data, TCArray* r_matrix)
{
    return stats_pearsonmat<TCFloat>(data, r_matrix);
}
