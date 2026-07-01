//<copyright>
// Copyright (c) Microsoft Corporation. All rights reserved.
//</copyright>

#pragma once

#include "TimeSeriesUtils.h"
#include "TCTypes.h"
#include "TCArray.h"
#include "TCError.h"
#include "TCMath.h"
#include "TC_mkl.h"
#include "TCArrayUtils.h"
#include "TCArrayUtilsInternal.h"

namespace TimeSeriesUtilsLA
{
    using namespace TimeSeriesUtils;

    // QR decomposition. See rq spec.
    template <typename T, TimeSeriesParallelMode mode>
    TCErrorCode QR(TCArray* x, TCArray* q, TCArray* r)
    {
        static_assert("Invalid specialization of QR function template");

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }
    template <>
    TCErrorCode QR<TCFloat, sequential>(TCArray* x, TCArray* q, TCArray* r)
    {
        return math_linalg_qr_f_e(x, q, r);
    }
    template <>
    TCErrorCode QR<TCDouble, sequential>(TCArray* x, TCArray* q, TCArray* r)
    {
        return math_linalg_qr_d_e(x, q, r);
    }

    // Dot routine
    template <typename T, TimeSeriesParallelMode mode>
    T Dot(TCArray* x, TCArray* y)
    {
        static_assert("Invalid specialization of Dot function template");

        return (T)0.;
    }
    template <>
    TCFloat Dot<TCFloat, sequential>(TCArray* x, TCArray* y)
    {
        TCFloat r;

        math_linalg_dot_f_e(x, y, &r);

        return r;
    }
    template <>
    TCDouble Dot<TCDouble, sequential>(TCArray* x, TCArray* y)
    {
        TCDouble r;

        math_linalg_dot_d_e(x, y, &r);

        return r;
    }

    // Axpy routine
    template <typename T, TimeSeriesParallelMode mode>
    void Axpy(T aplha, TCArray* x, TCArray* y)
    {
        static_assert("Invalid specialization of Axpy function template");
    }
    template <>
    void Axpy<TCFloat, sequential>(TCFloat alpha, TCArray* x, TCArray* y)
    {
        math_linalg_axpy_f_e(alpha, x, &y);
    }
    template <>
    void Axpy<TCDouble, sequential>(TCDouble alpha, TCArray* x, TCArray* y)
    {
        math_linalg_axpy_d_e(alpha, x, &y);
    }

    // Cholesky factorization routine
    template <typename T, TimeSeriesParallelMode mode>
    void Cholesky(TCArray* a, TCUpperLower uplo)
    {
        static_assert("Invalid specialization of Cholesky function template");
    }
    template <>
    void Cholesky<TCFloat, sequential>(TCArray* a, TCUpperLower uplo)
    {
        math_linalg_cholesky_f_e(&a, uplo);
    }
    template <>
    void Cholesky<TCDouble, sequential>(TCArray* a, TCUpperLower uplo)
    {
        math_linalg_cholesky_d_e(&a, uplo);
    }

    // Fill array a with Identity matrix
    template<typename T>
    void MatrixFillIdentity(TCArray *a)
    {
        TCSize_t m = a->m_dims[0];
        TCSize_t n = a->m_dims[1];
        TCSSize_t lda = a->m_strides[1];
        T* aData = (T*)a->m_data;

        for (TCSize_t j = 0; j < n; j++)
        {
            for (TCSize_t i = 0; i < m; i++)
            {
                aData[j*lda + i] = (T)0.;
            }
        }
        for (TCSize_t j = 0; j < n; j++)
        {
            aData[j*lda + j] = (T)1.;
        }
    }

    typedef enum _LABlasSide
    {
        leftSide,
        rightSide
    } LABlasSide;

    typedef enum _LABlasUpLo
    {
        upperTriangular,
        lowerTriangular
    } LABlasUpLo;

    typedef enum _LABlasTrans
    {
        noopOp,
        transOp,
        conjOp
    } LABlasTrans;

    typedef enum _LABlasDiag
    {
        nonUnitTriangular,
        unitTriangular
    } LABlasDiag;

