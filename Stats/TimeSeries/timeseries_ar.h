



#pragma once

#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "TCErrorCommon.h"
#include "TCMath.h"
#include "OlympCore.h"

#include "TCArrayUtils.h"
#include "TCArrayUtilsInternal.h"
#include "Descriptive\average.h"

#include "TimeSeriesUtils.h"
#include "TimeSeriesUtilsLA.h"
#include "TimeSeriesUtilsOptimization.h"

#include <vector>

namespace timeseries_ar_internal
{
    using namespace TimeSeriesUtils;

    template <typename T, TimeSeriesParallelMode mode>
    static void ar_cov_compute(T* y, TCSSize_t yStride, TCSize_t len, TCUInt32 order, T totalMean, T* covFuncData)
    {
        TCSize_t i;
        TCSize_t k;
        for (i = 0; i < order; i++)
        {
            covFuncData[i] = 0.;
            for (k = i; k < len; k++)
            {
                covFuncData[i] += (y[k * yStride] - totalMean) * (y[(k - i) * yStride] - totalMean);
            }
        }
    }

    template <typename T>
    TCErrorCode descriptive_average(const TCArray* numbers, TCSize_t narrays, T* mean)
    {
        static_assert("descriptive_average specialized by wrong type");
    }
    template <>
    TCErrorCode descriptive_average(const TCArray* numbers, TCSize_t narrays, TCFloat* mean)
    {
        return stats_redall_average_f_e(numbers, narrays, mean);
    }
    template <>
    TCErrorCode descriptive_average(const TCArray* numbers, TCSize_t narrays, TCDouble* mean)
    {
        return stats_redall_average_d_e(numbers, narrays, mean);
    }

