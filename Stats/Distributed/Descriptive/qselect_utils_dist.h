



#pragma once

#include <vector>
#include <mpi.h>

#include "TCArray.h"
#include "TCArrayUtils.h"
#include "TCCommon.h"
#include "TCCommonDist.h"
#include "TCArrayUtilsInternal.h"

#include "../Descriptive/qselect.h"
#include "../Descriptive/qselect_utils.h"

namespace Descriptive
{
namespace Sort
{

#define DISABLE_RANK 0
#define ENABLE_RANK  1


template <typename T>
struct TCVectorIterator
{
private:
    TCArray* m_array;

    TCSize_t nelem;
    TCSize_t nvector;
    TCSize_t stride_low;

public:
    TCVectorIterator(TCArray* arr, TCSize_t dim)
    {
        m_array = arr;

        if(m_array->m_numelt > 0)
        {
            nelem = m_array->m_dims[dim - 1];
            nvector = m_array->m_numelt / nelem;
            stride_low = m_array->m_strides[dim -1];
        }
        else
        {
            nelem = 0;
            nvector = 0;
            stride_low = 0;
        }
    };

    inline T* GetVector(TCSize_t idx)
    {
        if(idx >= nvector)
        {
            return NULL;
        }

        TCSize_t chunk_outer = idx / stride_low;
        TCSize_t chunk_inner = idx % stride_low;

        return ((T*)m_array->m_data) + chunk_outer * nelem * stride_low + chunk_inner;
    }

    inline TCSize_t GetCount()
    {
        return nvector;
    }

    inline TCSize_t GetSize()
    {
        return nelem;
    }

    inline TCSize_t GetStride()
    {
        return stride_low;
    }

    inline TCErrorCode GetElement(TCSize_t vidx, TCSize_t elidx, T* val)
    {
        T* v = GetVector(vidx);

        if((v == NULL) || (elidx >= nelem))
        {
            return TCError_Internal;
        }

        *val = v[elidx * stride_low];

        return TCError_NoError;
    }

