//<copyright>
// Copyright (c) Microsoft Corporation. All rights reserved.
//</copyright>

#pragma once

#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "TCErrorCommon.h"
#include "TCMath.h"
#include "OlympCore.h"
#include "TC_mkl.h"
#include "TCArrayUtils.h"
#include "TCArrayUtilsInternal.h"
#include "TCArrayAPI.h"

#include "TimeSeriesUtils.h"
#include "TimeSeriesUtilsLA.h"
#include "TimeSeriesUtilsOptimization.h"

#include <vector>

namespace adf_test_internal
{
    using namespace TimeSeriesUtils;

    enum TestModel
    {
        arModel,
        ardModel,
        tsModel
    };

    class ADFCVTable
    {
    public:
        static const TCSize_t minT = 25;
        static TCDouble InterpolatePvalue(TestModel testModel, TCUInt32 testType, TCDouble s, TCUInt32 m);
    };

    // AR model, Rho statistic
    static const TCDouble arRhoSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble arRhoSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble arRhoTable[6*9] =
    {
        -2.66, -2.26, -1.95, -1.60, 0.92, 1.33, 1.70, 2.16, 2.16,
        -2.62, -2.25, -1.95, -1.61, 0.91, 1.31, 1.66, 2.08, 2.08,
        -2.60, -2.24, -1.95, -1.61, 0.90, 1.29, 1.64, 2.03, 2.03,
        -2.58, -2.23, -1.95, -1.62, 0.89, 1.29, 1.63, 2.01, 2.01,
        -2.58, -2.23, -1.95, -1.62, 0.89, 1.28, 1.62, 2.00, 2.00,
        -2.58, -2.23, -1.95, -1.62, 0.89, 1.28, 1.62, 2.00, 2.00,
    };

    // ARD model, Rho statistic
    static const TCDouble ardRhoSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble ardRhoSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble ardRhoTable[6*9] =
    {
        -3.75, -3.33, -3.00, -2.63, -0.37, 0.00, 0.34, 0.72, 0.72,
        -3.58, -3.22, -2.93, -2.60, -0.40, -0.03, 0.29, 0.66, 0.66,
        -3.51, -3.17, -2.89, -2.58, -0.42, -0.05, 0.26, 0.63, 0.63,
        -3.46, -3.14, -2.88, -2.57, -0.42, -0.06, 0.24, 0.62, 0.62,
        -3.44, -3.13, -2.87, -2.57, -0.43, -0.07, 0.24, 0.61, 0.61,
        -3.43, -3.12, -2.86, -2.57, -0.44, -0.07, 0.23, 0.60, 0.60,
    };

    // TS model, Rho statistic
    static const TCDouble tsRhoSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble tsRhoSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble tsRhoTable[6*9] =
    {
        -4.38, -3.95, -3.60, -3.24, -1.14, -0.80, -0.50, -0.15, -0.15,
        -4.15, -3.80, -3.50, -3.18, -1.19, -0.87, -0.58, -0.24, -0.24,
        -4.04, -3.73, -3.45, -3.15, -1.22, -0.90, -0.62, -0.28, -0.28,
        -3.99, -3.69, -3.43, -3.13, -1.23, -0.92, -0.64, -0.31, -0.31,
        -3.98, -3.68, -3.42, -3.13, -1.24, -0.93, -0.65, -0.32, -0.32,
        -3.96, -3.66, -3.41, -3.12, -1.25, -0.94, -0.66, -0.33, -0.33
    };

    // AR model, DF statistic
    static const TCDouble arDFSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble arDFSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble arDFTable[6*9] =
    {
        -11.9, -9.3,  -7.3, -5.3, 1.01, 1.40, 1.79, 2.28, 2.28,
        -12.9, -9.9,  -7.7, -5.5, 0.97, 1.35, 1.70, 2.16, 2.16,
        -13.3, -10.2, -7.9, -5.6, 0.95, 1.31, 1.65, 2.09, 2.09,
        -13.6, -10.3, -8.0, -5.7, 0.93, 1.28, 1.62, 2.04, 2.04,
        -13.7, -10.4, -8.0, -5.7, 0.93, 1.28, 1.61, 2.04, 2.04,
        -13.8, -10.5, -8.1, -5.7, 0.93, 1.28, 1.60, 2.03, 2.03
    };

