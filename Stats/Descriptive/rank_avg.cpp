//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "rank_avg.h"

using namespace Descriptive;

TCErrorCode stats_redall_rank_avg_f_e(const TCFloat number,  const TCArray* ref, TCBool order, TCFloat* rank_avg)
{
    assert(ref->m_tag == TC_FLOAT);

    return stats_redall_rank_avg<TCFloat>(number, ref, order, rank_avg);
}

TCErrorCode stats_redall_rank_avg_d_e(const TCDouble number,  const TCArray* ref, TCBool order, TCDouble* rank_avg)
{
    assert(ref->m_tag == TC_DOUBLE);

    return stats_redall_rank_avg<TCDouble>(number, ref, order, rank_avg);
}

TCErrorCode stats_reddim_rank_avg_f_e(const TCFloat number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_avg)
{
    assert(ref->m_tag == TC_FLOAT && rank_avg->m_tag == TC_FLOAT);
    
    return stats_reddim_rank_avg<TCFloat>(number, ref, order, dim, rank_avg);
}

TCErrorCode stats_reddim_rank_avg_d_e(const TCDouble number,  const TCArray* ref, TCBool order, TCSize_t dim, TCArray* rank_avg)
{
    assert(ref->m_tag == TC_DOUBLE && rank_avg->m_tag == TC_DOUBLE);
    
    return stats_reddim_rank_avg<TCDouble>(number, ref, order, dim, rank_avg);
}