    // Blas trsm routine. See blas docs.
    template <typename T, TimeSeriesParallelMode mode>
    void BlasTrsm(LABlasSide side, LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, T alpha, TCArray* a, TCArray* b)
    {
        static_assert("Invalid specialization of BlasTrsm function template");
    }
    template <>
    void BlasTrsm<TCFloat, sequential>(LABlasSide side, LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, TCFloat alpha, TCArray* a, TCArray* b)
    {
        TCSSize_t m = a->m_dims[0];
        TCSSize_t n = b->m_dims[1];

        TCFloat* aData = (TCFloat*)a->m_data;
        TCSSize_t lda = a->m_dims[1];

        TCFloat* bData = (TCFloat*)b->m_data;
        TCSSize_t ldb = b->m_dims[1];

        char blasSide = 'R';
        if (side == leftSide) { blasSide = 'L'; }

        char blasUplo = 'U';
        if (uplo == lowerTriangular) { blasUplo = 'L'; }

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        char blasDiag = 'N';
        if (diag == unitTriangular) { blasDiag = 'U'; }

        TC_mkl_strsm_sequential(&blasSide, &blasUplo, &blasTrans, &blasDiag, &m, &n, &alpha, aData, &lda, bData, &ldb);
    }
    template <>
    void BlasTrsm<TCDouble, sequential>(LABlasSide side, LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, TCDouble alpha, TCArray* a, TCArray* b)
    {
        TCSSize_t m = a->m_dims[0];
        TCSSize_t n = b->m_dims[1];

        TCDouble* aData = (TCDouble*)a->m_data;
        TCSSize_t lda = a->m_dims[1];

        TCDouble* bData = (TCDouble*)b->m_data;
        TCSSize_t ldb = b->m_dims[1];

        char blasSide = 'R';
        if (side == leftSide) { blasSide = 'L'; }

        char blasUplo = 'U';
        if (uplo == lowerTriangular) { blasUplo = 'L'; }

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        char blasDiag = 'N';
        if (diag == unitTriangular) { blasDiag = 'U'; }

        TC_mkl_dtrsm_sequential(&blasSide, &blasUplo, &blasTrans, &blasDiag, &m, &n, &alpha, aData, &lda, bData, &ldb);
    }

