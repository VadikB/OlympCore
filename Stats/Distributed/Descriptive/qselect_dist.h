//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCCommon.h"
#include "TCCommonDist.h"
#include "TCMath.h"
#include "OlympCoreUtils.h"

#include <mpi.h>
#include <vector>
#include <algorithm>
#include <stack>

template <typename T>
struct value_location
{
    T val;
    TCInt32 rank;
};

SPECIALIZE_MPI_TRAIT(value_location<TCInt16>, MPI_SHORT_INT);
SPECIALIZE_MPI_TRAIT(value_location<TCInt32>, MPI_2INT);
SPECIALIZE_MPI_TRAIT(value_location<TCInt64>, MPI_LONG_LONG_INT);
SPECIALIZE_MPI_TRAIT(value_location<TCFloat>, MPI_FLOAT_INT);
SPECIALIZE_MPI_TRAIT(value_location<TCDouble>, MPI_DOUBLE_INT);

#define OP_MIN FALSE
#define OP_MAX TRUE

namespace Descriptive
{
namespace Sort
{

// Input data doesn't contain NaNs and has at least one element
// if even == false, then returns a k-th element in sorted array
// if even == true, then returns an interpolation of k & k+1 elements (zero-based) in the sorted array
template <typename T, bool even, typename TIndex, typename interpolation>
T quick_select_dist(std::vector<T>& arr, TIndex total_elems, TIndex k, MPI_Comm comm, const interpolation &median = interpolation())
{
    total_elems;
    // TODO parameter consistency via assert (k>0, k or k+1 < total_elems, etc)
    if (even)
    {
        k++;
    }
    k++; // zero-based -> 1-based

    TCSize_t len = arr.size();
    
    TCInt32 rank;
    MPI_Comm_rank(comm, &rank);

    std::sort(arr.begin(), arr.end());

    value_location<T> in, out;
    T tmp_val;

    TIndex minIdx = 0;
    T maximum_value = std::numeric_limits<T>::max();
    T value_to_compare_with = out.val = arr[0];
    for (TIndex i = 0; i < k; ++i)
    {
        in.val = value_to_compare_with;
        in.rank = rank;

        tmp_val = out.val;

        MPI_Allreduce(&in, &out, 1, mpi_traits<value_location<T> >::mpi_type(), MPI_MINLOC, comm); 

        if (out.rank == rank)
        {
            ++minIdx;
            if (minIdx < len)
            {
                value_to_compare_with = arr[minIdx];
            }
            else
            {
                minIdx = len - 1;
                value_to_compare_with = maximum_value;
            }
        }
    }

    if (even)
    {
        return median.get(tmp_val, out.val);
    }
    else
    {
        return out.val;
    }
}

//Interpolation
template<typename T>
struct interpolation_with_d_t_dist
{
    T d;

    interpolation_with_d_t_dist(){}

    interpolation_with_d_t_dist(T d)
    {
        this->d = d;
    }
    // a <= b
    inline T get(T a, T b) const
    {
        T Inf = Utils::infinity<T>();
        if (Inf == a)
        {
            return Inf;
        }
        if (-Inf == a)
        {
            if (Inf == b && d > 0)
            {
               return TCError::quiet_NaN<T>();
            }
            return -Inf;
        }
        if (Inf == b)
        {
            if (d > 0)
            {
                return b;
            }
            return a;
        }

        return a * (1 - d) + b * d;
    }

