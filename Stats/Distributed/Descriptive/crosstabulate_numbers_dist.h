//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <OlympCoreDist.h>
#include <vector>
#include <hash_map>
#include <ostream>
#include <iostream>

#include "TCTypes.h"
#include "../Descriptive/crosstabulate_numbers.h"
#include "qselect_utils_dist.h"
#include "qselect_dist.h"


namespace Descriptive
{
    struct CrosstabulateNumbersDistComparer
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

    template<typename T, TCTypeTag typeTag> TCErrorCode CrosstabulateNumbersDist(const TCDistArray* var1, const TCDistArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
    {
        TCErrorCode code = TCError_NoError;
        TCErrorCode globalCode = TCError_NoError;

        int processorCount;
        int myID;
        MPI_Comm communicator = (MPI_Comm)var1->m_Layout.m_Comm;
        MPI_Comm_size(communicator, &processorCount);
        MPI_Comm_rank(communicator, &myID);

        if (var1->m_LocalArray.m_numelt != var2->m_LocalArray.m_numelt)
        {
            code = tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
        }
        MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
        if (globalCode != TCError_NoError)
        {
            return TCError_IllegalInput;
        }

        code = TCError_NoError;
        globalCode = TCError_NoError;
        if (var1->m_LocalArray.m_numelt == 0)
        {
            code = tcerror_code_new(TCError_IllegalSize, 1);
        }
        MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
        if (globalCode != TCError_NoError)
        {
            return TCError_IllegalSize;
        }

        TCUInt64 missing_u64 = ((TCUInt64) 0x7FF8000000000000UL) | ((TCUInt64) 0x1UL);
        TCDouble missing_d = (*(TCDouble*)(&missing_u64));

        TCUInt32 missing_u32 = ((TCUInt32) 0x7FC00000) | ((TCUInt32) 0x1UL);
        TCFloat missing_f = (*(TCFloat*)(&missing_u32));

        TCDistArray* thisRowValues = 0;
        TCDistArray* thisColValues = 0;
        TCArray* allRowValues = 0;
        TCArray* allColValues = 0;

        std::vector<T> elementsRow; // to populate row_labels
        std::vector<T> elementsCol; // to populate column_labels

        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersDistComparer> > thisRowMap; // to find unique elements in linear time
        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersDistComparer> > thisColMap;
        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersDistComparer> > elementsRowMap;
        std::hash_map<T, TCSize_t, std::hash_compare<T, CrosstabulateNumbersDistComparer> > elementsColMap;

        code = TCError_NoError;
        globalCode = TCError_NoError;

        code = TCDistArray_New_1D(typeTag, var1->m_Layout.m_GlobalSize, 1, &thisRowValues); // distribute by data
        code |= TCDistArray_New_1D(typeTag, var2->m_Layout.m_GlobalSize, 1, &thisColValues); // distribute by data
        code |= TCArray_New_1D(typeTag, var1->m_Layout.m_GlobalSize, 1, &allRowValues);
        code |= TCArray_New_1D(typeTag, var2->m_Layout.m_GlobalSize, 1, &allColValues);
        code |= TCDistArray_Scatter(allRowValues, MASTER_RANK, thisRowValues);
        code |= TCDistArray_Scatter(allColValues, MASTER_RANK, thisColValues);
        MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
        if (globalCode != TCError_NoError)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }

        // Filling with missings by default
        for(TCSize_t i = 0; i < thisRowValues->m_LocalArray.m_numelt; i++)
        {
            if (typeTag == TC_DOUBLE)
            {
                ((T*)(thisRowValues->m_LocalArray.m_data))[i] = missing_d;
                ((T*)(thisColValues->m_LocalArray.m_data))[i] = missing_d;
            }
            else if (typeTag == TC_FLOAT)
            {
                ((T*)(thisRowValues->m_LocalArray.m_data))[i] = missing_f;
                ((T*)(thisColValues->m_LocalArray.m_data))[i] = missing_f;
            }
        }

        // Getting unique elements
        TCSize_t idx = 0;
        T* data1 = 0;
        T* data2 = 0;

        if ((var1->m_LocalArray.m_numelt > 0) && (var2->m_LocalArray.m_numelt > 0))
        {
            data1 = (T*)(var1->m_LocalArray.m_data);
            data2 = (T*)(var2->m_LocalArray.m_data);
        }

        for(TCSize_t i = 0; i < var1->m_LocalArray.m_numelt; i++)
        {
            if ((!data1) || (!data2))
            {
                continue;
            }

            T v1 = data1[i];
            T v2 = data2[i];

            bool good1 = !(TCError::IsMissing<T>(const_cast<T*>(&v1)));
            bool good2 = !(TCError::IsMissing<T>(const_cast<T*>(&v2)));

            if ((good1) && (good2))
            {
                if (thisRowMap.find(v1) == thisRowMap.end())
                {
                    idx = thisRowMap.size();
                    ((T*)(thisRowValues->m_LocalArray.m_data))[idx] = v1;
                    thisRowMap.insert(std::pair<T, TCSize_t>(v1, thisRowMap.size()));
                }
                    
                if (thisColMap.find(v2) == thisColMap.end())
                {
                    idx = thisColMap.size();
                    ((T*)(thisColValues->m_LocalArray.m_data))[idx] = v2;
                    thisColMap.insert(std::pair<T, TCSize_t>(v2, thisColMap.size()));
                }
            }
        }

        code = TCError_NoError;
        for(int i = 0; i < processorCount; i++)
        {
            code |= TCDistArray_Gather(thisRowValues, i, allRowValues);
            code |= TCDistArray_Gather(thisColValues, i, allColValues);
        }
        MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
        if (globalCode != TCError_NoError)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }

        for(TCSize_t i = 0; i < allRowValues->m_numelt; i++)
        {
            T v1 = ((T*)(allRowValues->m_data))[i];
            if ((typeTag == TC_DOUBLE) && (TCError::IsMissing<T>(const_cast<T*>(&v1))))
            {
                continue;
            }
            if ((typeTag == TC_FLOAT) && (TCError::IsMissing<T>(const_cast<T*>(&v1))))
            {
                continue;
            }

            if (elementsRowMap.find(v1) == elementsRowMap.end())
            {
                elementsRow.push_back(v1);
                elementsRowMap.insert(std::pair<T, TCSize_t>(v1, elementsRowMap.size()));
            }
        }

        for(TCSize_t i = 0; i < allColValues->m_numelt; i++)
        {
            T v2 = ((T*)(allColValues->m_data))[i];
            if ((typeTag == TC_DOUBLE) && (TCError::IsMissing<T>(const_cast<T*>(&v2))))
            {
                continue;
            }
            if ((typeTag == TC_FLOAT) && (TCError::IsMissing<T>(const_cast<T*>(&v2))))
            {
                continue;
            }

            if (elementsColMap.find(v2) == elementsColMap.end())
            {
                elementsCol.push_back(v2);
                elementsColMap.insert(std::pair<T, TCSize_t>(v2, elementsColMap.size()));
            }
        }

        // Now we have elements maps everywhere

        TCArray* thisTable = 0;
        code = TCArray_New_2D(TC_UINT64, elementsRowMap.size(), elementsColMap.size(), 1, &thisTable);
        MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
        if (globalCode != TCError_NoError)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }

        idx = 0;
        for(TCSize_t i = 0; i < var1->m_LocalArray.m_numelt; i++)
        {
            if ((!data1) || (!data2))
            {
                continue;
            }
            T v1 = data1[i];
            T v2 = data2[i];

            bool good1 = !(TCError::IsMissing<T>(const_cast<T*>(&v1)));
            bool good2 = !(TCError::IsMissing<T>(const_cast<T*>(&v2)));

            if ((good1) && (good2))
            {
                TCSize_t i1 = elementsRowMap[((T*)(var1->m_LocalArray.m_data))[i]];
                TCSize_t i2 = elementsColMap[((T*)(var2->m_LocalArray.m_data))[i]];
                idx = i2 * elementsRowMap.size() + i1;
                ((TCUInt64*)(thisTable->m_data))[idx] += 1;
            }
        }

        // Now we have found distArray
        // We are now going to gather all data to output arrays and then reduce thisTable

        // TODO: uncomment and make GV test passing
        //if (myID == masterRank)
        {
            //array_allocate2d<TCUInt64>(elementsRowMap.size(), elementsColMap.size(), table);
            code = TCArray_New_2D(TC_UINT64, elementsRowMap.size(), elementsColMap.size(), 1, table);
            //MPI_Bcast(&code, 1, MPI_INT, MASTER_RANK, communicator);
            MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
            if (globalCode != TCError_NoError)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }
        }

        MPI_Reduce((TCUInt64*)(thisTable->m_data), (TCUInt64*)((*table)->m_data), (int)thisTable->m_numelt, MPI_LONG_LONG_INT, MPI_SUM, MASTER_RANK, communicator);

        // TODO: uncomment and then make GV test passing
        if (myID == MASTER_RANK)
        {
            code = TCError_NoError;
            //code = array_allocate1d<T>(elementsRowMap.size(), row_labels);
            code = TCArray_New_1D(typeTag, elementsRowMap.size(), 1, row_labels);
            //code = array_allocate1d<T>(elementsColMap.size(), column_labels);
            code |= TCArray_New_1D(typeTag, elementsColMap.size(), 1, column_labels);
            MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
            if (globalCode != TCError_NoError)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }

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
        }
        else
        {
            MPI_Allreduce(&code, &globalCode, 1, MPI_INT, MPI_MAX, communicator);
            if (globalCode != TCError_NoError)
            {
                return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
            }
        }

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);;
    }
}