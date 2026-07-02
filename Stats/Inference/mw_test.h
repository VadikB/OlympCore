



#pragma once

#include "OlympCoreUtils.h"
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include "hypgeom.h"
#include "norm_s.h"

using namespace Utils;
using namespace Dist;

namespace Inference
{
    template <typename T>
    class IntTPairs 
    {
    public:
        TCSize_t ind;
        T val;
        IntTPairs(int c = 0, T d = 0): ind(c), val(d) {}
        bool operator == (const IntTPairs& y)
        { return val == y.val; }
        bool operator < (const IntTPairs& y) 
        { return val < y.val; }
    } ;

    template <typename T>
    class AllProb
    {
    public:
        TCSSize_t mSize; 
        TCSSize_t wStatistic; 
        T prob; 

        AllProb(TCSSize_t ms = 0, TCSSize_t ws = 0, T ps = 0.) : mSize(ms), wStatistic(ws), prob(ps) {}

        bool operator == (const AllProb& y) 
        { return (mSize == y.mSize) && (wStatistic == y.wStatistic); }  

        bool operator < (const AllProb& y) 
        { return ( (mSize < y.mSize) || ( (mSize == y.mSize) && (wStatistic < y.wStatistic) ) ); }
    };

    static TCSSize_t umin(TCSize_t num, TCSize_t ssize, std::vector<TCSize_t>& ties, std::vector<TCSize_t>& asumt)      
    {
        TCSSize_t ui;
        TCSSize_t wmin;
        TCSSize_t mk;
        TCSize_t i;

        wmin = -ssize * ssize;
        mk = ssize;
        for (i = 1; i <= num; i++)
        {
            ui = std::min<int>(mk, ties[i]);
            mk -= ui;
            wmin += ui * asumt[i];
        }
        return wmin;
    }

    static TCSSize_t umax(TCSize_t num, TCSize_t ssize, const std::vector<TCSize_t>& ties, std::vector<TCSize_t>& asumt)
    {
        TCSSize_t ui;
        TCSSize_t wmax;
        TCSSize_t mk;
        TCSize_t i;

        wmax = -ssize * ssize;
        mk = ssize;

        for (i = num; i > 0; i--)
        {
            ui = std::min<int>(mk, ties[i]);
            mk -= ui;
            wmax += ui * asumt[i];
        }
        return wmax;
    }

    
    template <typename T>
    static T check_hyp_params(T x, T n, T M, T N, int cumul)
    {
        if ( (x < std::max<T>(0, n + M - N)) || (x > std::min<T>(n, M)) ) 
        {
            return 0;
        }
        else if ( x == 0. && n == 0. )
        {
            return 1;
        }
        if (cumul)
        {
            return Dist::Hypgeom<T>::CDF((T)x, (T)n, (T)M, (T)N);
        }
        return Dist::Hypgeom<T>::PMF((T)x, (T)n, (T)M, (T)N);
    }

