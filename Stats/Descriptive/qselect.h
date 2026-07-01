//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCCommon.h"
#include "TCMath.h"
#include "OlympCoreUtils.h"

#include <algorithm>

template<typename T, int n = 1, int parts = 2>
struct interpolation_t
{
    // a <= b
    static inline T get(T a, T b)
    {
        T Inf = Utils::infinity<T>();
        if (-Inf == a && Inf == b)
        {
            return TCError::quiet_NaN<T>();
        }
        return (a * (parts - n) + b * n) / parts;
    }
};

template<typename T, typename Tindex = TCSize_t>
struct interpolation_with_pq_t
{
    Tindex p, q;
    interpolation_with_pq_t(Tindex p, Tindex q)
    {
        this->p = p;
        this->q = q;
    }
    // a <= b
    inline T get(T a, T b) const
    {
        T Inf = Utils::infinity<T>();
        if (-Inf == a && Inf == b)
        {
            TCError::quiet_NaN<T>();
        }
        return (a * (q - p) + b * p) / q;
    }
};

template<typename T>
struct interpolation_with_d_t
{
    T d;
    interpolation_with_d_t(T d)
    {
        this->d = d;
    }
    // a <= b
    inline T get(T a, T b) const
    {
        T Inf = Utils::infinity<T>();
        if (Inf == a)
        {
            return Inf;
        }
        if (-Inf == a)
        {
            if (Inf == b && d > 0)
            {
               return TCError::quiet_NaN<T>();
            }
            return -Inf;
        }
        if (Inf == b)
        {
            if (d > 0)
            {
                return b;
            }
            return a;
        }
        return a * (1 - d) + b * d;
    }
};

#define SWAP(a,b) std::swap(a,b)

// Input data doesn't contain NaNs.
// if even == false, then returns a k-th element in sorted array
// if even == true, then returns an interpolation of k & k+1 elements in the sorted array
template <typename T, bool even, typename TIndex, typename interpolation>
T quick_select(T arr[], TIndex n, TIndex k, const interpolation &median = interpolation()) 
{
    TIndex low, high;
    TIndex middle, ll, hh;

    assert(n - 1 >= k);
    assert(even == false || n - 1 > k);

    low = 0; high = n-1;
    for (;;)
    {
        if (!even)
        {
            if (high <= low)
            {
                // One element only
                return arr[k];
            }

            if (high == low + 1)
            {
                // Two elements only
                if (arr[low] > arr[high])
                {
                    SWAP(arr[low], arr[high]);
                }
                return arr[k];
            }
        }
        else
        {
            if (high <= low)
            {
                // One element only
                return median.get(arr[k],arr[k+1]);
            }

            if (high == low + 1)
            {
                // Two elements only
                if (arr[low] > arr[high])
                    SWAP(arr[low], arr[high]);
                return median.get(arr[k],arr[k+1]);
            }
        }

        // Find median of low, middle and high items; swap into position low
        middle = (low + high) / 2;
        if (arr[middle] > arr[high])
        {
            SWAP(arr[middle], arr[high]);
        }
        if (arr[low] > arr[high])
        {
            SWAP(arr[low], arr[high]);
        }
        if (arr[middle] > arr[low])
        {
            SWAP(arr[middle], arr[low]);
        }

        // Swap low item (now in position middle) into position (low+1)
        SWAP(arr[middle], arr[low+1]);

        // Nibble from each end towards middle, swapping items when stuck
        T pivotvalue = arr[low];
        ll = low + 1;
        hh = high;
        for (;;)
        {
            do 
            {
                ll++; 
            } while (pivotvalue > arr[ll]);
            do
            {
                hh--;
            } while (arr[hh]  > pivotvalue);

            if (hh < ll)
            {
                break;
            }

            SWAP(arr[ll], arr[hh]);
        }

        // Swap middle item (in position low) back into correct position
        SWAP(arr[low], arr[hh]);

        // Re-set active partition
        if (!even)
        {
            if (hh <= k)
                low = ll;
            if (hh >= k)
                high = hh - 1;
        }
        else
        {
            if (hh <= k)
                low = ll;
            if (hh >= k + 1)
                high = hh - 1;
        }
    }
}

#undef SWAP
