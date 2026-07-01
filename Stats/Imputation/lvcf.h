//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "stdafx.h"
#include <vector>
#include <limits>

namespace Imputation
{
    template <typename T>
    static TCErrorCode stats_imputation_lvcf(TCArray* dataset, TCUInt64* not_replaced)
    {
        *not_replaced = 0;
       
        if (dataset->m_numelt == 0)
        {
            return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown); 
        }
        switch (dataset->m_ndims)
        {
        case 1:
            {
                TCSize_t last = 0;
                T* data = (T*)dataset->m_data;
                if (TCError::IsMissingImpl<T>::run(data))
                {
                    (*not_replaced)++;
                }
                else if (Utils::isnan(*data))
                {
                    (*not_replaced)++;
                }
                for (TCSize_t i = 1; i < dataset->m_numelt; i++)
                {
                    if (TCError::IsMissingImpl<T>::run(&data[i]))
                    {
                        if (!TCError::IsMissingImpl<T>::run(&data[last]) &&
                            !Utils::isnan(data[last]))
                        {
                            data[i] = data[last];
                        }
                        else
                        {
                            (*not_replaced)++;
                        }
                    }
                    else if (Utils::isnan(data[i]))
                    {
                        (*not_replaced)++;
                    }
                    else
                    {
                        last = i;
                    }
                }
            }
            break;
        case 2:
            {
                TCSize_t m = dataset->m_dims[0];
                TCSize_t n = dataset->m_dims[1];
                T NaN = std::numeric_limits<T>::quiet_NaN();
                TCUInt64& nr = *not_replaced;

                std::vector<T> lv(m);
                for (TCSize_t i = 0; i < m; i++)
                {
                    lv[i] = NaN;
                }

                T* data = (T*)dataset->m_data;
                for (TCSize_t i = 0; i < m; i++)
                {
                    if (TCError::IsMissingImpl<T>::run(&data[i]))
                    {
                        nr++;
                    }
                    else if (Utils::isnan(data[i]))
                    {
                        nr++;
                    }
                    else
                    {
                        lv[i] = data[i];
                    }
                }

                for (TCSize_t j = 1; j < n; j++)
                {
                    data = &((T*)dataset->m_data)[j * m];
                    for (TCSize_t i = 0; i < m; i++)
                    {
                        if (TCError::IsMissingImpl<T>::run(&data[i]))
                        {
                            if (!Utils::isnan(lv[i]))
                            {
                                    data[i] = lv[i];
                            }
                            else
                            {
                                nr++;
                            }
                        }
                        else if (Utils::isnan(data[i]))
                        {
                            nr++;
                        }
                        else
                        {
                            lv[i] = data[i];
                        }
                    }
                }
            }
            break;
        default:
            return tcerror_code_new(TCError_IllegalInput, 1);
            break;
        }

        return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown);
    }
}
