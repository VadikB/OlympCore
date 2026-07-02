



#include "stdafx.h"
#include "TCArrayUtils.h"
#include <TCErrorCommon.h>
#include <TCFuncWrapper.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>

#pragma region Class for pair sorting
template <typename T>
class IntTPairs 
{
public:
    TCSize_t ind;
    T val;
    IntTPairs(TCSize_t c = 0, T d = 0): ind(c), val(d) {}
    bool operator == (const IntTPairs& y)
    { return val == y.val; }
    bool operator < (const IntTPairs& y) 
    { return val < y.val; }
} ;
#pragma endregion

template <typename T>
void makeCDF(TCSize_t na, std::vector<IntTPairs<T>> A_vec, std::vector<T> *cdfA)
{
    TCSize_t i;
    TCSize_t j;
    TCSize_t istart;

    i = na - 1;
    istart = i;
    while (i > 0)
    { 
        if ((A_vec[i].val > A_vec[i - 1].val)) 
        {
            for (j = i; j <= istart; j++)
            {
                (*cdfA)[j] = (T)(istart + 1) / na;
            }
            istart = i - 1;
        }
        i = i - 1;
    }
    for (j = i; j <= istart; j++)
    {
        (*cdfA)[j] = (T)(istart + 1) / na;
    }
}

template <typename T>
bool fillIndex(const TCArray* data, TCUInt64 nvec, std::vector<IntTPairs<T>>* A_vec)
{
    TCSize_t i;
    TCSize_t na = data->m_dims[0];
    T* pdata = ((T*)data->m_data);
    T tref;
    TCSize_t index;

    for (i = 0; i < na; i++)
    {
        (*A_vec)[i].ind = i;  
        
        index = i * data->m_strides[0] + (nvec - 1) * data->m_strides[1];
        tref = pdata[index];
        if (TCError::IsMissingImpl<T>::run(&tref) || isinf<T>(tref) || TCError::is_nan<T>(tref))
        {
            return false; 
        }
        (*A_vec)[i].val = tref;
    }
    std::sort(A_vec->begin(), A_vec->end());
    return true;
}

template <typename T>
TCErrorCode imputation_match_body(const TCArray* data_rec, TCUInt64 nvec_rec, const TCArray* data_don, TCUInt64 nvec_don, TCArray* data_out, TCUInt64* count_missings)
{
    T  tref;
    TCSize_t na;
    TCSize_t ma;
    TCSize_t nb;
    TCSize_t mb;
    TCSize_t i;
    TCSize_t j;
    TCSize_t p;
    TCSize_t index;

    TCBool isempty;
    array_isempty(data_rec, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    array_isempty(data_don, &isempty);
    if (isempty) 
    {
        return tcerror_code_new(TCError_IllegalSize, 3);
    }
    if (data_rec->m_ndims != 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (data_don->m_ndims != 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 3);
    }
    *count_missings = 0;
    na = data_rec->m_dims[0];
    ma = data_rec->m_dims[1];
    nb = data_don->m_dims[0];
    mb = data_don->m_dims[1];
    if (nvec_rec < 1 || nvec_rec > ma)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }
    if (nvec_don < 1 || nvec_don > mb)
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }
    
    std::vector<T> cdfA(na);
    std::vector<T> cdfB(nb);
    std::vector<IntTPairs<T>> A_vec(na);
    std::vector<IntTPairs<T>> B_vec(nb);
    T* prec = ((T*)data_rec->m_data);
    T* pdon = ((T*)data_don->m_data);
    T* pout = ((T*)data_out->m_data);

    if (!fillIndex<T>( data_rec, nvec_rec, &A_vec)) 
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1)); 
    }
    if (!fillIndex<T>( data_don, nvec_don, &B_vec)) 
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3)); 
    }
    
    makeCDF(na, A_vec, &cdfA);
    makeCDF(nb, B_vec, &cdfB);
    
    
    for (i = 0; i < na; i++)
    {
        for (j = 0; j < ma; j++)
        { 
            index = i * data_rec->m_strides[0] + j * data_rec->m_strides[1];
            tref = prec[index];
            if (TCError::IsMissingImpl<T>::run(&tref) || TCError::is_nan<T>(tref))
            {
                (*count_missings)++;
            }
            pout[i * data_out->m_strides[0] + j * data_out->m_strides[1]] = tref;
        }
    }
    
    p = 0;
    for (i = 0; i < na; i++)
    {
        while (p < nb - 1 && cdfA[i] > cdfB[p]) {p++;}
        if ((p > 0) && ( 2. * cdfA[i] <= (cdfB[p] + cdfB[p - 1] + std::numeric_limits<T>::epsilon()))) {p--;}
        for (j = 0; j < mb; j++)
        { 
            if (j != (nvec_don - 1))
            {
                index = B_vec[p].ind * data_don->m_strides[0] + j * data_don->m_strides[1];
                tref = pdon[index];
                if (TCError::IsMissingImpl<T>::run(&tref) || TCError::is_nan<T>(tref))
                {
                    (*count_missings)++;
                }
                pout[A_vec[i].ind * data_out->m_strides[0] + ((j < (nvec_don - 1) ? j : j - 1) + ma) * data_out->m_strides[1]] = pdon[index];
            }
        }
    }
    return TCError_NoError;
}

C_BEGINF(stats_imputation_match_d_e, const TCArray* data_rec, TCUInt64 nvec_rec, const TCArray* data_don, TCUInt64 nvec_don, TCArray* data_out, TCUInt64* count_missings)
{
    return imputation_match_body<TCDouble>(data_rec, nvec_rec, data_don, nvec_don, data_out, count_missings);
} C_ENDF

C_BEGINF(stats_imputation_match_f_e, const TCArray* data_rec, TCUInt64 nvec_rec, const TCArray* data_don, TCUInt64 nvec_don, TCArray* data_out, TCUInt64* count_missings)
{
    return imputation_match_body<TCFloat>(data_rec, nvec_rec, data_don, nvec_don, data_out, count_missings);
} C_ENDF
