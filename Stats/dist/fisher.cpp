//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "fisher.h"
using namespace Dist;

TCFloat stats_infer_fisher_f(const TCFloat x)
{
    return Fisher<TCFloat>::CDF(x);
}

TCDouble stats_infer_fisher_d(const TCDouble x)
{
    return Fisher<TCDouble>::CDF(x);
}

TCFloat stats_infer_inv_fisher_f(const TCFloat x)
{
    return Fisher<TCFloat>::InvCDF(x);
}

TCDouble stats_infer_inv_fisher_d(const TCDouble x)
{
    return Fisher<TCDouble>::InvCDF(x);
}