    inline T get(T a) const
    {
        return a;
    }
};

// Input data doesn't contain NaNs.
// if even == false, then returns a k-th element in sorted array
// if even == true, then returns an interpolation of k & k+1 elements in the sorted array
template <typename T, bool even, typename T_interpolation>
void select_dist(T* data, TCSize_t stride, TCSize_t* nelems, TCSize_t* gnelems, TCSize_t* k, TCSize_t* status, TCSize_t nvectors, T* v, T_interpolation &median, TCBool in_op, MPI_Comm& comm, int np)
{
    MPI_Op op = (in_op == OP_MIN)? MPI_MIN: MPI_MAX;

    std::list<TCSize_t> vidx;
    std::vector<TCBool> firstval;
    std::vector<TCSize_t> gstatus;

    std::vector<T> in_data;
    std::vector<T> out_data;

    gstatus.resize(nvectors);
    in_data.resize(nvectors);
    out_data.resize(nvectors);

    if(even)
    {
        firstval.resize(nvectors);
        memset((void*) firstval.data(), 0, nvectors * sizeof(TCBool));
    }

    for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
    {
        if(!TCError::is_qnan<T>(v[ivector]))
        {
            vidx.push_back(ivector);
        }
    }

    MPI_Allreduce((void*) status, (void*) gstatus.data(), nvectors, mpi_traits<TCSSize_t>::mpi_type(), MPI_SUM, comm);

	for(TCSize_t ivector = 0; ivector < nvectors; ivector++)
	{
		if(k[ivector] < gstatus[ivector])
		{
			status[ivector] = 0;
			gstatus[ivector] = 0;
		}
	}

    while(!vidx.empty())
    {
        std::list<TCSize_t>::iterator it = vidx.begin();
        TCSize_t i = 0;

        for(; it != vidx.end(); i++, it++)
        {
            TCSize_t ivector = *it;
            TCSize_t step = (k[ivector] - gstatus[ivector])/np;
            TCSize_t nelem = status[ivector] + step;

            if(nelem >= nelems[ivector])
            {
                if(status[ivector] == nelems[ivector])
                {
					T inf_val = std::numeric_limits<T>::infinity();
                    in_data[i] = (in_op == OP_MIN)? inf_val: -inf_val;
                }
                else
                {
                    in_data[i] = data[ivector * stride + nelems[ivector] - 1];
                }
            }
            else
            {
                in_data[i] = data[ivector * stride + nelem];
            }
        }

        MPI_Allreduce((void*) in_data.data(), (void*) out_data.data(), vidx.size(), mpi_traits<T>::mpi_type(), op, comm);

        it = vidx.begin();
        i = 0;
        for(; it != vidx.end(); i++, it++)
        {
            TCSize_t ivector = *it;
            TCSize_t j = status[ivector];
            
            while((j < nelems[ivector]) && (data[ivector * stride + j] <= out_data[i]))
            {
                j++;
            }

            if(j < nelems[ivector])
            {
                status[ivector] = j;
            }
            else
            {
                status[ivector] = nelems[ivector];
            }
        }

        MPI_Allreduce((void*) status, (void*) gstatus.data(), nvectors, mpi_traits<TCSSize_t>::mpi_type(), MPI_SUM, comm);

        std::list<TCSize_t>::iterator itDel = vidx.begin();
        TCSize_t vsize = vidx.size();
        it = vidx.begin();
        i = 0;
        for(; i < vsize; i++)
        {
            TCSize_t ivector = *it;

            if(k[ivector] < gstatus[ivector])
            {
                if(even)
                {
                    if(firstval[ivector])
                    {
                        v[ivector] = median[ivector].get(v[ivector], out_data[i]);
                        itDel = it++;
                        vidx.erase(itDel);
                    }
                    else
                    {
                        if(k[ivector] == (gnelems[ivector] - 1))
                        {
                            v[ivector] = median[ivector].get(out_data[i]);
                            itDel = it++;
                            vidx.erase(itDel);
                        }
                        else
                        {
                            k[ivector]++;

                            if(k[ivector] < gstatus[ivector])
                            {
                                v[ivector] = median[ivector].get(out_data[i], out_data[i]);
                                itDel = it++;
                                vidx.erase(itDel);
                            }
                            else
                            {
                                v[ivector] = out_data[i];
                                firstval[ivector] = TRUE;
                                it++;
                            }
                        }
                    }
                }
                else
                {
                    v[ivector] = out_data[i];
                    itDel = it++;
                    vidx.erase(itDel);
                }
            }
            else
            {
                it++;
            }
        }
    }
}

} /* namespace DistSort */
} /* namespace Descriptive */ 