    template <typename T>
    static TCErrorCode mw_cdf(TCSize_t wStat, TCSize_t sampleSize, std::vector<TCSize_t> ties, std::vector<TCSize_t> asumt, TCSize_t numdifelt, T* pvalue)
    {
        TCSSize_t x; 
        TCSize_t k; 
        TCSSize_t currStat;  
        TCSSize_t nextStat;  
        TCSize_t rk; 
        TCSSize_t currSize; 
        TCSSize_t nextSize; 
        TCSize_t sumTies;
        TCSize_t sumAllties;
        TCSize_t lmin; 
        TCSize_t lmax;
        T cumulProb;
        T temp;
        TCSize_t numallelt;

        
        std::vector<std::list<AllProb<T>>> dist_list(numdifelt + 1);  
            
        AllProb<T> elem_list;

        std::list<AllProb<T>>::iterator it_dist_list; 
        std::list<AllProb<T>>::iterator it_curr_list;

        numallelt = ties[1];
        for (k = 2; k <= numdifelt; k++)
        {
            numallelt += ties[k]; 
        }

        elem_list.mSize = sampleSize; 
        elem_list.wStatistic = wStat; 
        elem_list.prob = 0.;

        dist_list[numdifelt].push_back(elem_list);   

        sumTies = numallelt;
        for (k = numdifelt - 1; k > 1; k--)
        {
            sumTies -= ties[k + 1];

            for (it_dist_list = dist_list[k + 1].begin(); it_dist_list != dist_list[k + 1].end(); it_dist_list++)
            {
                nextSize = it_dist_list -> mSize;
                nextStat = it_dist_list -> wStatistic;
                lmin = std::max<TCSSize_t>(0, nextSize - sumTies);
                lmax = std::min<TCSSize_t>(nextSize, ties[k + 1]);

                for (rk = lmin; rk <= lmax; rk++)
                {
                    currStat = nextStat - rk * (asumt[k + 1] - 2 * nextSize + rk);
                    currSize = nextSize - rk;
                    elem_list.mSize = currSize; 
                    elem_list.wStatistic = currStat;
                    if ( (currStat >= umin(k, currSize, ties, asumt)) && (currStat <= umax(k, currSize, ties, asumt)) )
                    {
                        it_curr_list = find(dist_list[k].begin(), dist_list[k].end(), elem_list);
                        if (it_curr_list == dist_list[k].end()) 
                        { 
                            dist_list[k].push_back(elem_list);   
                        }
                    }
                } 
            }
        }

        if (numdifelt == 2)
        {
            x = (wStat - sampleSize * (ties[1] - sampleSize)) / numallelt;
            temp = check_hyp_params((T)x, (T)sampleSize , (T)ties[2], (T)numallelt, 1);	
            *pvalue = temp; 
            return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
        }

        sumTies = ties[1] + ties[2];
        for (it_dist_list = dist_list[2].begin(); it_dist_list != dist_list[2].end(); it_dist_list++)
        {
            currSize = it_dist_list -> mSize; 
            currStat = it_dist_list -> wStatistic;
            x = (currStat - currSize * (ties[1] - currSize)) / sumTies; 
            temp = check_hyp_params((T)x, (T)currSize, (T)ties[2], (T)sumTies, 1);
            it_dist_list->prob = temp;  
        }

        for (k = 3; k <= numdifelt; k++)
        {
            sumAllties = sumTies + ties[k];
            for (it_dist_list = dist_list[k].begin(); it_dist_list != dist_list[k].end(); it_dist_list++)
            {
                currSize = it_dist_list->mSize; 
                currStat = it_dist_list->wStatistic;
                cumulProb = 0;
                lmin = std::max<int>(0, currSize - sumTies); 
                lmax = std::min<int>(currSize, ties[k]);
                for (rk = lmin; rk <= lmax; rk++)
                {
                    nextStat = currStat - rk * (asumt[k] - 2 * currSize + rk);
                    nextSize = currSize - rk;
                    elem_list.mSize = nextSize;
                    elem_list.wStatistic = nextStat;
                    if ( (umin(k - 1, nextSize, ties, asumt) <= nextStat) && (nextStat <= umax(k - 1, nextSize, ties, asumt)) )
                    {
                        it_curr_list = find(dist_list[k - 1].begin(), dist_list[k - 1].end(), elem_list);
                        temp = check_hyp_params((T)rk, (T)currSize, (T)ties[k], (T)sumAllties, 0);
                        cumulProb += (it_curr_list->prob) * temp; 
                    }
                    else if (umax(k - 1, nextSize, ties, asumt) < nextStat)
                    {
                        temp = check_hyp_params((T)rk, (T)currSize , (T)ties[k], (T)sumAllties, 0);
                        cumulProb += temp; 
                    }
                }
                it_dist_list->prob = cumulProb;
            } 
            sumTies = sumAllties;
        } 
        it_dist_list = dist_list[numdifelt].begin();
        *pvalue = it_dist_list->prob;

        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }; 