    // ARD model, DF statistic
    static const TCDouble ardDFSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble ardDFSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble ardDFTable[6*9] =
    {
        -17.2, -14.6,  -12.5, -10.2, -0.76, 0.01, 0.65, 1.40, 1.40,
        -18.9, -15.7,  -13.3, -10.7, -0.81, -0.07, 0.53, 1.22, 1.22,
        -19.8, -16.3,  -13.7, -11.0, -0.83, -0.10, 0.47, 1.14, 1.14,
        -20.3, -16.6,  -14.0, -11.2, -0.84, -0.12, 0.43, 1.09, 1.09,
        -20.5, -16.8,  -14.0, -11.2, -0.84, -0.13, 0.42, 1.06, 1.06,
        -20.7, -16.9,  -14.1, -11.3, -0.85, -0.13, 0.41, 1.04, 1.04
    };

    // TS model, DF statistic
    static const TCDouble tsDFSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble tsDFSigLevels[9] = { 0.01, 0.025, 0.05, 0.10, 0.90, 0.95, 0.975, 0.99, 0.99 };
    static const TCDouble tsDFTable[6*9] =
    {
        -22.5, -19.9,  -17.9, -15.6, -3.66, -2.51, -1.53, -0.43, -0.43,
        -25.7, -22.4,  -19.8, -16.8, -3.71, -2.60, -1.66, -0.65, -0.65,
        -27.4, -23.6,  -20.7, -17.5, -3.74, -2.62, -1.73, -0.75, -0.75,
        -28.4, -24.4,  -21.3, -18.0, -3.75, -2.64, -1.78, -0.82, -0.82,
        -28.9, -24.8,  -21.5, -18.1, -3.76, -2.65, -1.78, -0.84, -0.84,
        -29.5, -25.1,  -21.8, -18.3, -3.77, -2.66, -1.79, -0.87, -0.87
    };

    // ARD model, F statistic
    static const TCDouble ardFSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble ardFSigLevels[9] = { 0.99, 0.975, 0.95, 0.90, 0.10, 0.05, 0.025, 0.01, 0.01 };
    static const TCDouble ardFTable[6*9] =
    {
        0.29, 0.38, 0.49, 0.65, 4.12, 5.18, 6.30, 7.88, 7.88,
        0.29, 0.39, 0.50, 0.66, 3.94, 4.86, 5.80, 7.06, 7.06,
        0.29, 0.39, 0.50, 0.67, 3.86, 4.71, 5.57, 6.70, 6.70,
        0.30, 0.39, 0.51, 0.67, 3.81, 4.63, 5.45, 6.52, 6.52,
        0.30, 0.39, 0.51, 0.67, 3.79, 4.61, 5.41, 6.47, 6.47,
        0.30, 0.40, 0.51, 0.67, 3.78, 4.59, 5.38, 6.43, 6.43
    };

    // TS model, F statistic
    static const TCDouble tsFSampleSizes[6] = { 25, 50, 100, 250, 500, 500 };
    static const TCDouble tsFSigLevels[9] = { 0.99, 0.975, 0.95, 0.90, 0.10, 0.05, 0.025, 0.01, 0.01 };
    static const TCDouble tsFTable[6*9] =
    {
        0.74, 0.90, 1.08, 1.33, 5.91, 7.24, 8.65, 10.61, 10.61,
        0.76, 0.93, 1.11, 1.37, 5.61, 6.73, 7.81, 9.31,  9.31,
        0.76, 0.94, 1.12, 1.38, 5.47, 6.49, 7.44, 8.73,  8.73,
        0.76, 0.94, 1.13, 1.39, 5.39, 6.34, 7.25, 8.43,  8.43,
        0.76, 0.94, 1.13, 1.39, 5.36, 6.30, 7.20, 8.34,  8.34,
        0.77, 0.94, 1.13, 1.39, 5.34, 6.25, 7.16, 8.27,  8.27
    };

