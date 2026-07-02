



#include "stdafx.h"

#include "TCRNGUtils.h"

#define NORM_METHOD VSL_RNG_METHOD_GAUSSIAN_BOXMULLER

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
static int rand_ziggurat_norm(int method, TCRNGStatePtr state, int count, T* ptr, T mean, T sigma)
{
    const T r = 3.442620f;
    const T rng_flt = T(1.0/(double)(1LL<<32));
    static unsigned kn[128];
    static T wn[128], fn[128];
    static bool initialized=false;

    int _rand[RNG_BUFFER_SIZE]; 
    int _current_rnd = -1;

    method; 

    if(!initialized)
    {
        const T m1 = 2147483648.0;
        T dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;
        
        
        T q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;
        
        wn[0] = (T)(q/m1);
        wn[127] = (T)(dn/m1);
        
        fn[0] = 1.f;
        fn[127] = (T)std::exp(-.5*dn*dn);
        
        for(int i = 126; i >= 1; i--)
        {
            dn = std::sqrt(-2.0*std::log(vn/dn+std::exp(-0.5*dn*dn)));
            kn[i+1] = (T)((dn/tn)*m1);
            tn = dn;
            fn[i] = (T)std::exp(-.5*dn*dn);
            wn[i] = (T)(dn/m1);
        }
        initialized = true;
    }
    
    for(int i = 0; i < count; i++)
    {
        T x, y;
        for(;;)
        {
            int hz;
            RNG_NEXT(hz, state);
            int iz = hz & 127;
            x = hz*wn[iz];
            if ((unsigned)std::abs(hz) < kn[iz])
                break;
            if (iz == 0) 
            {
                do
                {
                    int temp;
                    RNG_NEXT(temp, state);
                    x = (unsigned)temp*rng_flt;
                    RNG_NEXT(temp, state);
                    y = (unsigned)temp*rng_flt;
                    x = (T)(-std::log(x+FLT_MIN)*0.2904764); 
                    y = (T)-std::log(y+FLT_MIN);
                } while( y + y < x*x );
                x = hz >= 0 ? r + x : -(r + x);
                break;
            }
            
            int temp;
            RNG_NEXT(temp, state);
            y = (unsigned)temp*rng_flt;
            if(fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x))
                break;
        }
        
        ptr[i] = x * sigma + mean;
    }
    return VSL_ERROR_OK;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_norm_rand(const T mean, const T stand_dev, const TCArray* values, fcnType fcn)
{
    if (isinf(mean) || isnan(mean))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (isinf(stand_dev) || isnan(stand_dev))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (stand_dev <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    TCErrorCode result = stats_rand<T, tag, 3>(values, [mean, stand_dev, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(NORM_METHOD, state, count, ptr, mean, stand_dev);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

template <typename T, TCTypeTag tag, typename fcnType>
TCErrorCode stats_norm_rand(const TCUInt64 seed, const T mean, const T stand_dev, const TCArray* values, fcnType fcn)
{
    if (isinf(mean) || isnan(mean))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (isinf(stand_dev) || isnan(stand_dev))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (stand_dev <= 0)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    TCErrorCode result = stats_rand<T, tag, 4>(seed, values, [mean, stand_dev, fcn](TCRNGStatePtr state, int count, T* ptr) -> TCErrorCode {
        int res = fcn(NORM_METHOD, state, count, ptr, mean, stand_dev);
        if (res != VSL_ERROR_OK)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        }
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    });

    return result;
}

TCErrorCode stats_norm_rand_f_e(const TCFloat mean, const TCFloat stand_dev, TCArray* values)
{
    return stats_norm_rand<TCFloat, TC_FLOAT>(mean, stand_dev, values, rand_ziggurat_norm<TCFloat>);
}

TCErrorCode stats_norm_rand_d_e(const TCDouble mean, const TCDouble stand_dev, TCArray* values)
{
    return stats_norm_rand<TCDouble, TC_DOUBLE>(mean, stand_dev, values, rand_ziggurat_norm<TCDouble>);
}

TCErrorCode stats_norm_rand_seed_f_e(const TCUInt64 seed, const TCFloat mean, const TCFloat stand_dev, TCArray* values)
{
    return stats_norm_rand<TCFloat, TC_FLOAT>(seed, mean, stand_dev, values, rand_ziggurat_norm<TCFloat>);
}

TCErrorCode stats_norm_rand_seed_d_e(const TCUInt64 seed, const TCDouble mean, const TCDouble stand_dev, TCArray* values)
{
    return stats_norm_rand<TCDouble, TC_DOUBLE>(seed, mean, stand_dev, values, rand_ziggurat_norm<TCDouble>);
}