    template <typename T>
    static TCErrorCode mw_test_prob_short(T* xvec, TCSize_t x_len, T* yvec, TCSize_t y_len, TCHypothesis testtype, T* u_statistic, T* pvalue)
    {
        IntTPairs<T> curr_pair;
        std::vector<IntTPairs<T>> pairs_vec(x_len + y_len) ;

        TCSize_t i;
        TCSize_t k;
        TCSize_t numallelt;
        TCSize_t numdifelt;

        T ustat;
        T temp;
        TCSize_t ustat2;
        TCErrorCode code_return = TCError_NoError;

        numallelt = x_len + y_len;

        std::vector<TCSize_t> ties(numallelt + 1);
        std::vector<TCSize_t> x_indicator(numallelt + 1);
        std::vector<TCSize_t> asumt(numallelt + 1);

        for (k = 0; k < x_len; k++)
        {
            pairs_vec[k].ind = 1;  
            pairs_vec[k].val = xvec[k];
        }
        for (k = x_len; k < numallelt; k++)
        {
            pairs_vec[k].ind = 0;  
            pairs_vec[k].val = yvec[k - x_len];
        }

        code_return = mw_do_sort_pairs<T>(&pairs_vec);
        if (code_return != TCError_NoError)
        {
            return tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
        } 

        x_indicator[0] = 0;
        ties[0] = 0;
        x_indicator[1] = pairs_vec[0].ind;
        ties[1] = 1;

        k = 1;
        for (i = 1; i < numallelt; i++)
        {
            if (pairs_vec[i-1].val == pairs_vec[i].val)
            {
                ties[k]++;
                x_indicator[k] += pairs_vec[i].ind;
            }
            else
            {
                k++;
                ties[k] = 1;                        
                x_indicator[k] = pairs_vec[i].ind;
            }
        }
        if (k < 2) 
        {
            return tcerror_code_new(TCError_IllegalInput, TC_UNSPECIFIED_ARGID);
        }
        numdifelt = k;

        asumt[1] = ties[1];

        for (i = 2; i <= numdifelt; i++)
        {
            asumt[i] = asumt[i - 1] + ties[i - 1] + ties[i]; 
        }

        ustat2 = -x_len * x_len;

        for (k = 1; k <= numdifelt; k++) 
        { 
            ustat2 += x_indicator[k] * asumt[k]; 
        }

        ustat = ( (T)ustat2) / 2.0; 

        *u_statistic = ustat;

        switch (testtype)
        {
        case 1: 
            {
                code_return = mw_cdf<T>(ustat2, x_len, ties, asumt, numdifelt, pvalue);
                if (code_return != TCError_NoError)
                {
                    code_return = tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
                }
                break;
            }
        case 2:  
            {
                ustat2 = std::min<int>(ustat2, 2 * x_len * y_len - ustat2);
                code_return = mw_cdf<T>(ustat2, x_len, ties, asumt, numdifelt, &temp);
                if(code_return == TCError_NoError)
                {
                    code_return = mw_cdf<T>((2 * x_len * y_len - ustat2 - 1), x_len, ties, asumt, numdifelt, pvalue);
                    if(code_return == TCError_NoError)
                    {
                        *pvalue = temp + 1. - *pvalue;
                    }
                }
                if (code_return != TCError_NoError)
                {
                    code_return = tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
                }
                break;
            }
        case 3:  
            {
                code_return = mw_cdf<T>(ustat2, x_len, ties, asumt, numdifelt, pvalue);
                if(code_return == TCError_NoError)
                {
                    *pvalue = 1. - *pvalue;
                }
                else
                {
                    code_return = tcerror_code_new(TCError_Internal, TC_UNSPECIFIED_ARGID);
                }
                break;
            }
        default:
            code_return = tcerror_code_new(TCError_IllegalInput, 3);
            break;
        }
        return code_return;
    }

    template <typename T>
    static TCErrorCode mw_test_prob_long(T* xvec, TCSize_t x_len, T* yvec, TCSize_t y_len, T* u_statistic, T* pvalue)
    {
        T zstat;
        T val;
        T ux;
        T ustat;
        T ranksum;
        T expU;
        T varU;
        T tiesum;
        T pre_x;
        T pre_y;
        T cont_corr;

        TCSize_t total_len; 

        TCSize_t i = 0; 
        TCSize_t j = 0;
        TCSize_t rank = 1;
        TCSize_t tie_len_x = 0;
        TCSize_t tie_len_y = 0;
        TCSize_t tie_len = 0;

        tiesum = 0.;
        ranksum = 0.;
        ux = 0.;

        pre_x = xvec[0] - 1.;
        pre_y = yvec[0] - 1.;

        while (i < x_len)
        {
            if (xvec[i] == pre_x) 
            {
                tie_len_x++;
                ux += rank; 
                rank++; 
                pre_x = xvec[i]; 
                i++; 
                continue;
            }
            else if (tie_len_x) 
            {
                tie_len_x++;
                tiesum += (T)(tie_len_x * (tie_len_x * tie_len_x - 1));
                tie_len_x = 0;
            }

            if (j == y_len) 
            {
                ux += rank;
                rank++;
                pre_x = xvec[i];
                i++;
                continue;
            }
            while (j < y_len)  
            {
                if (yvec[j] == pre_y) 
                {
                    tie_len_y++;
                    rank++;
                    pre_y = yvec[j]; 
                    j++; 
                    continue;
                }
                else if (tie_len_y) 
                {
                    tie_len_y++;
                    tiesum += (T)(tie_len_y * (tie_len_y * tie_len_y - 1));
                    tie_len_y = 0;
                }

                if (xvec[i] > yvec[j]) 
                {
                    rank++; 
                    pre_y = yvec[j]; 
                    j++; 
                }
                else 
                {
                    if (xvec[i] == yvec[j]) 
                    {
                        tie_len_x++;
                        tie_len_y++;
                        ranksum = (T)(rank + rank + 1 );
                        rank++; 
                        rank++;
                        val = xvec[i];
                        pre_y = val;
                        pre_x = val;
                        j++;    
                        while (j < y_len) 
                        {
                            if(yvec[j] == val)
                            {
                                tie_len_y++;  
                                ranksum += (T)rank;
                                rank++;
                                j++;
                            }
                            else { break; }
                        }
                        i++;
                        while(i < x_len) 
                        {
                            if(xvec[i] == val)
                            {
                                tie_len_x++;
                                ranksum += (T)rank;
                                rank++;
                                i++;
                            }
                            else { break; }
                        }
                        tie_len = tie_len_x + tie_len_y ;
                        ranksum = ranksum / ((T)tie_len);
                        ux += ranksum*( (T)tie_len_x );
                        tiesum += (T)( tie_len * ( tie_len * tie_len - 1) );
                        tie_len_x = 0;
                        tie_len_y = 0;
                    }
                    else
                    {
                        ux += rank;
                        rank++;
                        pre_x = xvec[i]; 
                        i++;
                    }
                    break;
                }
            } 
        }

        while (j < y_len) 
        {
            if (yvec[j] == pre_y)  
            {
                tie_len_y++;
                rank++; 
                j++;
                continue;
            }
            else if (tie_len_y) 
            {
                tie_len_y++;
                tiesum += (T)( tie_len_y * ( tie_len_y * tie_len_y - 1) );
                tie_len_y = 0;
            }
            pre_y = yvec[j]; 
            j++;
        }
        if(tie_len_x) 
        {
            tiesum += (T)( tie_len_x * ( tie_len_x * tie_len_x - 1) );
            tie_len_x = 0;
        }
        if(tie_len_y) 
        {
            tiesum += (T)( tie_len_y * ( tie_len_y * tie_len_y - 1) );
            tie_len_y = 0;
        }
        
        ustat = ux - (T)(x_len *( x_len + 1 )) * 0.5;
        
        total_len = x_len + y_len;
        expU = .5 * ( (T)(x_len * y_len) );
        
        cont_corr = (ustat > expU ? -0.5 : (ustat < expU ? 0.5 : 0));
        
        varU =  expU * ((T)total_len + 1.) / 6.*(1. - tiesum / ((T)(total_len * ( total_len * total_len - 1 ))) );

        zstat = (ustat - expU + cont_corr) / Utils::sqrt(varU);  

        *u_statistic = ustat;
        *pvalue = Dist::Norm_S<T>::CDF(zstat);

        return TCError_NoError;
    }

