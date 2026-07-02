



#include "stdafx.h"
#include "..\..\Math\LinearAlgebra\syrk.hpp"
#include "cov.h"

using namespace Descriptive;


TCErrorCode stats_covp_d_e(const TCArray* array1, const TCArray* array2, TCDouble* p_value)
{
    return stats_cov<TCDouble>(array1, array2, p_value, population);
}

TCErrorCode stats_covp_f_e(const TCArray* array1, const TCArray* array2, TCFloat* p_value)
{
    return stats_cov<TCFloat>(array1, array2, p_value, population);
}

TCErrorCode stats_covpmat_d_e(const TCArray* dataMatrix, const TCArray* covMatrix)
{
    return stats_covmat<TCDouble>(dataMatrix, covMatrix, population);
}

TCErrorCode stats_covpmat_f_e(const TCArray* dataMatrix, const TCArray* covMatrix)
{
    return stats_covmat<TCFloat>(dataMatrix, covMatrix, population);
}
