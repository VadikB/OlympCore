



#pragma once

#include "stdafx.h"
#include "TCRNGUtils.h"
#include "stdlib.h"
#include <vector>

namespace Utilities
{
    template <typename T>
    class Rand_Perm
    {
        static __inline TCErrorCode Permute(TCRNGStatePtr state, const TCSize_t n, T *x)
        {
            
            for (TCSize_t i = 0; i < n; i++)
            {
                x[i] = i + 1;
            }

            std::vector<double> rand_seq(n);
            TCSize_t count = n;
            double *ptr = rand_seq.data();
            
            while(count > 0)
            {
                TCSize_t sz = std::min<TCSize_t>(count, std::numeric_limits<int>::max());
                int res = TC_mkl_vdRngUniform_sequential(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE, state, sz, ptr, 0.0, 1.0);
                if (res != VSL_ERROR_OK)
                {
                    return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
                }
                count -= sz;
                ptr += sz;
            }

            
            ptr = rand_seq.data();
            for (TCSize_t k = 0, i = n - 1, j; i > 0; i--, k++)
            {
                j = (i + 1) * ptr[k];
                T temp = x[j];
                x[j] = x[i];
                x[i] = temp;
            }
            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

    public:
        static TCErrorCode Generate(const TCSize_t n, TCArray *p)
        {
            try
            {
                if (p != NULL && p->m_numelt >= n && n > 0 && n <= (TCSize_t)std::numeric_limits<T>::max())
                {
                    return Permute(RNGStateManager::GetState(), n, (T *)p->m_data);
                }

                if (p == NULL) 
                {
                    return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
                }

                if (n < 1 || n > (TCSize_t)std::numeric_limits<T>::max()) 
                {
                    return tcerror_code_new(TCError_IllegalInput, TCArgumentID(1));
                }

                assert(p->m_numelt < n);
                return tcerror_code_new(TCError_IllegalSize, TC_UNSPECIFIED_ARGID);
            }
            catch(...)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }
        }

        static TCErrorCode Generate(const TCUInt64 seed, const TCSize_t n, TCArray *p)
        {
            try
            {
                if (p != NULL && p->m_numelt >= n && n > 0 && n <= (TCSize_t)std::numeric_limits<T>::max())
                {
                    TCRNGStatePtr state;
                    int brng = RNGStateManager::GetBRNG();
                    int res = TC_mkl_vslNewStream(&state, brng, seed);
                    if (res != VSL_ERROR_OK)
                    {
                        if (res == VSL_ERROR_MEM_FAILURE)
                        {
                            return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
                        }
                        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
                    }
                    return Permute(state, n, (T *)p->m_data);
                }

                if (p == NULL) 
                {
                    return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
                }

                if (n < 1 || n > (TCSize_t)std::numeric_limits<T>::max()) 
                {
                    return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
                }

                assert(p->m_numelt < n);
                return tcerror_code_new(TCError_IllegalSize, TC_UNSPECIFIED_ARGID);
            }
            catch(...)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }
        }
    };
}