    template <typename T>
    static TCErrorCode stats_mw_test(T* xvec, TCSize_t x_len, T* yvec, TCSize_t y_len, TCHypothesis testtype, T* p_value, T* u_statistic)
    {
        const int CutOffSize = 20;

        T val;
        T u_stat;
        TCErrorCode code_return;

        if(x_len > CutOffSize)
        {   
            code_return = mw_test_prob_long<T>(xvec, x_len, yvec, y_len, &u_stat, &val);
            if (TCError::IsErrorImpl<T>::run(&val))
            {
                *p_value = 0.;
                *u_statistic = 0.;
                return TCError::GetTCErrorImpl<T>::run(&val);
            }
            switch(testtype)
            {
            case 1:  
                {
                    *p_value = val;
                    break;
                }
            case 2:  
                {
                    *p_value = (val > .5 ? 2.*(1. - val) : 2.*val);
                    break;
                }
            case 3:  
                {
                    *p_value = 1. - val;
                    break;
                }
            default:
                code_return = tcerror_code_new(TCError_IllegalInput, 3);
                *p_value = val;
                break;
            }
            *u_statistic = u_stat;
        }
        else
        {   
            code_return = mw_test_prob_short<T>(xvec, x_len, yvec, y_len, testtype, &u_stat, &val);
            if (TCError::IsErrorImpl<T>::run(&val))
            {
                *p_value = 0.;
                *u_statistic = 0.;
                return TCError::GetTCErrorImpl<T>::run(&val);
            }
            *p_value = val;
            *u_statistic = u_stat;
        }
        *p_value = (*p_value > 1. ? 1. : *p_value);
        return code_return; 
    }

    template<typename T>
    static TCErrorCode mw_do_sort_pairs(std::vector<IntTPairs<T>>* pairs_vec)
    {
        std::sort(pairs_vec->begin(), pairs_vec->end());
        return TCError_NoError;
    }

    template<typename T>
    static TCErrorCode mw_check_input(T* data, TCSize_t& length, TCUInt8 narg)
    {
        if (Utils::isinf(data[0]))
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(narg));
        }
        while (length > 0 && TCError::IsMissingImpl<T>::run(&data[length-1]))
        {
            length--;
        }
        if (length > 0 && (Utils::isinf(data[length-1]) || Utils::isnan(data[length-1])))
        {
            return tcerror_code_new(TCError_IllegalInput, TCArgumentID(narg));
        }
        if (length == 0)
        {
            return tcerror_code_new(TCError_IllegalSize, TCArgumentID(narg));
        }
        return TCError_NoError;
    }
} 