    template <typename T, TimeSeriesParallelMode mode>
    class ADFStats
    {
    public:
        static T RhoStat(TCArray* c, TCArray* inorm, TCSize_t m, T sse, TCSize_t rhoIdx);
        static T DFStat(TCArray* c, TCSize_t m, TCSize_t aIdx, TCSize_t zetaIdxStart, TCSize_t zetaIdxEnd);
        static T FStat(TCArray* c, TCArray* rInv, TCSize_t constIdx, TCSize_t rhoIdx, TCSize_t m, T sse);
    };

    template<typename T, TimeSeriesParallelMode mode>
    T ADFStats<T, mode>::RhoStat(TCArray* c, TCArray* inorm, TCSize_t m, T sse, TCSize_t rhoIdx)
    {
        T* cData = (T*)c->m_data;
        TCSSize_t cStride = c->m_strides[0];
        TCSize_t n = c->m_dims[0];
        T* inormData = (T*)inorm->m_data;
        TCSSize_t inormLd = inorm->m_strides[1];

        T rho = cData[rhoIdx * cStride];
        T seRho = inormData[inormLd * rhoIdx + rhoIdx] * sse / ((T)m - n);

        return (rho - 1) / sqrt(seRho);
    }

    template<typename T, TimeSeriesParallelMode mode>
    T ADFStats<T, mode>::DFStat(TCArray* c, TCSize_t m, TCSize_t rhoIdx, TCSize_t zetaIdxStart, TCSize_t zetaIdxEnd)
    {
        T* cData = (T*)c->m_data;
        TCSSize_t cStride = c->m_strides[0];

        T rho = cData[rhoIdx * cStride];
        T zetaSum = 0.;
        for (TCSize_t i = zetaIdxStart; i < zetaIdxEnd; i++)
        {
            zetaSum += cData[i * cStride];
        }

        return (T)m * ((rho - 1) / (1 - zetaSum));
    }

