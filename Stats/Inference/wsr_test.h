



#pragma once

#include "OlympCoreUtils.h"
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include "binom.h"

using namespace Utils;
using namespace Dist;

namespace Inference
{
    template <typename T>
    class Node 
    {
    public:
        TCSize_t wstat; 
        T prob; 

        Node(TCSize_t c = 0, T ps = 0.) : wstat(c), prob(ps) {}

        bool operator == (const Node &y) 
        { return (wstat == y.wstat); }  
        bool operator < (const Node &y) 
        { return (wstat < y.wstat); }
    };

    template <typename T>
    TCErrorCode wsr_cdf(TCSize_t w_stat, TCSize_t sampleSize, std::vector<TCSize_t> ties, 
        TCSize_t numdifelt, T* pvalue)
    {
        std::vector<std::list<Node<T>>> distList(numdifelt + 1); 
        std::vector<TCSize_t> asumt(numdifelt + 1);
        TCSize_t i;
        TCSSize_t k;
        TCSize_t ncurrent;
        TCSize_t sumallties;
        TCSize_t expWcurrent;
        TCSSize_t wcurrent;
        TCSSize_t wnext;
        TCBool complement;
           
        T sumProb;

        Node<T> elemList;

        std::list<Node<T>>::iterator it_dist_list;
        std::list<Node<T>>::iterator it_curr_list; 

        sumallties = sampleSize;
        sumallties = ties[0];
        asumt[0] = ties[0] + 1;
        for (i = 1; i <= numdifelt; i++)
        {
            sumallties += ties[i];
            asumt[i] = asumt[i - 1] + ties[i - 1] + ties[i]; 
        }

        expWcurrent = sumallties * (sumallties + 1) - ties[0] * (ties[0] + 1);
        elemList.wstat = w_stat;
        elemList.prob = 0.;
        
        distList[numdifelt].push_back(elemList);
            
        ncurrent = sumallties;
        
        for (k = numdifelt - 1; k >= 0; k--)
        {
            ncurrent -= ties[k+1];
            expWcurrent = ncurrent * (ncurrent + 1) - ties[0] * (ties[0] + 1); 
            for (it_dist_list = distList[k + 1].begin(); it_dist_list != distList[k + 1].end(); it_dist_list++)
            {
                wnext = it_dist_list->wstat;
                for (i = 0; i <= ties[k + 1]; i++)
                {
                    wcurrent = wnext - asumt[k + 1] * i; 
                    if (wcurrent > (TCSSize_t)expWcurrent / 2) 
                    {
                        wcurrent = expWcurrent - wcurrent - 1;
                    }
                    elemList.wstat = wcurrent;
                    
                    it_curr_list = find(distList[k].begin(), distList[k].end(), elemList);
                    if (wcurrent >= 0 && it_curr_list == distList[k].end()) 
                    {   
                        distList[k].push_back(elemList);
                    }
                }
            }
        }
        
                     
        for(it_dist_list = distList[0].begin(); it_dist_list != distList[0].end(); it_dist_list++) 
        {
            it_dist_list->prob = 1.0;
        }
        
        ncurrent = 0; 
        for(k = 0; k < (TCSSize_t)numdifelt; k++)
        {
            ncurrent += ties[k]; 
            expWcurrent = ncurrent * (ncurrent + 1) - ties[0] * (ties[0] + 1); 
            for(it_dist_list = distList[k + 1].begin(); it_dist_list != distList[k + 1].end(); it_dist_list++)
            {
                wnext = it_dist_list->wstat; 
                sumProb = 0.;
                for (i = 0; i <= ties[k + 1]; i++)
                {
                    wcurrent = wnext - i * asumt[k + 1]; 
                    if (wcurrent > (TCSSize_t)expWcurrent / 2) 
                    {
                        wcurrent = expWcurrent - wcurrent - 1;
                        complement = true; 
                    }
                    else 
                    {
                        complement = false;
                    }
                    if(wcurrent >= 0)
                    {
                        elemList.wstat = wcurrent;
                        
                        it_curr_list = find(distList[k].begin(), distList[k].end(), elemList);
                        if (!complement)
                        {
                            sumProb += (it_curr_list->prob) * Dist::Binom<T>::PMF(i, ties[k + 1], 0.5); 
                        }
                        else 
                        {   
                            sumProb += (1.0 - (it_curr_list->prob)) * Dist::Binom<T>::PMF(i, ties[k + 1], 0.5); 
                        }
                    }
                    else if(complement) 
                    {
                        sumProb += Dist::Binom<T>::PMF(i, ties[k + 1], 0.5);
                    }
                }
                it_dist_list->prob = sumProb;
            }
        }
        it_dist_list = distList[numdifelt].begin();
        *pvalue = it_dist_list->prob;
        return tcerror_code_new(TCError_NoError, TC_UNSPECIFIED_ARGID);
    }; 
} 