    inline TCErrorCode PutElement(TCSize_t vidx, TCSize_t elidx, T& val)
    {
        T* v = GetVector(vidx);

        if((v == NULL) || (elidx >= nelem))
        {
            return TCError_Internal;
        }

        v[elidx * stride_low] = val;
        
        return TCError_NoError;
    }
};



template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process_missings_and_nans_reduce_all(OP::T_state& state, TCSize_t& size, const TCDistArray* numbers, TCSize_t narrays)
{
    typedef bool (*missingCheckFcnPtr)(OP::in_t&);
    missingCheckFcnPtr isMissing;
    size = 0;

    for (TCSize_t k = 0; k < narrays; ++k)
    {
        TCSize_t nelem_array = numbers[k].m_LocalArray.m_numelt;

        if (0 == nelem_array)
        {
            continue;
        }

        isMissing = numbers[k].m_LocalArray.m_isref ? MissingPolicyRef<OP::in_t>::ismissing :
            MissingPolicyNotRef<OP::in_t>::ismissing;

        OP::in_t* valArr = (OP::in_t*)numbers[k].m_LocalArray.m_data;
        for (TCSize_t i = 0; i < nelem_array; ++i)
        {
            if (!isMissing(valArr[i]))
            {
                state[size] = valArr[i];
                ++size;
            }

            
            



        }
    }

    return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
}


template <template <class> class MissingPolicy, class OP>
void process_missings_and_nans_reduce_dim(OP &op, const TCArray *in_array, OP::T_state& state, std::vector<TCSize_t>& nelems, TCSize_t dim)
{
    TCSize_t numelt = in_array->m_numelt;
    TCSize_t max_nelem = in_array->m_dims[dim - 1];
    TCSize_t nvector = numelt / max_nelem;
    TCSize_t stride_low = in_array->m_strides[dim - 1];
    TCSize_t chunks_outer = nvector / stride_low;

    op.InitState(state, numelt);
    nelems.resize(nvector);
    memset(nelems.data(), 0, nvector * sizeof(TCSize_t));
    
    TCSize_t i;
    TCSize_t chunk;
    OP::in_t v;
    const OP::in_t* base;

    for (TCSize_t chunk_outer = 0; chunk_outer < chunks_outer; chunk_outer++)
    {
        TCSize_t base_element = chunk_outer * max_nelem * stride_low;
        for (TCSize_t chunk_inner = 0; chunk_inner < stride_low; chunk_inner++, base_element++)
        {
            base = ((OP::in_t*)in_array->m_data) + base_element;
            i = 0;
            v = base[i * stride_low];

            if (MissingPolicy<OP::in_t>::ismissing(v))
            {
                i++;
                while (i < max_nelem)
                {
                    v = base[i * stride_low];
                    if (!MissingPolicy<OP::in_t>::ismissing(v))
                    {
                        break;
                    }
                    i++;
                }
            }
            for (; i < max_nelem; i++)
            {
                v = base[i * stride_low];
                if (!MissingPolicy<OP::in_t>::ismissing(v))
                {
                    chunk = chunk_outer * stride_low + chunk_inner;
                    op.Process(state, v, chunk * max_nelem + nelems[chunk]);
                    nelems[chunk]++;
                }
            }
        }
    }
}

struct operator_temp_copy_dist_t
{
    template <class OP>
    inline static TCErrorCode getDistributedResult(OP& op, OP::T_state& state, TCSize_t &nelem, MPI_Comm comm, TCInt32 rank, TCInt32 destRank, OP::ou_t *out)
    {
        typedef OP::in_t T;

        TCInt32 total_ranks;
        MPI_Comm_size(comm, &total_ranks);
        TCInt32 masterRank = destRank == ALL_RANKS ? MASTER_RANK : destRank;

        
        TCSSize_t total_elem = 0;
        MPI_Allreduce(&nelem, &total_elem, 1, mpi_traits<TCSSize_t>::mpi_type(), MPI_SUM, comm);

        TCErrorCode err = 0;
        if (rank == masterRank)
        {
            state.resize(total_elem);

            MPI_Status status;
            for (int i = 0; i < total_ranks; i++)
            {
                if (i == masterRank)
                {
                    continue;
                }
                int sz = 0;
                MPI_Recv(&sz, 1, MPI_INT, i, 1, comm, &status);
                T dummy; 
                MPI_Recv(sz == 0 ? &dummy : &state[nelem], sz, mpi_traits<T>::mpi_type(), i, 2, comm, &status);
                nelem += sz;
            }

            err = op.GetResult(state, nelem, *out);
        }
        else
        {
            int sz = (int)nelem;
            MPI_Send(&sz, 1, MPI_INT, masterRank, 1, comm);
            T dummy; 
            MPI_Send(sz == 0 ? &dummy : &state[0], sz, mpi_traits<T>::mpi_type(), masterRank, 2, comm);
            nelem = -1;
        }
        MPI_Bcast(&err, 1, MPI_UNSIGNED_SHORT, masterRank, comm);
        return err;
    }
};

struct operator_qselect_dist_t
{
    template <class OP>
    inline static TCErrorCode getDistributedResult(OP& op, OP::T_state& state, TCSize_t &nelem, MPI_Comm comm, TCInt32 rank, TCInt32 destRank, OP::ou_t *out)
    {
        state.resize(nelem);

        
        TCSSize_t total_elem = 0;
        TCSSize_t nelem_s = nelem;
        MPI_Allreduce(&nelem_s, &total_elem, 1, mpi_traits<TCSSize_t>::mpi_type(), MPI_SUM, comm);

        
        

        int color = (0 == nelem ? MPI_UNDEFINED : 0);

        
        MPI_Comm newcomm = MPI_COMM_NULL;
        MPI_Comm_split(comm, color, rank, &newcomm);

        TCErrorCode err = tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
        OP::ou_t result = tcerror_code_new(TCError_Internal, TCArgPosition_UnKnown);
        if (MPI_COMM_NULL != newcomm)
        {
            err = op.GetResultDist(state, total_elem, newcomm, result);
            if (destRank == rank)
                *out = result;
            MPI_Comm_free(&newcomm);
        }

        TCSSize_t err_s = (TCSSize_t)err;
        TCSSize_t total_err_s = 0;
        MPI_Allreduce(&err_s, &total_err_s, 1, mpi_traits<TCSSize_t>::mpi_type(), MPI_MAX, comm); 

        return (TCErrorCode)total_err_s;
    }
};


#define MAX_RESERVED_ELEMENTS (1024*1024)


template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process_all_dist(OP &op, const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** value)
{
    TCErrorID err(TCError_NoError);

    MPI_Comm comm = (MPI_Comm)numbers->m_Layout.m_Comm;
    int rank;
    MPI_Comm_rank(comm, &rank);

    
    if (0 == dim)
    {
        
        size_t telem = 0;
        size_t total_local_elem = 0;
        for (TCSize_t i = 0 ; i < narrays; i++)
        {
            telem += numbers[i].m_Layout.m_GlobalSize;
            total_local_elem += numbers[i].m_LocalArray.m_numelt;
        }
        
        if (telem == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }
        assert(value[0]->m_Layout.m_GlobalSize == 1);

        
        OP::T_state state;
        TCSSize_t destRank = TCDistArray_Get_MasterRank(*(value[0])); 
        TCInt32 masterRank = destRank == ALL_RANKS ? MASTER_RANK : destRank;
        TCSSize_t init_err;
        if (rank == masterRank && telem <= MAX_RESERVED_ELEMENTS)
        {
            
            init_err = (TCSSize_t)op.InitState(state, rank == masterRank ? telem : total_local_elem);
        }
        else
        {
            init_err = (TCSSize_t)op.InitState(state, total_local_elem);
        }
        TCSSize_t total_init_errors = 0;
        MPI_Allreduce(&init_err, &total_init_errors, 1, mpi_traits<TCSSize_t>::mpi_type(), MPI_MAX, comm);
        if (0 != total_init_errors)
        {
            return tcerror_code_new(TCError_OutOfMemory, TCArgPosition_UnKnown);
        }

        TCSize_t nelem = 0;
        TCSize_t k = 0;
        do {
            while(k < narrays && numbers[k].m_LocalArray.m_numelt == 0) k++;
            if (k < narrays)
            {
                const OP::in_t* base = (OP::in_t*)numbers[k].m_LocalArray.m_data;
                TCSize_t nelem_array = numbers[k].m_LocalArray.m_numelt;
                bool isref = numbers[k].m_LocalArray.m_isref;
                TCSize_t i = 0;
                OP::in_t v;
                if (isref)
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                else
                {
                    while (i < nelem_array)
                    {
                        v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            break;
                        }
                        i++;
                    }
                }
                if (i < nelem_array)
                {
                    op.ProcessFirst(state, v);
                    i++; nelem++;
                }
                else
                {
                    k++;
                    continue;
                }
                if (isref)
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                else
                {
                    for (; i < nelem_array; i++)
                    {
                        OP::in_t v = base[i];
                        if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                        {
                            op.Process(state, v, nelem); nelem++;
                        }
                    }
                }
                k++;
            }
        } while (0);
        for (; k < narrays; k++)
        {
            const OP::in_t* base = (OP::in_t*)numbers[k].m_LocalArray.m_data;
            TCSize_t nelem_array = numbers[k].m_LocalArray.m_numelt;
            if (numbers[k].m_LocalArray.m_isref)
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
            else
            {
                for (TCSize_t i = 0; i < nelem_array; i++)
                {
                    OP::in_t v = base[i];
                    if (!MissingPolicyNotRef<OP::in_t>::ismissing(v))
                    {
                        op.Process(state, v, nelem); nelem++;
                    }
                }
            }
        }
        return op.getDistributedResult(op, state, nelem, comm, rank, destRank, (OP::ou_t*)(value[0]->m_LocalArray.m_data));
    }
    else
    {
        
        const TCArray* numbersLocal = &(numbers[0].m_LocalArray);
        TCArray* valueLocal = &(value[0]->m_LocalArray);

        
        if (narrays != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (dim > numbersLocal->m_ndims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (numbers[0].m_Layout.m_GlobalSize == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }

        
        if ((dim - 1) == numbers->m_Layout.m_DistDim)
        {
            
            

            TCSize_t myID = TCDistArray_Get_Rank(*numbers);
            TCSize_t rankCount = TCDistArray_Get_NProcs(*numbers);

            TCArray* fullNumbers = 0;
            TCArray* fullValue = 0;

            TCTypeTag tagNumbers = numbers->m_LocalArray.m_tag;
            TCTypeTag tagValue = (*value)->m_LocalArray.m_tag;

            TCErrorCode code;

            if (myID == MASTER_RANK)
            {
                code = TCArray_New_ND(tagNumbers, numbers->m_Layout.m_GlobalDims, numbers->m_Layout.m_GlobalShape, 1, &fullNumbers);
                code = TCArray_New_ND(tagValue, (*value)->m_Layout.m_GlobalDims, (*value)->m_Layout.m_GlobalShape, 1, &fullValue);
            }

            code = TCDistArray_Gather(numbers, MASTER_RANK, fullNumbers);

            TCDistArray* newNumbers = NULL;
            TCDistArray* newValue = NULL;

            TCSize_t newDistDim = 0;
            if (myID == MASTER_RANK)
            {
                
                
                
                TCSize_t piecesCount = 0;
                TCSize_t rankOverhead = 0;
                TCSize_t rankOverheadMin = rankCount + 1;
                for(TCSize_t i = 0; i < numbers->m_Layout.m_GlobalDims; i++)
                {
                    if (i == numbers->m_Layout.m_DistDim)
                    {
                        continue;
                    }
                    piecesCount = numbers->m_Layout.m_GlobalShape[i] / 256;
                    rankOverhead = piecesCount % rankCount;
                    if (rankOverhead < rankOverheadMin)
                    {
                        rankOverheadMin = rankOverhead;
                        newDistDim = i;
                    }
                }

            }

            MPI_Bcast(&newDistDim, 1, mpi_traits<TCSize_t>::mpi_type(), MASTER_RANK, comm);

            code = TCDistArray_New_ND(tagNumbers, numbers->m_Layout.m_GlobalDims, numbers->m_Layout.m_GlobalShape, newDistDim, 1, &newNumbers);
            code = TCDistArray_New_ND(tagValue, (*value)->m_Layout.m_GlobalDims, (*value)->m_Layout.m_GlobalShape,
                newDistDim > dim - 1 ? newDistDim - 1 : newDistDim,
                1, &newValue);

            code = TCDistArray_Scatter(fullNumbers, MASTER_RANK, newNumbers);

            const TCArray* numbersLocal = &(newNumbers[0].m_LocalArray);
            TCArray* valueLocal = &((&newValue)[0]->m_LocalArray);
            if (numbersLocal->m_numelt > 0)
            {
                if (numbersLocal->m_isref)
                {
                    err = process_dim<MissingPolicyRef, OP>(op, numbersLocal, valueLocal, dim);
                }
                else
                {
                    err = process_dim<MissingPolicyNotRef, OP>(op, numbersLocal, valueLocal, dim);
                }
            }

            code = TCDistArray_Gather(newValue, MASTER_RANK, fullValue);
            code = TCDistArray_Scatter(fullValue, MASTER_RANK, *value);

            code = TCDistArray_Delete(&newNumbers);
            code = TCDistArray_Delete(&newValue);      
        }
        else
        {
            assert(numbers->m_Layout.m_DistDim == (*value)->m_Layout.m_DistDim);
            if (numbersLocal->m_numelt > 0)
            {
                if (numbersLocal->m_isref)
                {
                    err = process_dim<MissingPolicyRef, OP>(op, numbersLocal, valueLocal, dim);
                }
                else
                {
                    err = process_dim<MissingPolicyNotRef, OP>(op, numbersLocal, valueLocal, dim);
                }
            }
        }
    } 

    return tcerror_code_new(err, TCArgPosition_UnKnown);
}


template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process2vector_dim_dist(OP &op, const TCDistArray *numbers, TCDistArray** value, TCSize_t dim)
{
    int rank;
    MPI_Comm newcomm;
    MPI_Comm comm = (MPI_Comm)numbers->m_Layout.m_Comm;

    MPI_Comm_rank(comm, &rank);

    const TCArray* numbersLocal = &(numbers->m_LocalArray);
    TCArray* valueLocal = &((*value)->m_LocalArray);

    if((numbersLocal->m_numelt > 0) || (valueLocal->m_numelt > 0))
    {
        std::vector<TCSize_t> nelems;
        OP::T_state state;
        
        if(numbersLocal->m_numelt > 0)
        {
            if(numbersLocal->m_isref)
            {
                process_missings_and_nans_reduce_dim<MissingPolicyRef>(op, numbersLocal, state, nelems, dim);
            }
            else
            {
                process_missings_and_nans_reduce_dim<MissingPolicyNotRef>(op, numbersLocal, state, nelems, dim);
            }
        }

        MPI_Comm_split(comm, ENABLE_RANK, rank, &newcomm);

        TCVectorIterator<OP::ou_t> vit(valueLocal, dim);

        op.GetResultDist(state, nelems, &vit, (*value)->m_Layout.m_Spans + rank, newcomm);
            
        MPI_Comm_free(&newcomm);

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
    
    MPI_Comm_split(comm, DISABLE_RANK, rank, &newcomm);
    MPI_Comm_free(&newcomm);

    return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
}


template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process2vector_all_dist(OP &op, const TCDistArray *numbers, TCSize_t narrays, TCDistArray** value)
{
    TCSize_t gt_elements = 0;
    TCSize_t lt_elements = 0;

    for (TCSize_t i = 0; i < narrays; ++i)
    {
        gt_elements += numbers[i].m_Layout.m_GlobalSize;
        lt_elements += numbers[i].m_LocalArray.m_numelt;
    }

    if(gt_elements == 0)
    {
        return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
    }

    int rank;
    MPI_Comm newcomm;
    MPI_Comm comm = (MPI_Comm)numbers->m_Layout.m_Comm;

    MPI_Comm_rank(comm, &rank);

    TCArray* valueLocal = &((*value)->m_LocalArray);

    if((lt_elements > 0) || (valueLocal->m_numelt > 0))
    {
        std::vector<TCSize_t> nelems;
        OP::T_state state;
    
        op.InitState(state, lt_elements);
        nelems.push_back(0);

        if(lt_elements > 0)
        {
            process_missings_and_nans_reduce_all<MissingPolicyRef, MissingPolicyNotRef, OP>(state, nelems[0], numbers, narrays);
        }

        MPI_Comm_split(comm, ENABLE_RANK, rank, &newcomm);

        TCVectorIterator<OP::ou_t> vit(valueLocal, 1);
        TCSpan span;

        if((*value)->m_Layout.m_DistDim > 0)
        {
            span.EndIdx = 0;
            span.StartIdx = (*value)->m_Layout.m_GlobalSize;
        }
        else
        {
            memcpy(&span, (*value)->m_Layout.m_Spans + rank, sizeof(TCSpan));
        }

        op.GetResultDist(state, nelems, &vit, &span, newcomm);
            
        MPI_Comm_free(&newcomm);

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
    
    MPI_Comm_split(comm, DISABLE_RANK, rank, &newcomm);
    MPI_Comm_free(&newcomm);

    return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
}



template <template <class> class MissingPolicyRef, template <class> class MissingPolicyNotRef, class OP> 
TCErrorCode process2vector_dist(OP &op, const TCDistArray* numbers, TCSize_t narrays, TCSize_t dim, TCDistArray** value)
{
    if (0 == dim)
    {
        return process2vector_all_dist<SkipMissingValues, ConverMissingValuesTo0>(op, numbers, narrays, value);
    }
    else
    {
        
        if (narrays != 1)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (dim > numbers[0].m_Layout.m_GlobalDims)
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgPosition_UnKnown);
        }
        if (numbers[0].m_Layout.m_GlobalSize == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgPosition_UnKnown);
        }

        if ((dim - 1) == numbers->m_Layout.m_DistDim)
        {
            return process2vector_dim_dist<SkipMissingValues, ConverMissingValuesTo0>(op, numbers, value, dim);
        }
        else
        {
            TCErrorID err(TCError_NoError);

            const TCArray* numbersLocal = &(numbers[0].m_LocalArray);
            TCArray* valueLocal = &(value[0]->m_LocalArray);

            if(numbersLocal->m_numelt > 0)
            {
                if (numbersLocal->m_isref)
                {
                    err = process2vector_dim<Sort::SkipMissingValues>(op, numbersLocal, valueLocal, dim);
                }
                else
                {
                    err = process2vector_dim<Sort::ConverMissingValuesTo0>(op, numbersLocal, valueLocal, dim);
                }
            }

            return tcerror_code_new(err, TCArgPosition_UnKnown);
        }
    }
}

} 
} 
