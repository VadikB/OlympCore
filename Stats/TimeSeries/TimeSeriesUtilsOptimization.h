//<copyright>
// Copyright (c) Microsoft Corporation. All rights reserved.
//</copyright>

#pragma once

#include "TimeSeriesUtils.h"
#include "TimeSeriesUtilsLA.h"
#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "TCArrayUtils.h"
#include "TCArrayUtilsInternal.h"

namespace TimeSeriesUtilsOptimization
{
    using namespace TimeSeriesUtils;

    // Linear Least Squares Fit
    //
    // Params:
    //      x: Input. Design matrix. 2d array of size mxn.
    //      y: Input-output. RHS vector. Contains residual vector on output. 1d array of size m.
    //      c: Output. Coefficients vector. 1d array of size n.
    //      inorm: Output. Inverse normal equations matrix (inverse(trans(x)*x)). 2d array of size nxn.
    //      rss: Output. Residual sum of squares. Scalar.
    //
    // Given the design matrix "x" and right hand side vector "y"
    // solve the linear least squares problem.
    // QR-decomposition method used.
    // Assumptions: normal equations matrix is not singular. (this imply m >= n)
    // TODO: see for gels* lapack routines
    template <typename T, TimeSeriesParallelMode mode>
    static TCErrorCode LinearLeastSquares(TCArray *x, TCArray *y, TCArray *c, TCArray *inorm, T* sse)
    {
        TCSize_t m = x->m_dims[0];
        TCSize_t n = x->m_dims[1];

        // Do QR decomposition of matrix x
        TCArray *r;
        if (array_allocate2d<T>(n, n, &r) != TCError_NoError) { return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID); }
        TCArrayUniquePtr rPtr(r);
        TCArray *q;
        if (array_allocate2d<T>(m, n, &q) != TCError_NoError) { return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID); }
        TCArrayUniquePtr qPtr(q);
        TCErrorCode errCode = TimeSeriesUtilsLA::QR<T, mode>(x, q, r);
        if (errCode != TCError_NoError) { return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID); }

        // R inverse
        TCArray *rInv;
        if (array_allocate2d<T>(n, n, &rInv) != TCError_NoError) { return TCError_OutOfMemory; }
        TCArrayUniquePtr rInvPtr(rInv);
        TimeSeriesUtilsLA::MatrixFillIdentity<T>(rInv);
        TimeSeriesUtilsLA::BlasTrsm<T, mode>(
            TimeSeriesUtilsLA::leftSide,
            TimeSeriesUtilsLA::upperTriangular,
            TimeSeriesUtilsLA::noopOp,
            TimeSeriesUtilsLA::nonUnitTriangular,
            1., r, rInv);

        // Coefficient vector
        TCArray *c0;
        if (array_allocate1d<T>(n, &c0) != TCError_NoError) { return tcerror_code_new(TCError_OutOfMemory, TC_UNSPECIFIED_ARGID); }
        TCArrayUniquePtr c0Ptr(c0);
        TimeSeriesUtilsLA::BlasGemv<T, mode>(TimeSeriesUtilsLA::transOp, 1., q, y, 0., c0);
        TimeSeriesUtilsLA::BlasGemv<T, mode>(TimeSeriesUtilsLA::noopOp, 1., rInv, c0, 0., c);

        // Residual vector: put it to y
        TimeSeriesUtilsLA::BlasGemv<T, mode>(TimeSeriesUtilsLA::noopOp, -1., x, c, 1., y);
        *sse = TimeSeriesUtilsLA::Dot<T, mode>(y, y);

        // Inverse normal eq. matrix
        TimeSeriesUtilsLA::BlasGemm<T, mode>(TimeSeriesUtilsLA::noopOp, TimeSeriesUtilsLA::transOp, 1., rInv, rInv, 0., inorm);

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }
}