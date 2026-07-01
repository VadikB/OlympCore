//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"
#include "crosstabulate_numbers.h"

using namespace Descriptive;

TCErrorCode stats_descriptive_crosstabulate_numbers_d_e(const TCArray* var1, const TCArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
{
    try
    {
        return CrosstabulateNumbers<TCDouble>(var1, var2, table, row_labels, column_labels);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}

TCErrorCode stats_descriptive_crosstabulate_numbers_f_e(const TCArray* var1, const TCArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
{
    try
    {
        return CrosstabulateNumbers<TCFloat>(var1, var2, table, row_labels, column_labels);
    }
    catch(std::bad_alloc &ba)
    {
        ba.what();
        return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID);
    }
    catch(...)
    {
        return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
    }
}