



#pragma once

#include "..\stdafx.h"
#include "TCArrayUtils.h"
#include "Descriptive\average.h"
#include "TCArrayUtilsInternal.h"

using namespace Utils;

namespace Imputation
{
    template <typename T>
    static TCErrorCode imputation_mean_column(TCArray* data_in, TCSize_t indx_start, TCSize_t indx_end, T mean, TCUInt64& num_errors)
    {
        T* data = (T*)data_in->m_data;
        for (TCSize_t indx = indx_start; indx < indx_end; ++indx)
            {
                T* value = &(data[indx]); 
                if (TCError::IsMissing<T>(value)) 
                { 
                    data[indx] = mean; 
                }
                if (isnan(*value)) 
                {
                    num_errors++;
                }
            }
        return TCError_NoError;
    }

    template <typename T>
    static TCErrorCode count_errors(TCArray* data_in, TCSize_t indx_start, TCSize_t indx_end, TCUInt64& num_errors)
    {
        T* data = (T*)data_in->m_data;
        for (TCSize_t indx = indx_start; indx < indx_end; ++indx)
            {
                if (isnan(data[indx]))
                { 
                    num_errors++;
                }
            }
        return TCError_NoError;
    }

    template <typename T>
    static TCErrorCode imputation_mean(TCArray* data_in, TCUInt64* not_replaced)
    {
        TCErrorCode err_code = TCError_Internal;
        TCUInt64 count = 0;
        T column_mean = 0.;
        T temp_mean;
        TCSize_t num = data_in->m_numelt;
        TCSize_t ndims = data_in->m_ndims;
        TCSize_t numrows;
        TCSize_t numcols;
        TCSize_t stride;
        
        *not_replaced = 0;
        
        if (num == 0)
        {
            return TCError_NoError; 
        }

        switch (ndims)
        {
        case 1:
            {
                
                err_code = Descriptive::stats_redall_average<T>(data_in, 1, &column_mean);
                if (TCError::IsMissing<T>(&column_mean))
                {
                    err_code = count_errors<T>(data_in, 0, num, count);
                }
                else 
                {
                    err_code = imputation_mean_column<T>(data_in, 0, num, column_mean, count);
                }
            }
            break;
        case 2:
            {
                
                TCArray* column_means; 
                numrows = data_in->m_dims[0];
                numcols = data_in->m_dims[1];

                err_code = array_allocate1d<T>(numcols, &column_means); 
                if (err_code != TCError_NoError)
                {
                    break;
                }
                TCArrayUniquePtr column_meansPtr(column_means);

                Descriptive::stats_reddim_average<T>(data_in, 1, column_means);

                
                stride = (TCSize_t)data_in->m_strides[1];
                for (TCSize_t ncol = 0; ncol < numcols; ++ncol)
                {
                    temp_mean = ((T*)(column_means->m_data))[ncol];
                    TCSize_t indx_start = stride * ncol; 

                    if (isnan(temp_mean))
                    {
                        err_code = count_errors<T>(data_in, indx_start, indx_start + numrows, count);
                    }
                    else 
                    {
                        err_code = imputation_mean_column<T>(data_in, indx_start, indx_start + numrows, temp_mean, count);
                    }
                    if (err_code != TCError_NoError)
                    {
                        break;
                    }
                }
            }
            break;
        default:
            err_code = tcerror_code_new(TCError_IllegalSize, 1);
            break;
        }
        *not_replaced = count;
        return err_code;
    }; 
} 