    template <typename T, TimeSeriesParallelMode mode>
    TCErrorCode stats_timeseries_ar_internal(TCArray* data, TCUInt32 order, TSEstimationMethod method, TSExtrapolateSample presample, TCArray* params, 
        TCArray* tstats, TCArray* residuals, T* sse, T* r2 , TCArray* cov)
    {
        TCTypeTag typeTag = TimeSeriesUtils::argTypeTag<T>();

        
        if (!TimeSeriesUtils::TCArrayIs1D(data)) { return tcerror_code_new(TCError_IllegalInput, 1); }
        
        if (data->m_tag != typeTag) { return tcerror_code_new(TCError_IllegalInput, 1); }
        
        if (TimeSeriesUtils::TCArray1DContainsMetaValues<T>(data)) { return tcerror_code_new(TCError_IllegalInput, 1); }

        
        if (method != MaxLikelihood && method != LeastSquare && method != YuleWalker && method != ForwardBackward) 
        { return tcerror_code_new(TCError_IllegalInput, 3); }

        
        if (presample != extraNone && presample != extraPast && presample != extraFuture && presample != extraPastFuture) 
        { return tcerror_code_new(TCError_IllegalInput, 4); }

        
        if (params->m_tag != typeTag) { return tcerror_code_new(TCError_IllegalInput, 5); }
        if (!TimeSeriesUtils::TCArrayIs1D(params)) { return tcerror_code_new(TCError_IllegalInput, 5); }
        if (params->m_numelt < order + 1) { return tcerror_code_new(TCError_IllegalInput, 5); }
        
        
        if (tstats->m_tag != typeTag) { return tcerror_code_new(TCError_IllegalInput, 6); }
        if (!TimeSeriesUtils::TCArrayIs1D(tstats)) { return tcerror_code_new(TCError_IllegalInput, 6); }
        if (tstats->m_numelt < order + 1) { return tcerror_code_new(TCError_IllegalInput, 6); }

        
        if (residuals->m_tag != typeTag) { return tcerror_code_new(TCError_IllegalInput, 7); }
        if (!TimeSeriesUtils::TCArrayIs1D(residuals)) { return tcerror_code_new(TCError_IllegalInput, 7); }
        if (residuals->m_numelt < data->m_numelt) { return tcerror_code_new(TCError_IllegalInput, 7); }

        
        if (cov->m_tag != typeTag) { return tcerror_code_new(TCError_IllegalInput, 10); }
        if (cov->m_ndims != 2) { return tcerror_code_new(TCError_IllegalInput, 10); }
        if (!(cov->m_dims[0] == order && cov->m_dims[1] == order)) { return tcerror_code_new(TCError_IllegalInput, 10); }

        
        if (method == MaxLikelihood) 
        {
            method = LeastSquare;
        }

        TCSize_t i;
        TCSize_t j;

        
        TCSize_t len = data->m_numelt;
        if (len < order + 1) { return tcerror_code_new(TCError_IllegalSize, 1); }
               
        
        TCSize_t lagStart = ( (presample == extraPast || presample == extraPastFuture) ? 0 : order);
        TCSize_t lagEnd = len;
        if (presample == extraFuture || presample == extraPast) 
        {
            lagEnd += order;
        }
        if (presample == extraPastFuture)
        {
            lagEnd += 2 * order;
        }

        
        TCSize_t meq = (method == YuleWalker ? order : lagEnd - order);

        
        T* tsData = (T*)data->m_data;
        TCSSize_t* tsStrides = data->m_strides;
        TCSSize_t tsStride = tsStrides[0];

        
        T* tsLongData;
        TCSSize_t tsLongStride;
        if (presample == extraNone) 
        { 
            tsLongData = tsData;
            tsLongStride = tsStride;
        }
        else
        {
            TCArray* tsLong;
            if (array_allocate1d<T>(lagEnd, &tsLong) != TCError_NoError) { return TCError_OutOfMemory; }
            TCArrayUniquePtr tsLongPtr(tsLong);
            tsLongData = (T*)tsLong->m_data;
            TCSSize_t* tsLongStrides = tsLong->m_strides;
            tsLongStride = tsLongStrides[0];
            for (i = lagStart; i < order; i++)
            {
                tsLongData[i * tsLongStride] = 0.;
            }
            for (i = order; i < len + order; i++)
            {
                tsLongData[(i - lagStart) * tsLongStride] = tsData[(i - order) * tsStride];
            }
            for (i = len + order - lagStart; i < lagEnd; i++)
            {
                tsLongData[i * tsLongStride] = 0.;
            }
        }

        
        T totalMean = 0.;
        TCErrorCode err_code = descriptive_average<T>(data, 1, &totalMean);
        if (err_code != TCError_NoError) { return TCError_Internal; }

        
        TCSize_t nRows = meq;
        TCSize_t nCols = order;

        TCArray* xMatrix;
        if (array_allocate2d<T>(nRows, nCols, &xMatrix) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr xPtr(xMatrix);
        T* xData = (T*)xMatrix->m_data;
        TCSSize_t* xStrides = xMatrix->m_strides;
        TCSSize_t xLd = xStrides[1];
        T* xDataCol;

        
        TCArray* rhs;
        if (array_allocate1d<T>(nRows, &rhs) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr rhsPtr(rhs);
        T* rhsData = (T*)rhs->m_data;

        
        TCArray* c;
        if (array_allocate1d<T>(nCols, &c) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr cPtr(c);
        T* coefData = (T*)c->m_data;

        
        TCArray* inorm;
        if (array_allocate2d<T>(nCols, nCols, &inorm) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr inormPtr(inorm);

        
        TCArray* covFunc;
        if (array_allocate1d<T>(order + 1, &covFunc) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr covFncPtr(covFunc);
        T* covFuncData = (T*)covFunc->m_data;

        T sseOLS;
        
        if (method == YuleWalker)
        {
            ar_cov_compute<T, mode>(tsLongData, tsLongStride, lagEnd, order + 1, totalMean, covFuncData); 
            if (covFuncData[0] == 0.) { return TCError_Internal; }
            for (i = 1; i < order + 1; i++)
            {
                covFuncData[i] /= covFuncData[0];
            }
            for (j = 0; j < nCols; j++)
            {
                xDataCol = &xData[j * xLd];
                xDataCol[j] = 1.;
                for (i = 1; i < nRows - j; i++)
                {
                    xDataCol[j + i] = covFuncData[i];
                }
                for (i = 1; i < j + 1; i++)
                {
                    xDataCol[j - i] = covFuncData[i];
                }
            }
            for (i = 0; i < nRows; i++)
            {
                rhsData[i] = covFuncData[i + 1];
            }

        }
        else 
        {
            ar_cov_compute<T, mode>(tsLongData, tsLongStride, lagEnd, 1, totalMean, covFuncData); 
            for (j = 0; j < nCols; j++)
            {
                xDataCol = &xData[j * xLd];
                for (i = 0; i < nRows; i++)
                {
                    xDataCol[i] = tsLongData[(order + i - j - 1) * tsLongStride];
                }
            }
            for (i = 0; i < nRows; i++)
            {
                rhsData[i] = tsLongData[(order + i) * tsLongStride];
            }
        }
        TimeSeriesUtilsOptimization::LinearLeastSquares<T, mode>(xMatrix, rhs, c, inorm, &sseOLS);

        
        T* paramsData = (T*)params->m_data;
        TCSSize_t* paramsStrides = params->m_strides;
        TCSSize_t paramsStride = paramsStrides[0];
        for (i = 0; i < order; i++)
        {
            paramsData[i * paramsStride] = coefData[i];
        }
        T sumCoefs = 1.;
        for (i = 0; i < order; i++)
        {
            sumCoefs -= paramsData[i * paramsStride];
        }
        if (sumCoefs == 0) { return TCError_Internal; } 
        paramsData[order * paramsStride] = totalMean * sumCoefs;

        
        sseOLS = 0.;
        T resMean = 0.;
        T* resData = (T*)residuals->m_data;
        TCSSize_t* resStrides = residuals->m_strides;
        TCSSize_t resStride = resStrides[0];
        T tsModel = 0.;
        T diffr = 0.;
        for (i = 0; i < order; i++)
        {
            resData[i * resStride] = (T)0.;
        }
        for (i = order; i < lagEnd; i++)
        {
            tsModel = 0.;
            for (j = 0; j < order; j++)
            {
                tsModel += paramsData[j] * tsLongData[tsLongStride * (i - j - 1)];
            }
            diffr = tsLongData[tsLongStride * i] - tsModel;
            resData[i * resStride] = diffr;
            resMean += diffr;
            sseOLS += diffr * diffr;
        }
        resMean /= (T) (lagEnd - order);
        
        
        *sse = sseOLS;
        *r2 = 1 - sseOLS / covFuncData[0];

        
        T resSigma2 = sseOLS / (T)(lagEnd - order) - resMean * resMean;
        T* covData = (T*)cov->m_data;
        TCSSize_t* covStrides = cov->m_strides;
        TCSSize_t covLd = covStrides[1];

        if (method == YuleWalker)
        {
            for (j = 0; j < order; j++)
            {
                for (i = 0; i < order; i++)
                {
                    covData[j * covLd + i] = xData[j * xLd + i] * resSigma2;
                }
            }
        }
        else
        {
            for (j = 0; j < order; j++)
            {
                for (i = 0; i < order; i++)
                {
                    covData[j * covLd + i] = ((T*)inorm->m_data)[j * nCols + i] * resSigma2;
                }
            }
        }

        
        T* tstatsData = (T*)tstats->m_data;
        tstatsData[0] = 1.;
        TCSSize_t* tstatsStrides = tstats->m_strides;
        TCSSize_t tstatsStride = tstatsStrides[0];

        for (i = 0; i < order; i++)
        {
            tstatsData[i * tstatsStride] = coefData[i] / sqrt(covData[i * covLd + i]);
        }
        tstatsData[order * tstatsStride] = paramsData[order * paramsStride] / sqrt(resSigma2);

        return TCError_NoError;
    }

    
}