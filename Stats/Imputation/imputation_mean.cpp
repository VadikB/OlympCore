



#include "stdafx.h"
#include <OlympCore.h>
#include <TCFuncWrapper.h>
#include "imputation_mean.h"

using namespace Imputation;

C_BEGINF(stats_imputation_mean_f_e, InOut TCArray* data_in, TCUInt64* not_replaced)
{
    assert(data_in->m_tag == TC_FLOAT);
    return imputation_mean<TCFloat>(data_in, not_replaced);
} C_ENDF

C_BEGINF(stats_imputation_mean_d_e, InOut TCArray* data_in, TCUInt64* not_replaced)
{
    assert(data_in->m_tag == TC_DOUBLE);
    return imputation_mean<TCDouble>(data_in, not_replaced);
} C_ENDF

