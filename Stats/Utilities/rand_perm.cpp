//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "rand_perm.h"

using namespace Utilities;

TCErrorCode stats_rand_perm_i8_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT8)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCInt8>::Generate(n, p);
}

TCErrorCode stats_rand_perm_i16_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT16)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCInt16>::Generate(n, p);
}

TCErrorCode stats_rand_perm_i32_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT32)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCInt32>::Generate(n, p);
}

TCErrorCode stats_rand_perm_i64_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT64)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCInt64>::Generate(n, p);
}

TCErrorCode stats_rand_perm_ui8_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT8)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCUInt8>::Generate(n, p);
}

TCErrorCode stats_rand_perm_ui16_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT16)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCUInt16>::Generate(n, p);
}

TCErrorCode stats_rand_perm_ui32_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT32)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCUInt32>::Generate(n, p);
}

TCErrorCode stats_rand_perm_ui64_e(const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT64)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    return Rand_Perm<TCUInt64>::Generate(n, p);
}

TCErrorCode stats_rand_perm_seed_i8_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT8)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCInt8>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_i16_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT16)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCInt16>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_i32_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT32)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCInt32>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_i64_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_INT64)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCInt64>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_ui8_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT8)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCUInt8>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_ui16_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT16)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCUInt16>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_ui32_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT32)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCUInt32>::Generate(seed, n, p);
    return ret;
}

TCErrorCode stats_rand_perm_seed_ui64_e(const TCUInt64 seed, const TCSize_t n, TCArray *p)
{
    if (p == NULL || p->m_tag != TC_UINT64)
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
    }
    TCErrorCode ret = Rand_Perm<TCUInt64>::Generate(seed, n, p);
    return ret;
}