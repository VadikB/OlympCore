//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "regressionOperators.h"

template<typename T> 
TCErrorCode regression_intercept_inner(TCSize_t numElements, TCBool , T* known_xs, T* known_ys, T* ret)
{
    const T zero = tc_static_cast<T>(0);
    T averX, averY;
    T b;

    CountOp<T> opCount(zero);
    T numEl;
    reduce2arrays(opCount, numElements, known_xs, known_ys, zero, zero, zero, &numEl);

    if((TCDouble)Utils::abs(numEl) < 2.0)
    {
        return TCError_IllegalInput;
    }

    SumOperator<T> op(zero);
    reduce2arrays(op, numElements, known_xs, known_ys, zero, zero, zero, &averX);

    reduce2arrays(op, numElements, known_ys, known_xs, zero, zero, zero, &averY);

    averX /= numEl;
    averY /= numEl;

    T xy,xx;

    SumMultSlopeOperator<T> op2(zero);
    reduce2arrays(op2, numElements, known_xs, known_ys, averX, averY, zero, &xy);

    SumSqSlopeOperator<T> op3(zero);
    reduce2arrays(op3, numElements, known_xs, known_ys, averX, averY, zero, &xx);

    b = xy / xx;
    *ret = averY - b * averX;

    return TCError_NoError;
}

template<typename T> 
TCErrorCode  regression_intercept(TCSize_t numElements, TCBool isref, T* known_xs, T* known_ys, TCSize_t rowsX, TCSize_t colsX, TCSize_t rowsY, TCSize_t colsY, T* ret)
{
    T *tmp = known_ys;
    std::vector<T> tmpMem;

    if (colsX != colsY)
    {
        tmpMem.resize(colsX*rowsX);
        tmp = &(tmpMem.front());
        reshape(numElements, known_ys, tmp, rowsX, colsX, rowsY, colsY);
    }

    return regression_intercept_inner(numElements, isref, known_xs, tmp, ret);
}
