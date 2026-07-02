



#include "stdafx.h"
#include "crosstabulate_numbers_dist.h"

using namespace Descriptive;

TCErrorCode stats_descriptive_crosstabulate_numbers_d_e_dist(const TCDistArray* var1, const TCDistArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
{
    try
    {
        return CrosstabulateNumbersDist<TCDouble, TC_DOUBLE>(var1, var2, table, row_labels, column_labels);
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

TCErrorCode stats_descriptive_crosstabulate_numbers_f_e_dist(const TCDistArray* var1, const TCDistArray* var2, TCArray** table, TCArray** row_labels, TCArray** column_labels)
{
    try
    {
        return CrosstabulateNumbersDist<TCFloat, TC_FLOAT>(var1, var2, table, row_labels, column_labels);
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