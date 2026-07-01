//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <vector>
#include <list>

#include "TCConsts.h"
#include "../Descriptive/quantile_inc.h"
#include "../Distributed/Descriptive/qselect_dist.h"
#include "../Distributed/Descriptive/qselect_utils_dist.h"

namespace Descriptive
{

#define TAG_MASK 0
#define TAG_VAL_ERROR 1

template <typename T>
struct quantile_inc_dist_t : public quantile_inc_t<T>
{
    typedef TCBool T_error;
    typedef std::vector<T_error> T_errors;
    typedef std::list<T_error>     T_list_errors;
    typedef std::list<T>         T_list_valerrors; 
    typedef std::vector<Sort::interpolation_with_d_t_dist<T>> T_interpolation;

    quantile_inc_dist_t(TCSize_t q):quantile_inc_t(q){}

    inline void GetResultDist(T_state &state, std::vector<TCSize_t> &nelems, Sort::TCVectorIterator<T>* vit, TCSpan* span, MPI_Comm& comm)
    {
        int np, work_np, rank;
        MPI_Status status;
     
        MPI_Comm_rank(comm, &rank);
        MPI_Comm_size(comm, &np);
        
        T* data = state.data();
        T* output;

        TCSize_t nvectors = nelems.size();
        TCSize_t stride = state.size()/nvectors;
        
        T_errors lerrors;
        T_errors gerrors;
        T_list_valerrors list_verrors;

        lerrors.resize(nvectors);
        gerrors.resize(nvectors);
        
        memset(lerrors.data(), 0, nvectors * sizeof(T_error));
        
        for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
        {
            for(TCSize_t ielem = 0; ielem < nelems[nvectors - 1]; ielem++)
            {
                T v = data[ivector * stride + ielem];

                if(TCError::IsErrorImpl<T>::run(&v))
                {
                    lerrors[ivector] = TRUE;
                    list_verrors.push_back(v);
                    break;
                }
            }
        }

        MPI_Allreduce((void*) lerrors.data(), (void*) gerrors.data(), nvectors, mpi_traits<T_error>::mpi_type(), MPI_BOR, comm);

        TCSize_t nerrors = 0;

        for(TCSize_t ivector  = 0; ivector < nvectors; ivector++)
        {
            if(gerrors[ivector])
            {
                nerrors++;
            }
        }

        if(nerrors > 0)
        {
            T_errors mask;
            T_state  verrors;

            mask.resize(nerrors);
            verrors.resize(nerrors);
            
            TCSize_t sq = 0;
            T_list_valerrors::iterator it = list_verrors.begin();

            for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
            {
                if(gerrors[ivector])
                {
                    if(lerrors[ivector])
                    {
                        verrors[sq] = *it;
                        it++;
                    }
                    mask[sq] = lerrors[ivector];
                    sq++;
                }
            }

            if(rank == MASTER_RANK)
            {
                T_errors recv_mask;
                T_state recv_verrors;
                std::list<TCSize_t> idxs;

                for(TCSize_t i = 0; i < nerrors; i++)
                {
                    if(mask[i])
                    {
                        idxs.push_back(i);
                    }
                }
                
                recv_mask.resize(nerrors);
                recv_verrors.resize(nerrors);
                                
                for(int i = 1; i < np; i++)
                {
                    MPI_Recv((void*) recv_mask.data(), nerrors, mpi_traits<T_error>::mpi_type(), i, TAG_MASK, comm, &status);
                    MPI_Recv((void*) recv_verrors.data(), nerrors, mpi_traits<T>::mpi_type(), i, TAG_VAL_ERROR, comm, &status);

                    std::list<TCSize_t>::iterator it = idxs.begin();

                    while(it != idxs.end())
                    {
                        TCSize_t i = *it;

                        if(recv_mask[i])
                        {
                            verrors[i] = recv_verrors[i];
                            idxs.remove(i);
                        }
                        else
                        {
                            it++;
                        }
                    }
                }
            }
            else
            {
                MPI_Send((void*) mask.data(), nerrors, mpi_traits<T_error>::mpi_type(), MASTER_RANK, TAG_MASK, comm);
                MPI_Send((void*) verrors.data(), nerrors, mpi_traits<T>::mpi_type(), MASTER_RANK, TAG_VAL_ERROR, comm);
            }

            MPI_Bcast((void*) verrors.data(), nerrors, mpi_traits<T>::mpi_type(), MASTER_RANK, comm);

            sq = 0;
            TCSize_t stride_low = vit->GetStride();
            TCSize_t vnelem = vit->GetSize();

            for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
            {
                if(gerrors[ivector])
                {
                    output = vit->GetVector(ivector);

                    for(TCSize_t j = 0; j < vnelem; j++)
                    {
                        output[j * stride_low] = verrors[sq];
                    }

                    nelems[ivector] = 0;
                    sq++;
                }
            }
        }
        
        int working_p = 0;
        TCSize_t ivector = 0;

        while((working_p == 0) && (ivector < nvectors))
        {
            if(nelems[ivector] > 0)
            {
                working_p = 1;
            }

            ivector++;
        }

        MPI_Allreduce((void*) &working_p, (void*) &work_np, 1, MPI_INT, MPI_SUM, comm);

        std::vector<TCSize_t> gnelems;
        gnelems.resize(nvectors);

        MPI_Allreduce((void*) nelems.data(), (void*) gnelems.data(), nvectors, mpi_traits<TCSSize_t>::mpi_type(), MPI_SUM, comm);

        for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
        {
			T_state::iterator itBegin = state.begin() + ivector * stride;
			T_state::iterator itEnd = itBegin + nelems[ivector];

			std::sort(itBegin, itEnd);
        }

        std::vector<TCSize_t> m;
        std::vector<TCSize_t> status_select;
        T_state v;
        T_interpolation mean;

        v.resize(nvectors);
        m.resize(nvectors);
        status_select.resize(nvectors);
        mean.resize(nvectors);

        memset(status_select.data(), 0, nvectors * sizeof(TCSize_t));
        
        for (TCSize_t i = 0; i <= q; i++)
        {
            memset(v.data(), 0, nvectors * sizeof(T));

            for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
            {
				TCSize_t p_entier = (gnelems[ivector] - 1) / q;
				TCSize_t p_excess = (gnelems[ivector] - 1) % q;

				TCSize_t nexcess = p_excess * i;
				m[ivector] = p_entier * i + nexcess / q;
				
				TCSize_t excess = nexcess % q;
				mean[ivector].d = (T)excess / q;
                              
                if (m[ivector] >= gnelems[ivector])
                {
                    v[ivector] = TCError::quiet_NaN<T>();
                }
            }

            Sort::select_dist<T, true, T_interpolation>(state.data(), stride, nelems.data(), gnelems.data(), m.data(), status_select.data(), nvectors, v.data(), mean, OP_MIN, comm, work_np);
        
			TCSSize_t ui = static_cast<TCSSize_t>(i);
            if((span->StartIdx <= ui) && (ui <= span->EndIdx))
            {
                TCSize_t elidx = i - span->StartIdx;
                TCSize_t stride_low = vit->GetStride();

                for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
                {
                    output = vit->GetVector(ivector);
                    output[elidx * stride_low] = v[ivector];
                }
            }
        }
    }
};
}