    // Blas trsv routine. See blas docs.
    template <typename T, TimeSeriesParallelMode mode>
    void BlasTrsv(LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, TCArray* a, TCArray* b)
    {
        static_assert("Invalid specialization of BlasTrsv function template");
    }
    template <>
    void BlasTrsv<TCFloat, sequential>(LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, TCArray* a, TCArray* b)
    {
        TCSSize_t n = a->m_dims[1];
        TCFloat* aData = (TCFloat*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCFloat* bData = (TCFloat*)b->m_data;
        TCSSize_t bStride = b->m_strides[0];

        char blasUplo = 'U';
        if (uplo == lowerTriangular) { blasUplo = 'L'; }

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        char blasDiag = 'N';
        if (diag == unitTriangular) { blasDiag = 'U'; }

        TC_mkl_strsv_sequential(&blasUplo, &blasTrans, &blasDiag, &n, aData, &lda, bData, &bStride);
    }
    template <>
    void BlasTrsv<TCDouble, sequential>(LABlasUpLo uplo, LABlasTrans trans, LABlasDiag diag, TCArray* a, TCArray* b)
    {
        TCSSize_t n = a->m_dims[1];
        TCDouble* aData = (TCDouble*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCDouble* bData = (TCDouble*)b->m_data;
        TCSSize_t bStride = b->m_strides[0];

        char blasUplo = 'U';
        if (uplo == lowerTriangular) { blasUplo = 'L'; }

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        char blasDiag = 'N';
        if (diag == unitTriangular) { blasDiag = 'U'; }

        TC_mkl_dtrsv_sequential(&blasUplo, &blasTrans, &blasDiag, &n, aData, &lda, bData, &bStride);
    }

    // Blas gemv routine. See blas docs.
    template <typename T, TimeSeriesParallelMode mode>
    void BlasGemv(LABlasTrans trans,
        T alpha, TCArray* a,
        TCArray* x,
        T beta, TCArray* y)
    {
        static_assert("Invalid specialization of BlasGemv function template");
    }
    template <>
    void BlasGemv<TCFloat, sequential>(LABlasTrans trans,
        TCFloat alpha, TCArray* a,
        TCArray* x,
        TCFloat beta, TCArray* y)
    {
        TCSSize_t m = a->m_dims[0];
        TCSSize_t n = a->m_dims[1];
        TCFloat* aData = (TCFloat*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCFloat* xData = (TCFloat*)x->m_data;
        TCSSize_t xInc = x->m_strides[0];

        TCFloat* yData = (TCFloat*)y->m_data;
        TCSSize_t yInc = y->m_strides[0];

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        TC_mkl_sgemv_sequential(&blasTrans, &m, &n, &alpha, aData, &lda, xData, &xInc, &beta, yData, &yInc);
    }
    template <>
    void BlasGemv<TCDouble, sequential>(LABlasTrans trans,
        TCDouble alpha, TCArray* a,
        TCArray* x,
        TCDouble beta, TCArray* y)
    {
        TCSSize_t m = a->m_dims[0];
        TCSSize_t n = a->m_dims[1];
        TCDouble* aData = (TCDouble*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCDouble* xData = (TCDouble*)x->m_data;
        TCSSize_t xInc = x->m_strides[0];

        TCDouble* yData = (TCDouble*)y->m_data;
        TCSSize_t yInc = y->m_strides[0];

        char blasTrans = 'N';
        if (trans == transOp) { blasTrans = 'T'; }
        if (trans == conjOp) { blasTrans = 'C'; }

        TC_mkl_dgemv_sequential(&blasTrans, &m, &n, &alpha, aData, &lda, xData, &xInc, &beta, yData, &yInc);
    }

    // Blas gemm routine. See blas docs.
    template <typename T, TimeSeriesParallelMode mode>
    void BlasGemm(LABlasTrans transa, LABlasTrans transb, T alpha, TCArray* a, TCArray* b, T beta, TCArray* y)
    {
        static_assert("Invalid specialization of BlasGemm function template");
    }
    template <>
    void BlasGemm<TCFloat, sequential>(LABlasTrans transa, LABlasTrans transb, TCFloat alpha, TCArray* a, TCArray* b, TCFloat beta, TCArray* c)
    {
        TCSSize_t m = c->m_dims[0];
        TCSSize_t n = c->m_dims[1];
        TCSSize_t k = (transa == noopOp) ? a->m_dims[1] : a->m_dims[1];

        TCFloat* aData = (TCFloat*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCFloat* bData = (TCFloat*)b->m_data;
        TCSSize_t ldb = b->m_strides[1];

        TCFloat* cData = (TCFloat*)c->m_data;
        TCSSize_t ldc = c->m_strides[1];

        char blasTransA = 'N';
        if (transa == transOp) { blasTransA = 'T'; }
        if (transa == conjOp) { blasTransA = 'C'; }

        char blasTransB = 'N';
        if (transb == transOp) { blasTransB = 'T'; }
        if (transb == conjOp) { blasTransB = 'C'; }

        TC_mkl_sgemm_sequential(&blasTransA, &blasTransB, &m, &n, &k, &alpha, aData, &lda, bData, &ldb, &beta, cData, &ldc);
    }
    template <>
    void BlasGemm<TCDouble, sequential>(LABlasTrans transa, LABlasTrans transb, TCDouble alpha, TCArray* a, TCArray* b, TCDouble beta, TCArray* c)
    {
        TCSSize_t m = c->m_dims[0];
        TCSSize_t n = c->m_dims[1];
        TCSSize_t k = (transa == noopOp) ? a->m_dims[1] : a->m_dims[1];

        TCDouble* aData = (TCDouble*)a->m_data;
        TCSSize_t lda = a->m_strides[1];

        TCDouble* bData = (TCDouble*)b->m_data;
        TCSSize_t ldb = b->m_strides[1];

        TCDouble* cData = (TCDouble*)c->m_data;
        TCSSize_t ldc = c->m_strides[1];

        char blasTransA = 'N';
        if (transa == transOp) { blasTransA = 'T'; }
        if (transa == conjOp) { blasTransA = 'C'; }

        char blasTransB = 'N';
        if (transb == transOp) { blasTransB = 'T'; }
        if (transb == conjOp) { blasTransB = 'C'; }

        TC_mkl_dgemm_sequential(&blasTransA, &blasTransB, &m, &n, &k, &alpha, aData, &lda, bData, &ldb, &beta, cData, &ldc);
    }
}