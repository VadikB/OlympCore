



#include <stdlib.h>
#include "TCErrorCommon.h"
#include "OlympCoreUtils.h"
using namespace TCError;

namespace Descriptive
{   
  
    template <typename T>
    TCErrorCode stats_pearson(const TCArray* array1, const TCArray* array2, T* r_value)
    {
        TCErrorCode ret = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        T value = 0, sum2, mean2, sum;
        T count, sum1, mean1;
        T sumsqr1, sumsqr2;
        TCSize_t numelt1 = array1-> m_numelt;
        TCSize_t numelt2 = array2-> m_numelt;

        
        if ((numelt1 != numelt2) || (numelt1 <= 1))
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }
        
        
        count = 0, sum1 = 0, sum2 = 0;
        for (TCSize_t ind = 0; ind < numelt1; ind ++)
        {
            T a1 = ( (T*)(array1 -> m_data) ) [ind];
            T a2 = ( (T*)(array2 -> m_data) ) [ind];

            
            if (TCError::IsMissingImpl<T>::run(&a1) || TCError::IsMissingImpl<T>::run(&a2))
            {
                continue;
            }

            sum1 += a1;
            sum2 += a2;
            count++;
        }
        
        if (count == 0)
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }
        mean1 = sum1 / count;
        mean2 = sum2 / count;

        
        sum = 0, sumsqr1 = 0, sumsqr2 = 0;
        for (TCSize_t ind = 0; ind < numelt1; ind ++)
        {
            T a1 = ( (T*)(array1 -> m_data) ) [ind];
            T a2 = ( (T*)(array2 -> m_data) ) [ind];

            
            if (TCError::IsMissingImpl<T>::run(&a1) || TCError::IsMissingImpl<T>::run(&a2))
            {
                continue;
            }
            sum += (a1 - mean1) * (a2 - mean2);
            sumsqr1 += (a1 - mean1) * (a1 - mean1);
            sumsqr2 += (a2 - mean2) * (a2 - mean2);
        }
        value = sum / Utils::sqrt(sumsqr1 * sumsqr2);

exitlabel:
        *r_value = value;
        return ret;
    }

    template <typename T>
    TCErrorCode stats_pearsonmat(const TCArray* data, TCArray* r_matrix)
    {
        TCErrorCode ret = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        TCSize_t dim1 = data-> m_dims[0];
        TCSize_t dim2 = data-> m_dims[1];
        T* mean = NULL;
        T* deviation = NULL;
        T devsq1 = 0, devsq2 = 0;
        
        
        if (dim1 == 0)
        {
            ret = tcerror_code_new(TCError_IllegalSize, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        
        if (data -> m_ndims != 2 || r_matrix -> m_ndims != 2)
        {
            ret = tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        
        if ( (dim2 != r_matrix -> m_dims[0]) || (dim2 != r_matrix -> m_dims[1]) )
        {
            ret = tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        mean = new T[dim2];
        if (NULL == mean)
        {
            ret = tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
            BAIL();
        }
        
        deviation = new T[dim1 * dim2];
        if (NULL == deviation)
        {
            ret = tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
            BAIL();
        }

        
        for (TCSize_t i2 = 0; i2 < dim2; i2++)
        {
            mean[i2] = 0;
            for (TCSize_t i1 = 0; i1 < dim1; i1++)
            {
                mean[i2] += ( (T*)(data -> m_data) ) [i1+i2*dim1];
            }
            mean[i2] /= static_cast<T>(dim1);
        }

        
        for (TCSize_t i2 = 0; i2 < dim2; i2++)
        {
            for (TCSize_t i1 = 0; i1 < dim1; i1++)
            {
                deviation[i1+i2*dim1] = ( (T*)(data -> m_data) ) [i1+i2*dim1] - mean[i2];
            }
        }

        for (TCSize_t i1 = 0; i1 < dim2; i1++)
        {
            for (TCSize_t i2 = 0; i2 < dim2; i2++)
            {
                ( (T*)(r_matrix -> m_data) ) [i1+i2*dim2] = 0;
                for (TCSize_t i3 = 0; i3 < dim1; i3++)
                {
                    ( (T*)(r_matrix -> m_data) ) [i1+i2*dim2] += deviation[i3+i1*dim1] * deviation[i3+i2*dim1];
                    devsq1 += deviation[i3+i1*dim1] * deviation[i3+i1*dim1];
                    devsq2 += deviation[i3+i2*dim1] * deviation[i3+i2*dim1];
                }
                ( (T*)(r_matrix -> m_data) ) [i1+i2*dim2] /= Utils::sqrt (devsq1 * devsq2);
            }
        }

exitlabel:
        delete []mean;
        delete []deviation;
        return ret;
    }
}

