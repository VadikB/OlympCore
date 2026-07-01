//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include <stdlib.h>
#include "TCErrorCommon.h"
using namespace TCError;
using namespace math_linalg_syrk_internal;

namespace Descriptive
{   
    enum setType{sample, population};
  
    template <typename T>
    TCErrorCode stats_cov(const TCArray* array1, const TCArray* array2, T* p_value, setType sampOrPop)
    {
        TCErrorCode ret = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        TCSize_t count = 0, subtract = 0;
        T sum1 = 0, mean1, sum = 0;
        T sum2 = 0, mean2, value = 0;
        TCSize_t numelt1 = array1-> m_numelt;
        TCSize_t numelt2 = array2-> m_numelt;

        // switch for sample covariance of population covariance
        if (sampOrPop == sample)
        {
            subtract = 1;
        }
        else if(sampOrPop == population)
        {
            subtract = 0;
        }
        else
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID));
            BAIL();
        }

        //if no. elements in arrays are not equal, or divide by zero, return illegal input
        if ((numelt1 != numelt2) || (numelt1 <= subtract))
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }
        
        //calculate mean
        count = 0, sum1 = 0.0, sum2 = 0.0;
        for (TCSize_t ind = 0; ind < numelt1; ind ++)
        {
            T a1 = ( (T*)(array1 -> m_data) ) [ind];
            T a2 = ( (T*)(array2 -> m_data) ) [ind];

            //if either a or e is missing, skip this ind
            if (TCError::IsMissingImpl<T>::run(&a1) || TCError::IsMissingImpl<T>::run(&a2))
            {
                continue;
            }

            sum1 += a1;
            sum2 += a2;
            count++;
        }

        //if count <= 1 for sample, or count <= 0 for population, divide by zero occurs
        if (count <= subtract)
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }

        mean1 = sum1 / static_cast<T>(count);
        mean2 = sum2 / static_cast<T>(count);

        //calculate variance using mean
        sum = 0.0;
        for (TCSize_t ind = 0; ind < numelt1; ind ++)
        {
            T a1 = ( (T*)(array1 -> m_data) ) [ind];
            T a2 = ( (T*)(array2 -> m_data) ) [ind];

            //if either a or e is missing, skip this ind
            if (TCError::IsMissingImpl<T>::run(&a1) || TCError::IsMissingImpl<T>::run(&a2))
            {
                continue;
            }
            sum += (a1 - mean1) * (a2 - mean2);
        }
        value = sum / static_cast<T>(count - subtract);

exitlabel:
        *p_value = value;
        return ret;
    }

    template <typename T>
    TCErrorCode stats_covmat(const TCArray* dataMatrix, const TCArray* covMatrix, setType sampOrPop)
    {
        TCErrorCode ret = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        TCSize_t dim1 = dataMatrix-> m_dims[0];
        TCSize_t dim2 = dataMatrix-> m_dims[1];
        TCSize_t subtract;
        T* mean = NULL;
        T* deviation = NULL;

        char uplo, trans;
        T alpha, beta, *mc;
        TCSSize_t n, k, lda, ldc;

        // switch for sample covariance of population covariance
        if (sampOrPop == sample)
        {
            subtract = 1;
        }
        else if(sampOrPop == population)
        {
            subtract = 0;
        }
        else
        {
            ret = tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        //need dim1 > subtract or divide by zero occurs
        if (dim1 <= subtract)
        {
            ret = tcerror_code_new(TCError_IllegalSize, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        //if the number of dimensions of either array is not 2, return TCError_IllegalInput
        if (dataMatrix -> m_ndims != 2 || covMatrix -> m_ndims != 2)
        {
            ret = tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        //if the dimensions of the two arrays are not equal, return illegal input
        if ( (dim2 != covMatrix -> m_dims[0]) || (dim2 != covMatrix -> m_dims[1]) )
        {
            ret = tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        mean = (T*) malloc(sizeof(T) * dim2);
        if (NULL == mean)
        {
            ret = tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
            BAIL();
        }
        deviation = (T*) malloc(sizeof(T) * dim1 * dim2);
        if (NULL == deviation)
        {
            ret = tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        //calculate mean 
        for (TCSize_t i2 = 0; i2 < dim2; i2++)
        {
            mean[i2] = 0;
            for (TCSize_t i1 = 0; i1 < dim1; i1++)
            {
                mean[i2] += ( (T*)(dataMatrix -> m_data) ) [i1+i2*dim1];
            }
            mean[i2] /= static_cast<T>(dim1);
        }

        //calculate deviation
        for (TCSize_t i2 = 0; i2 < dim2; i2++)
        {
            for (TCSize_t i1 = 0; i1 < dim1; i1++)
            {
                deviation[i1+i2*dim1] = ( (T*)(dataMatrix -> m_data) ) [i1+i2*dim1] - mean[i2];
            }
        }

        mc = (T*)covMatrix->m_data;
        uplo='U', trans='T', n=dim2, k=dim1, lda=dim1, beta=0.0, ldc=dim2;
        alpha = 1.0 / static_cast<T>(dim1 - subtract);
        syrk<T, sequential>(&uplo, &trans, &n, &k, &alpha, deviation, &lda, &beta, mc, &ldc);

        // copy upper half of symmetric matrix into lower half
        for (TCSize_t i1 = 0; i1 < dim2 - 1; i1++)
        {
            for (TCSize_t i2 = i1 + 1; i2 < dim2; i2++)
            {
                ( (T*)(covMatrix -> m_data) ) [i2+i1*dim2] = ( (T*)(covMatrix -> m_data) ) [i1+i2*dim2];
            }
        }

exitlabel:
        free (mean);
        free (deviation);
        return ret;
    }
}

