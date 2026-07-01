//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "stdafx.h"

#include "TCRNGUtils.h"

#define EXPON_METHOD VSL_RNG_METHOD_EXPONENTIAL_ICDF_ACCURATE

#define RNG_BUFFER_SIZE 64
#define RNG_NEXT(val, state) \
    if ((++_current_rnd % RNG_BUFFER_SIZE) == 0) \
    { \
        _current_rnd = 0; \
        int res = TC_mkl_viRngUniform_sequential(VSL_RNG_METHOD_UNIFORM_STD, state, _countof(_rand), _rand, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); \
        if (res != VSL_ERROR_OK) \
        { \
            return res; \
        } \
    } \
    val = _rand[_current_rnd]

template <typename T>
static int rand_ziggurat_expon(int method, TCRNGStatePtr state, int count, T* ptr, T alfa, T beta)
{
    const T rng_flt = T(1.0/(double)(1LL<<32));
    static unsigned ke[256];
    static T we[256], fe[256];
    static bool initialized = false;
    
    int _rand[RNG_BUFFER_SIZE]; // for RNG_NEXT
    int _current_rnd = -1;

    method; // unreferenced parameter

    if(!initialized)
    {
        const T m2 = 4294967296.0;
        T de = 7.697117470131487, te=de, ve = 3.949659822581572e-3;
        
        // init the tables
        T q = ve/exp(-de);
        ke[0] = (de/q)*m2;
        ke[1] = 0;

        we[0] = q/m2;
        we[255] = de/m2;

        fe[0] = 1.;
        fe[255] = (T)std::exp(-de);

        for(int i = 254; i >= 1; i--)
        {
            de = (T)-std::log(ve/de + std::exp(-de));
            ke[i+1] = (de/te)*m2;
            te = de;
            fe[i] = (T)std::exp(-de);
            we[i] = de/m2;
        }
        initialized = true;
    }
    
    for(int i = 0; i < count; i++)
    {
        T x;
        for(;;)
        {
            unsigned int jz;
            RNG_NEXT(jz, state);
            int iz = jz & 255;
            x = jz * we[iz];
            if (jz < ke[iz])
                break;

            if (iz == 0)
            {
                int temp;
                RNG_NEXT(temp, state);
                x = 7.69711 - std::log((unsigned)temp*rng_flt);
                break;
            }
            else
            {
                int temp;
                RNG_NEXT(temp, state);
                if (fe[iz]+(unsigned)temp*rng_flt*(fe[iz-1]-fe[iz]) < std::exp(-x))
                {
                    break;
                }
            }
        }
        ptr[i] = x * beta + alfa;
    }
    return VSL_ERROR_OK;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_expon_rand(const T lambda, TCArray* values, fcnType fcn)
{
    if (isinf(lambda) || isnan(lambda))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (lambda <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    TCErrorCode result = stats_rand<T, tag, 2>(values, [lambda, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(EXPON_METHOD, state, count, ptr, 0, 1 / lambda);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_expon_rand(const TCUInt64 seed, const T lambda, TCArray* values, fcnType fcn)
{
    if (isinf(lambda) || isnan(lambda))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (lambda <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(seed, values, [lambda, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(EXPON_METHOD, state, count, ptr, 0, 1 / lambda);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_expon_rand_f_e(const TCFloat lambda, TCArray* values)
{
    return stats_expon_rand<TCFloat, TC_FLOAT>(lambda, values, rand_ziggurat_expon<TCFloat>);
}

TCErrorCode stats_expon_rand_d_e(const TCDouble lambda, TCArray* values)
{
    return stats_expon_rand<TCDouble, TC_DOUBLE>(lambda, values, rand_ziggurat_expon<TCDouble>);
}

TCErrorCode stats_expon_rand_seed_f_e(const TCUInt64 seed, const TCFloat lambda, TCArray* values)
{
    return stats_expon_rand<TCFloat, TC_FLOAT>(seed, lambda, values, rand_ziggurat_expon<TCFloat>);
}

TCErrorCode stats_expon_rand_seed_d_e(const TCUInt64 seed, const TCDouble lambda, TCArray* values)
{
    return stats_expon_rand<TCDouble, TC_DOUBLE>(seed, lambda, values, rand_ziggurat_expon<TCDouble>);
}
