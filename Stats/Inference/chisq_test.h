



#include "Descriptive\devsq.h"
#include "chisq.h"
#include "TCErrorCommon.h"

using namespace TCError;
using namespace Dist;

namespace Inference 
{   
  
    template <typename T>
    TCErrorCode stats_inference_chisq_test(const TCArray* actual_range, const TCArray* expected_range, T* p_value)
    {
        TCErrorCode ret = tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        T value = 0;
        T df = 0;
        TCSize_t numrows = 0, numcols = 0;
        T sum =0;

        
        if (actual_range -> m_ndims != 2 || expected_range -> m_ndims != 2)
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }

        
        if ( (actual_range-> m_dims[0] != expected_range -> m_dims[0]) || (actual_range-> m_dims[1] != expected_range -> m_dims[1]) )
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }
        
        
        if ( (actual_range-> m_dims[0] <= 1) && (actual_range-> m_dims[1] <= 1) )
        {
            value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
            BAIL();
        }        

        
        sum = 0;
        for (TCSize_t ind = 0; ind < actual_range -> m_numelt; ind ++)
        {
            T a = ( (T*)(actual_range -> m_data) ) [ind];
            T e = ( (T*)(expected_range -> m_data) ) [ind];

            
            if (TCError::IsMissingImpl<T>::run(&a) || TCError::IsMissingImpl<T>::run(&e))
            {
                continue;
            }

            
            if (a < 0 || e <= 0)
            {
                value = NewErrorImpl<T>::run (tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID));
                BAIL();
            }

            sum += (a-e)*(a-e)/e;
        }
        
        
        numrows = actual_range -> m_dims[0];
        numcols = actual_range -> m_dims[1];

        if (numrows > 1 && numcols > 1)
        {
            df = (numrows - 1) * (numcols - 1);
        }
        else if (1 == numrows && numcols > 1)
        {
            df = numcols - 1;
        }
        else if (numrows > 1 && 1 == numcols)
        {
            df = numrows - 1;
        }
        else
        {
            assert (false);
        }

        
        value = Chisq<T>::RT( sum, T(df) );

exitlabel:
        *p_value = value;
        return ret;
    }
}
