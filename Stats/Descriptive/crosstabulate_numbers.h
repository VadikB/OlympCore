//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <hash_map>
#include <vector>
#include "TCArrayUtilsInternal.h"

namespace Descriptive
{
    struct CrosstabulateNumbersComparer
    {
        bool operator() (const TCDouble& lhs, const TCDouble& rhs) const
        {
            if (isnan(lhs))  return false;
            if (isnan(rhs))  return true;
            return lhs < rhs;
        }

        bool operator() (const TCFloat& lhs, const TCFloat& rhs) const
        {
            if (isnan(lhs))  return false;
            if (isnan(rhs))  return true;
            return lhs < rhs;
        }
    };

    template<typename T> TCErrorCode CrosstabulateNumbers(const TCArray* var1, const TCArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
    {
        if (var1->m_numelt != var2->m_numelt)
        {
            return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
        }

        if (var1->m_numelt == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersComparer> > elementsRowMap;
        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersComparer> > elementsColMap;
        std::vector<T> elementsRow;
        std::vector<T> elementsCol;

        T* data1 = (T*)(var1->m_data);
        T* data2 = (T*)(var2->m_data);

        for(TCSize_t i = 0; i < var1->m_numelt; i++)
        {
            T v1 = data1[i];
            T v2 = data2[i];

            
            bool good1 = !(TCError::IsMissing<T>(const_cast<T*>(&v1)));
            bool good2 = !(TCError::IsMissing<T>(const_cast<T*>(&v2)));

            if ((good1) && (good2))
            {
                if (elementsRowMap.find(v1) == elementsRowMap.end())
                {
                    elementsRowMap.insert(std::pair<T, TCSize_t>(v1, elementsRowMap.size()));
                    elementsRow.push_back(v1);
                }

                if (elementsColMap.find(v2) == elementsColMap.end())
                {
                    elementsColMap.insert(std::pair<T, TCSize_t>(v2, elementsColMap.size()));
                    elementsCol.push_back(v2);
                }
            }
        }

        TCSize_t dimRow = elementsRowMap.size();
        TCSize_t dimCol = elementsColMap.size();

        TCErrorCode code = TCError_Internal;
        code = array_allocate1d<T>(dimRow, row_labels);
        if (code != TCError_NoError)
        {
            return code;
        }

        code = array_allocate1d<T>(dimCol, column_labels);
        if (code != TCError_NoError)
        {
            return code;
        }

        code = array_allocate2d<TCUInt64>(dimRow, dimCol, table);
        if (code != TCError_NoError)
        {
            return code;
        }

        memset((*table)->m_data, 0, dimRow * dimCol * sizeof(TCUInt64));

        int i = 0;
        std::vector<T>::iterator it1 = elementsRow.begin();
        for( ; it1 != elementsRow.end(); ++it1)
        {
            ((T*)((*row_labels)->m_data))[i] = *it1;
            i++;
        }
        i = 0;
        std::vector<T>::iterator it2 = elementsCol.begin();
        for( ; it2 != elementsCol.end(); ++it2)
        {
            ((T*)((*column_labels)->m_data))[i] = *it2;
            i++;
        }

        TCSSize_t idx = 0;
        for(TCSize_t i = 0; i < var1->m_numelt; i++)
        {
            T v1 = data1[i];
            T v2 = data2[i];

            bool good1 = !(TCError::IsMissing<T>(const_cast<T*>(&v1)));
            bool good2 = !(TCError::IsMissing<T>(const_cast<T*>(&v2)));

            if ((good1) && (good2))
            {
                TCSize_t i1 = elementsRowMap[((T*)(var1->m_data))[i]];
                TCSize_t i2 = elementsColMap[((T*)(var2->m_data))[i]];
                idx = i2 * dimRow + i1;
                ((TCUInt64*)((*table)->m_data))[idx] += 1;
            }
        }

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }
}