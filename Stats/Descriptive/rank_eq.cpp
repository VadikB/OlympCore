//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "rank_eq.h"

using namespace Descriptive;

TCErrorCode stats_redall_rank_eq_f_e(const TCFloat number,  const TCArray* ref, TCBool order, TCFloat* rank_eq)
{
    assert(ref->m_tag == TC_FLOAT);

    return stats_redall_rank_eq<TCFloat>(number, ref, order, rank_eq);
}

TCErrorCode stats_redall_rank_eq_d_e(const TCDouble number,  const TCArray* ref, TCBool order, TCDouble* rank_eq)
{
    assert(ref->m_tag == TC_DOUBLE);

    return stats_redall_rank_eq<TCDouble>(number, ref, order, rank_eq);
}

TCErrorCode stats_reddim_rank_eq_f_e(const TCFloat number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_eq)
{
    assert(ref->m_tag == TC_FLOAT && rank_eq->m_tag == TC_FLOAT);
    
    return stats_reddim_rank_eq<TCFloat>(number, ref, order, dim, rank_eq);
}

TCErrorCode stats_reddim_rank_eq_d_e(const TCDouble number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_eq)
{
    assert(ref->m_tag == TC_DOUBLE && rank_eq->m_tag == TC_DOUBLE);
    
    return stats_reddim_rank_eq<TCDouble>(number, ref, order, dim, rank_eq);
}