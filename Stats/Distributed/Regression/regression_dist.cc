


#include "TCReduceDist.h"
#include "OlympCoreDist.h"
#include "regression_dist.h"

void GetRankWithLocal(TCDistArray *known, TCSize_t *rank, TCSize_t *ir)
{
    TCSSize_t ranks;
    ranks  = TCDistArray_Get_NProcs(*known);

    TCSSize_t i = 0;

    for(;i<ranks;i++)
    {
        if(((TCSize_t)known->m_Layout.m_Spans[i].StartIdx <= *ir) &&
            ((TCSize_t)known->m_Layout.m_Spans[i].EndIdx >= *ir))
        {
            break;
        }
    }
    *rank = i;

    TCSpan span;
    TCDistArray_Get_Span_On_Rank(known, i, &span);

    *ir -= span.StartIdx;
}

MPI_Datatype GetType(TCDistArray *known_xs)
{
    switch(known_xs->m_LocalArray.m_tag)
    {
    case TC_DOUBLE:
        return MPI_DOUBLE;
    case TC_FLOAT:
        return MPI_FLOAT;
    case TC_DCOMPLEX:
        return MPI_COMPLEX16;    
    case TC_FCOMPLEX:
        return MPI_COMPLEX32;    
    }

    return TC_ERROR;
}



template<typename T>
TCErrorID reshape(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<T> &tmpMem, TCSize_t rank)
{
    bool same = true;

    if(known_xs->m_Layout.m_GlobalDims != known_ys->m_Layout.m_GlobalDims)
    {
        same = false;
    }
    for(TCSize_t i=0; i < known_xs->m_Layout.m_GlobalDims; i++)
    {
        TCBool result;
        TCDistArray_Is_Like(known_xs, known_ys, &result);
        if(!result)
        {
            same = false;
            break;
        }
    }

    if(same)
    {
        known_zs->m_LocalArray.m_data = known_ys->m_LocalArray.m_data;
        known_zs->m_LocalArray.m_numelt = known_ys->m_LocalArray.m_numelt;
    }
    else
    {
        tmpMem.resize(known_xs->m_LocalArray.m_numelt);
        known_zs->m_LocalArray.m_data = &tmpMem;
        known_zs->m_LocalArray.m_numelt = known_xs->m_LocalArray.m_numelt;

        TCSize_t cols_newone = known_ys->m_Layout.m_GlobalDims == 1 ? 1 : known_ys->m_Layout.m_GlobalShape[1];
        TCSize_t rows_orig = known_xs->m_Layout.m_GlobalShape[0];
        TCSize_t cols_orig = known_xs->m_Layout.m_GlobalDims == 1 ? 1 : known_xs->m_Layout.m_GlobalShape[1];
        for(TCSize_t i=0;i<known_xs->m_Layout.m_GlobalSize;i++)
        {
            TCSize_t ir = i % rows_orig;
            TCSize_t ic = i / rows_orig;

            TCSize_t j = ir * cols_orig + ic;

            TCSize_t jr = j / cols_newone;
            TCSize_t jc = j % cols_newone;

            TCSize_t rank_from;
            TCSize_t rank_to;

            if(known_xs->m_Layout.m_DistDim == 0)
            {
                GetRankWithLocal(known_xs, &rank_to, &ir);
                GetRankWithLocal(known_ys, &rank_from, &jr);
            }
            else
            {
                GetRankWithLocal(known_xs, &rank_to, &ic);
                GetRankWithLocal(known_ys, &rank_from, &jc);
            }
                
            TCSize_t *shape;
            TCDistArray_Get_Shape(known_ys, &shape);
            TCSize_t k = jc * shape[0] + jr;

            T tmp = tc_static_cast<T>(0);
            if(rank_to == rank_from)
            {
                tmp = ((T*)known_ys->m_LocalArray.m_data)[k];
            }
            else
            {
                if(rank == rank_to)
                {
                    MPI_Status status;
                    if(MPI_Recv(&tmp, 1, GetType(known_ys), rank_from, 123, known_ys->m_Layout.m_Comm, &status) != MPI_SUCCESS)
                    {
                        return TCError_Internal;
                    }
                }
                if(rank == rank_from)
                {
                    tmp = ((T*)known_ys->m_LocalArray.m_data)[k];
                    if(MPI_Send(&tmp, 1, GetType(known_ys), rank_to, 123, known_ys->m_Layout.m_Comm) != MPI_SUCCESS)
                    {
                        return TCError_Internal;
                    }
                }
            }

            if(rank == rank_to)
            {
                ((T*)known_zs->m_LocalArray.m_data)[ic * known_xs->m_LocalArray.m_dims[0] + ir] = tmp;
            }
        }
    }

    return TCError_NoError;
}

TCSize_t number(TCDouble num)
{
    return (TCSize_t)num;
}
TCSize_t number(TCFloat num)
{
    return (TCSize_t)num;
}
TCSize_t number(TCDComplex num)
{
    return (TCSize_t)num._real;
}
TCSize_t number(TCFComplex num)
{
    return (TCSize_t)num._real;
}




template
TCErrorID reshape<TCDouble>(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<TCDouble> &tmpMem, TCSize_t rank);
template
TCErrorID reshape<TCFloat>(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<TCFloat> &tmpMem, TCSize_t rank);
template
TCErrorID reshape<TCDComplex>(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<TCDComplex> &tmpMem, TCSize_t rank);
template
TCErrorID reshape<TCFComplex>(TCDistArray *known_xs, TCDistArray *known_ys, TCDistArray *known_zs, std::vector<TCFComplex> &tmpMem, TCSize_t rank);