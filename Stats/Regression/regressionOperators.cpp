//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "TCReduce.h"
#include "TCTypes.h"
#include "TCArray.h"
#include <vector>

template<typename T> 
void reshape(TCSize_t numElements, const T *orig, T *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone)
{
    for(TCSize_t i=0;i<numElements;i++)
    {
        const TCSize_t ir = i % rows_orig;
        const TCSize_t ic = i / rows_orig;

        const TCSize_t j = ir * cols_orig + ic;

        const TCSize_t jr = j / cols_newone;
        const TCSize_t jc = j % cols_newone;

        const TCSize_t k = jc * rows_newone + jr;

        newone[i] = orig[k];
    }
}

void SetRowsCols(TCSize_t &rows, TCSize_t &cols, const TCArray *known)
{
    rows = known->m_dims[0];
    if (known->m_ndims == 1)
    {
        cols = 1;
    }
    else
    {
        cols = known->m_dims[1];
    }
}

template
void reshape<TCFloat>(TCSize_t numElements, const TCFloat *orig, TCFloat *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone);
template
void reshape<TCDouble>(TCSize_t numElements, const TCDouble *orig, TCDouble *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone);
template
void reshape<TCFComplex>(TCSize_t numElements, const TCFComplex *orig, TCFComplex *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone);
template
void reshape<TCDComplex>(TCSize_t numElements, const TCDComplex *orig, TCDComplex *newone, TCSize_t rows_orig, TCSize_t cols_orig, TCSize_t rows_newone, TCSize_t cols_newone);