    template<typename T, TimeSeriesParallelMode mode>
    T ADFStats<T, mode>::FStat(TCArray* c, TCArray* inorm, TCSize_t extraConstrIdx, TCSize_t rhoIdx, TCSize_t m, T sse)
    {
        TCSize_t n = c->m_dims[0];

        TCArray* B;
        if (array_allocate2d<T>(2, n, &B) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr BPtr(B);
        T* BData = (T*)B->m_data;
        TCSize_t BLd = 2;
        memset(BData, 0, 2*n*sizeof(T));
        BData[extraConstrIdx * BLd] = (T)1.;
        BData[rhoIdx * BLd + 1] = (T)1.;

        // Form c0
        TCArray* c0;
        if (array_allocate1d<T>(n, &c0) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr c0Ptr(c0);
        T* c0Data = (T*)c0->m_data;
        memset(c0Data, 0, n*sizeof(T));
        c0Data[rhoIdx] = (T)1.;

        // U = sqrt((B*Rinv)*(B*Rinv)')
        TCArray* U;
        if (array_allocate2d<T>(2, 2, &U) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr UPtr(U);
        TCArray* Tmp;
        if (array_allocate2d<T>(2, n, &Tmp) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr TmpPtr(Tmp);
        TimeSeriesUtilsLA::BlasGemm<T, mode>(TimeSeriesUtilsLA::noopOp, TimeSeriesUtilsLA::noopOp, 1., B, inorm, 0., Tmp);
        TimeSeriesUtilsLA::BlasGemm<T, mode>(TimeSeriesUtilsLA::noopOp, TimeSeriesUtilsLA::transOp, 1., Tmp, B, 0., U);
        TimeSeriesUtilsLA::Cholesky<T, mode>(U, upperTriangle);

        // u = B*(c0-c);
        TCArray* u;
        if (array_allocate1d<T>(2, &u) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr uPtr(u);
        TimeSeriesUtilsLA::Axpy<T, mode>(-1., c, c0);
        TimeSeriesUtilsLA::BlasGemv<T, mode>(TimeSeriesUtilsLA::noopOp, 1., B, c0, 0., u);

        // u = U^(-1)u
        TimeSeriesUtilsLA::BlasTrsv<T, mode>(
                TimeSeriesUtilsLA::upperTriangular,
                TimeSeriesUtilsLA::transOp,
                TimeSeriesUtilsLA::nonUnitTriangular,
                U, u);

        // F = ((u'*u)/2) * (sse / (m - n))
        T f = TimeSeriesUtilsLA::Dot<T, mode>(u, u);
        f *= (((T)m - n)) / (2. * sse);

        return f;
    }

    TCDouble ADFCVTable::InterpolatePvalue(TestModel testModel, TCUInt32 testType, TCDouble s, TCUInt32 m)
    {
        TCSize_t sampleSizeN;
        TCSize_t sigLevelN;
        const TCDouble* critValTable;
        const TCDouble* sampleSizes;
        const TCDouble* sigLevels;

        if (testModel == arModel && testType == 1)
        {
            sampleSizeN = sizeof(arRhoSampleSizes) / sizeof(arRhoSampleSizes[0]);
            sigLevelN = sizeof(arRhoSigLevels) / sizeof(arRhoSigLevels[0]);
            sampleSizes = arRhoSampleSizes;
            sigLevels = arRhoSigLevels;
            critValTable = arRhoTable;
        }
        else if (testModel == ardModel && testType == 1)
        {
            sampleSizeN = sizeof(ardRhoSampleSizes) / sizeof(ardRhoSampleSizes[0]);
            sigLevelN = sizeof(ardRhoSigLevels) / sizeof(ardRhoSigLevels[0]);
            sampleSizes = ardRhoSampleSizes;
            sigLevels = ardRhoSigLevels;
            critValTable = ardRhoTable;
        }
        else if (testModel == tsModel && testType == 1)
        {
            sampleSizeN = sizeof(tsRhoSampleSizes) / sizeof(tsRhoSampleSizes[0]);
            sigLevelN = sizeof(tsRhoSigLevels) / sizeof(tsRhoSigLevels[0]);
            sampleSizes = tsRhoSampleSizes;
            sigLevels = tsRhoSigLevels;
            critValTable = tsRhoTable;
        }
        else if (testModel == arModel && testType == 2)
        {
            sampleSizeN = sizeof(arDFSampleSizes) / sizeof(arDFSampleSizes[0]);
            sigLevelN = sizeof(arDFSigLevels) / sizeof(arDFSigLevels[0]);
            sampleSizes = arDFSampleSizes;
            sigLevels = arDFSigLevels;
            critValTable = arDFTable;
        }
        else if (testModel == ardModel && testType == 2)
        {
            sampleSizeN = sizeof(ardDFSampleSizes) / sizeof(ardDFSampleSizes[0]);
            sigLevelN = sizeof(ardDFSigLevels) / sizeof(ardDFSigLevels[0]);
            sampleSizes = ardDFSampleSizes;
            sigLevels = ardDFSigLevels;
            critValTable = ardDFTable;
        }
        else if (testModel == tsModel && testType == 2)
        {
            sampleSizeN = sizeof(tsDFSampleSizes) / sizeof(tsDFSampleSizes[0]);
            sigLevelN = sizeof(tsDFSigLevels) / sizeof(tsDFSigLevels[0]);
            sampleSizes = tsDFSampleSizes;
            sigLevels = tsDFSigLevels;
            critValTable = tsDFTable;
        }
        else if (testModel == ardModel && testType == 3)
        {
            sampleSizeN = sizeof(ardFSampleSizes) / sizeof(ardFSampleSizes[0]);
            sigLevelN = sizeof(ardFSigLevels) / sizeof(ardFSigLevels[0]);
            sampleSizes = ardFSampleSizes;
            sigLevels = ardFSigLevels;
            critValTable = ardFTable;
        }
        else if (testModel == tsModel && testType == 3)
        {
            sampleSizeN = sizeof(tsFSampleSizes) / sizeof(tsFSampleSizes[0]);
            sigLevelN = sizeof(tsFSigLevels) / sizeof(tsFSigLevels[0]);
            sampleSizes = tsFSampleSizes;
            sigLevels = tsFSigLevels;
            critValTable = tsFTable;
        }
        else
        {
            return -1.;
        }

        // Do linear interpolation for desired sample size
        TCSize_t next;
        std::vector<TCDouble> table(sigLevelN);
        for (next = 0; next < sampleSizeN - 1; next++)
        {
            if (sampleSizes[next] >= m)
                break;
        }
        if (next != 0 && next != (sampleSizeN - 1))
        {
            TCSize_t prev = next - 1;
            TCDouble prevSize = sampleSizes[prev];
            TCDouble nextSize = sampleSizes[next];
            TCDouble l = (nextSize - m)/(nextSize - prevSize);
            TCDouble r = 1. - l;
            for (TCSize_t i = 0; i < sigLevelN; i++)
            {
                table[i] = l*critValTable[sigLevelN*prev + i] + r*critValTable[sigLevelN*next + i];
            }
        }
        else
        {
            for (TCSize_t i = 0; i < sigLevelN; i++)
            {
                table[i] = critValTable[sigLevelN*next + i];
            }
        }

        // Do linear interpolation for desired statistic value
        for (next = 0; next < sigLevelN - 1; next++)
        {
            if (table[next] >= s)
                break;
        }

        // Only one neighbor
        if (next == 0 || next == (sigLevelN - 1))
        {
            return sigLevels[next];
        }

        TCSize_t prev = next - 1;
        TCDouble prevSigLevel = sigLevels[prev];
        TCDouble nextSigLevel = sigLevels[next];
        TCDouble l = (table[next] - s)/(table[next] - table[prev]);
        TCDouble r = 1 - l;

        return l*prevSigLevel + r*nextSigLevel;
    }

    template <typename T, TimeSeriesParallelMode mode>
    static TCErrorCode adf_test_internal(TCArray* ts, TCUInt32 p, TCInt32 testType, T* pvalue, TestModel testModel)
    {
        // Check if we have 1d array
        if (!TimeSeriesUtils::TCArrayIs1D(ts)) { return tcerror_code_new(TCError_IllegalInput, 1); }

        // Check for meta values
        if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>(ts)) { return tcerror_code_new(TCError_IllegalInput, 1); }

        // Check if testType is correct
        if (testType != 1 && testType != 2 && testType != 3) { return tcerror_code_new(TCError_IllegalInput, 3); }

        // Check of testType is correct for current testModel
        if (testModel == arModel && testType == 3) { return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID); }

        // Check if p is positive
        if (p == 0) { return tcerror_code_new(TCError_IllegalInput, 2); }

        // Configure internal parameters
        // Time series length
        TCSize_t l;
        TCErrorCode err = TCArray_Get_Numelt(ts, &l);
        if (err != TCError_NoError) { return err; }

        // Non-stationary predictors num
        TCSSize_t nNonStationary = 1;
        if (testModel == ardModel) { nNonStationary += 1; }
        if (testModel == tsModel) { nNonStationary += 2; }
        // Total predictors num: p - 1 as y_t-1 included as part of nNonStationary
        TCSize_t n = p - 1 + nNonStationary;
        // First valid elements index for lagged series
        TCSize_t lagStart = p;
        // Regression eq. num
        if (lagStart >= l) { return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID); }
        TCSize_t m = l - lagStart;

        // Check if we definetely have singular case.
        // Also, require m > n, as we need cov estimation.
        if (m <= n) { return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID); }

        // Check if m is enough to perform table-based interpolation
        if (m < ADFCVTable::minT) { return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID); }

        // Time series data
        T* tsData = (T*)ts->m_data;
        TCSSize_t* tsStrides = ts->m_strides;
        TCSSize_t tsStride = tsStrides[0];

        // Create OLS design matrix
        TCArray *x;
        if (array_allocate2d<T>(m, n, &x) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr xPtr(x);
        T* xData = (T*)x->m_data;
        TCSSize_t* xStrides = x->m_strides;
        TCSSize_t xLd = xStrides[1];

        // Fill in non-stationary part: const
        TCSize_t xColIdx = 0;
        TCSize_t constIdx = xColIdx;
        T* xDataCol;
        TCSize_t i;
        if (testModel == ardModel || testModel == tsModel)
        {
            xDataCol = &xData[xLd * (xColIdx++)];
            for (TCSize_t i = 0; i < m; i++)
            {
                xDataCol[i] = (T)1.;
            }
        }

        // Fill in non-stationary part: time
        TCSize_t timeIdx = xColIdx;
        if (testModel == tsModel)
        {
            xDataCol = &xData[xLd * (xColIdx++)];
            for (TCSize_t i = 0; i < m; i++)
            {
                xDataCol[i] = (T)(i + 1);
            }
        }

        // Fill in non-stationary part: y
        TCSize_t rhoIdx = xColIdx;
        xDataCol = &xData[xLd * (xColIdx++)];
        for (i = 0; i < m; i++)
        {
            xDataCol[i] = tsData[(lagStart - 1 + i) * tsStride];
        }

        // Fill in stationary part
        TCSize_t zetaIdxStart = nNonStationary;
        TCSize_t j;
        for (j = nNonStationary; j < n; j++)
        {
            xDataCol = &xData[j*xLd];
            for (i = 0; i < m; i++)
            {
                TCSize_t jStationary = j - nNonStationary;
                TCSize_t idx = lagStart - jStationary + i - 1;
                xDataCol[i] = tsData[idx] - tsData[idx - 1];
            }
        }

        // OLS RHS
        TCArray *y;
        if (array_allocate1d<T>(m, &y) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr yPtr(y);
        T* yData = (T*)y->m_data;
        for (i = 0; i < m; i++)
        {
            yData[i] = tsData[(lagStart + i) * tsStride];
        }

        // Do LSE
        TCArray *c;
        if (array_allocate1d<T>(n, &c) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr cPtr(c);
        TCArray *inorm;
        if (array_allocate2d<T>(n, n, &inorm) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr inormPtr(inorm);
        T sse;
        TimeSeriesUtilsOptimization::LinearLeastSquares<T, mode>(x, y, c, inorm, &sse);

        // Test statistic calculation
        T s = 0;
        switch(testType)
        {
        case 1:
            s = ADFStats<T, mode>::RhoStat(c, inorm, m, sse, rhoIdx);
            break;
        case 2:
            s = ADFStats<T, mode>::DFStat(c, m, rhoIdx, zetaIdxStart, zetaIdxStart + p - 1);
            break;
        case 3:
            TCSize_t extraConstrIdx = (testModel == ardModel) ? constIdx : timeIdx;
            s = ADFStats<T, mode>::FStat(c, inorm, extraConstrIdx, rhoIdx, m, sse);
            break;
        }

        *pvalue = ADFCVTable::InterpolatePvalue(testModel, testType, (TCDouble)s, m);
        return TCError_NoError;
    }

    template <typename T, TimeSeriesParallelMode mode>
    TCErrorCode adf_test_ar_internal(TCArray* x, TCUInt32 p, TCUInt32 test_type, T* pvalue)
    {
        return adf_test_internal<T, mode>(x, p, test_type, pvalue, arModel);
    }

    template <typename T, TimeSeriesParallelMode mode>
    TCErrorCode adf_test_ard_internal(TCArray* x, TCUInt32 p, TCUInt32 test_type, T* pvalue)
    {
        return adf_test_internal<T, mode>(x, p, test_type, pvalue, ardModel);
    }

    template <typename T, TimeSeriesParallelMode mode>
    TCErrorCode adf_test_ts_internal(TCArray* x, TCUInt32 p, TCUInt32 test_type, T* pvalue)
    {
        return adf_test_internal<T, mode>(x, p, test_type, pvalue, tsModel);
    }
}