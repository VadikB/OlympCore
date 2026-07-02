



#include "stdafx.h"
#include "wsr_test.h"
#include "norm_s.h"
#include <TCFuncWrapper.h>

using namespace Inference;
using namespace Dist;

template <typename T>
class pairtype 
{
public:
    T x;
    T absx;
    pairtype(T c = 0., T d = 0.): x(c), absx(d) {}
    bool operator == (const pairtype &y) 
    { return absx == y.absx; }
    bool operator < (const pairtype &y) 
    { return absx < y.absx; }
} ;

template<typename T>
static void wsr_do_sort_pairs(std::vector<pairtype<T>>* pairs_vec, TCSize_t countelem)
{
    std::sort(pairs_vec->begin(), pairs_vec->begin() + countelem);
}

template <typename T>
static TCErrorCode wstat_comp(T* xvec, TCSize_t x_len, T median, TCSize_t* wstat, TCSize_t* numallelt, TCSize_t* numdifelt, std::vector<TCSize_t>& ties)
{
    TCSize_t i;
    TCSize_t j;
    TCSize_t asumt;
    TCSize_t countelem;
    TCSize_t wstattemp;

    std::vector<pairtype<T>> pairsValAbs(x_len + 1);
    countelem = x_len;
    pairsValAbs[0].x = 0.;
    pairsValAbs[0].absx = 0.;
    j = 1;
    for (i = 0; i < x_len; i++)
    {
        if (TCError::IsMissingImpl<T>::run(&xvec[i]))
        {
            countelem--;
            continue;
        }
        if (Utils::isinf(xvec[i]) || Utils::isnan(xvec[i]))
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }
        pairsValAbs[j].x = xvec[i] - median;    
        pairsValAbs[j].absx = Utils::abs(pairsValAbs[j].x);
        j++;
    }

    if (countelem < 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    
    wsr_do_sort_pairs<T>(&pairsValAbs, countelem + 1);

    std::vector<TCSize_t> countPositive(countelem + 1);

    
    i = 1; 
    j = 1; 
    ties[0] = 0; 
    countPositive[0] = 0;
    
    while ((i < countelem) && (pairsValAbs[i].absx == 0))  
    {
        ties[0]++; 
        i++;
    }
    while (j <= countelem && i <= countelem)  
    {
        ties[j] = 1; 
        countPositive[j] = 0;  
        if (pairsValAbs[i].x > 0) 
        { 
            countPositive[j]++; 
        }
        while ((i < countelem) && (pairsValAbs[i].absx == pairsValAbs[i + 1].absx))
        {
            ties[j]++;
            if (pairsValAbs[i + 1].x > 0) 
            { 
                countPositive[j]++; 
            }
            i++;
        }
        j++;
        i++;  
    }
    if(j < 3) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    *numdifelt  = j - 1;

    if (countelem - ties[0] < 2) 
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    *numallelt = countelem - ties[0];

    ties[0] = 0; 

    
    asumt = 1 + ties[1];
    wstattemp = asumt * countPositive[1];  
    for (i = 2; i <= *numdifelt; i++) 
    {
        asumt += ties[i-1] + ties[i];
        wstattemp += asumt * countPositive[i];
    }
    *wstat = wstattemp;
    return TCError_NoError;
}

template <typename T>
static TCErrorCode wsr_test_prob_long(TCSize_t numallelt, TCSize_t numdifelt, std::vector<TCSize_t>& ties, T wstat, T* pvalue)
{
    T x;
    T eWstat;
    T varWstat;
    TCSize_t k;
    TCSize_t sumties = 0;

    for (k = 1; k <= numdifelt; k++)
    {
        sumties += ties[k] * (ties[k] * ties[k] - 1);
    }

    eWstat = .25 * ( (T)(numallelt * (numallelt + 1) ));
    varWstat = (T)(numallelt * (numallelt + 1) * (2 * numallelt + 1)  ) / 24. - ((T)sumties) / 48 ;
    x = (wstat - eWstat)/Utils::sqrt(varWstat);  

    *pvalue = Dist::Norm_S<T>::CDF(x);

    return TCError_NoError;
}

template <typename T>
TCErrorCode stats_wsr_test(T* xvec, TCSize_t x_len, const T median, TCHypothesis testtype, T* p_value, T* statistic)
{
    const TCSize_t CutOffSize = 50;
  
    T wstat;
    T temp;
    T retval;
    TCSize_t wstatx;
    TCSize_t wstat2symmetric;
    TCSize_t numdifelt;
    TCSize_t numallelt;
    TCErrorCode code_return;
    std::vector<TCSize_t> ties;

    ties = std::vector<TCSize_t>(x_len + 1);

    code_return = wstat_comp<T>(xvec, x_len, median, &wstatx, &numallelt, &numdifelt, ties);
    if (code_return != TCError_NoError)
    {
        return code_return;
    }
    
    wstat = 0.5 * (T)wstatx ;

    if(numallelt > CutOffSize)
    {   
        code_return = wsr_test_prob_long<T>(numallelt, numdifelt, ties, wstat, &temp);
        if (TCError::IsErrorImpl<T>::run(&temp))
        {
            return TCError::GetTCErrorImpl<T>::run(&temp);
        }
        switch(testtype)
        {
        case OneTailGT:  
            {   
                retval = temp;
                break;
            }
        case TwoTail:
            {
                retval = (temp > .5 ? 2.* (1. - temp) : 2. * temp);
                break;
            }
        case OneTailLT:  
            {
                retval = 1. - temp;
                break;
            }
        default:
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
            break;
        }
    }
    else
    {   
        wstat2symmetric = numallelt * (numallelt + 1) - wstatx;
        wstatx = std::min<TCSize_t>(wstatx, wstat2symmetric);
        wstat2symmetric = numallelt * (numallelt + 1) - wstatx;
        switch (testtype)
        {
        case OneTailGT: 
            {
                code_return = wsr_cdf<T>(wstatx, numallelt, ties, numdifelt, &retval);
                break;
            }
        case TwoTail:  
            {
                code_return = wsr_cdf<T>(wstatx, numallelt, ties, numdifelt, &temp);
                if(code_return == TCError_NoError)
                {
                    code_return = wsr_cdf<T>(wstat2symmetric, numallelt, ties, numdifelt, &retval);
                    if(code_return == TCError_NoError)
                    {
                        retval = 2. * std::min<T>(temp, retval);
                    }
                }
                break;
            }
        case OneTailLT:  
            {
                code_return = wsr_cdf<T>(wstatx, numallelt, ties, numdifelt, &temp);
                if(code_return == TCError_NoError)
                {
                    retval = 1. - temp;
                }
                break;
            }
        default:
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(3));
            break;
        }
        if (TCError::IsErrorImpl<T>::run(&retval))
        {
            return TCError::GetTCErrorImpl<T>::run(&retval);
        }
    }
    *statistic = wstat;
    
    *p_value = ( retval > 1. ? 1. : retval);
    return tcerror_code_new(TCError_NoError, TCArgPosition_UnKnown); 
}

template<typename T>
TCErrorCode wsr_check_data(TCArray* sample, T** xvec, TCSize_t &x_len, const T median)
{
    x_len = sample->m_numelt;
    if (x_len == 0)
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }
    if (Utils::isinf(median) || Utils::isnan(median))
    {
        return tcerror_code_new(TCError_IllegalInput, TCArgumentID(2));
    }
    *xvec = (T*)sample->m_data;
    return TCError_NoError;
}

C_BEGINF(stats_inference_wsr_test_f_e, TCArray* sample, TCFloat median, TCHypothesis tails, TCFloat* p_value, TCFloat* test_statistic)
{
    assert(sample->m_tag == TC_FLOAT);

    TCErrorCode code_return;
    TCFloat* xvec; 
    TCSize_t x_len;

    code_return = wsr_check_data<TCFloat>(sample, &xvec, x_len, median);
    if(code_return == TCError_NoError)
    {
        code_return = stats_wsr_test<TCFloat>(xvec, x_len, median, tails, p_value, test_statistic);
    }
    return code_return;
} C_ENDF

C_BEGINF(stats_inference_wsr_test_d_e, TCArray* sample, TCDouble median, TCHypothesis tails, TCDouble* p_value, TCDouble* test_statistic)
{
    assert(sample->m_tag == TC_DOUBLE);
    TCDouble* xvec; 
    TCSize_t x_len;
    TCErrorCode code_return;

    code_return = wsr_check_data<TCDouble>(sample, &xvec, x_len, median);
    if(code_return == TCError_NoError)
    {
        code_return = stats_wsr_test<TCDouble>(xvec, x_len, median, tails, p_value, test_statistic);
    }
    return code_return;
} C_ENDF