



#include "stdafx.h"

#include <limits>
#include <vector>

template <typename T>
void nb_initialize_array(T* ptr_data, TCSize_t n, T val)
{
    assert(ptr_data);
    for (TCSize_t i = 0; i < n; ++i)
    {
        ptr_data[i] = val;
    }
}

template <typename T>
void nb_initialize_array(TCArray* arr, T val)
{
    assert(arr);
    const TCSize_t size = arr->m_numelt;

    T* ptr_data = (T*)arr->m_data;
    for (TCSize_t i = 0; i < size; ++i)
    {
        *ptr_data = val;
        ptr_data++;
    }
}

bool ValidateArray1D(const TCArray* A, const TCSize_t len)
{
    assert(A);

    if (A->m_ndims != 1)
    {
        return false;
    }

    if (A->m_dims[0] != len)
    {
        return false;
    }

    return true;
}

bool ValidateArray2D(const TCArray* A, const TCSize_t* nrows, const TCSize_t* ncols)
{
    assert(A);

    if (A->m_ndims != 2)
    {
        return false;
    }

    if (nrows)
    {
        if (A->m_dims[0] != *nrows)
        {
            return false;
        }
    }

    if (ncols)
    {
        if (A->m_dims[1] != *ncols)
        {
            return false;
        }
    }

    return true;
}

template <typename IntType>
TCErrorCode do_nb_training(    
    const TCArray* trainingData, 
    const TCArray* outcomes, 
    TCArray* featureCounts,
    TCArray* classCounts,
    TCBool bInitialize)
{
    if (bInitialize)
    {
        nb_initialize_array<TCUInt64>(featureCounts, 0);
        nb_initialize_array<TCUInt64>(classCounts, 0);
    }

    const TCSize_t nFeatures = trainingData->m_dims[0];
    const TCSize_t nSamples = trainingData->m_dims[1];
    const TCSize_t nClasses = classCounts->m_numelt;

    TCUInt64* featureCounts_data = (TCUInt64*)featureCounts->m_data;
    TCUInt64* classCounts_data = (TCUInt64*)classCounts->m_data;
    IntType* ptr_sample_data = (IntType*)trainingData->m_data;
    IntType* ptr_result_data = (IntType*)outcomes->m_data;
       
    for (TCSize_t i = 0; i < nSamples; ++i)
    {
        TCUInt64 iClass = (TCUInt64)(ptr_result_data[i]);

        assert(iClass < nClasses);
        if (iClass >= nClasses)
        {
            
            ptr_sample_data += nFeatures;
            continue;
        }
        classCounts_data[iClass]++;
        
        TCUInt64* ptr_featureCounts =  featureCounts_data + (iClass * nFeatures);
        for (TCSize_t j = 0; j < nFeatures; j++)
        {
            *ptr_featureCounts += (TCUInt64)*ptr_sample_data;
            ptr_featureCounts++;
            ptr_sample_data++;
        }
    }

    return tcerror_code_new(TCError_NoError, 0);
}

TCErrorCode stats_train_naive_bayes(
    const TCArray* trainingData, 
    const TCArray* outcomes, 
    TCArray* featureCount, 
    TCArray* classCount)
{
    if (trainingData == NULL)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if ((outcomes == NULL) || (trainingData->m_tag != outcomes->m_tag))
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if ((featureCount == NULL) || (featureCount->m_tag != TC_UINT64))
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if ((classCount == NULL) || (classCount->m_tag != TC_UINT64))
    {
        return tcerror_code_new(TCError_IllegalInput, 4);
    }

    const TCSize_t nFeatures = trainingData->m_dims[0];
    const TCSize_t nSamples = trainingData->m_dims[1];
    const TCSize_t nClasses = featureCount->m_dims[1];

    if ((nFeatures < 1) || (nSamples < 1))
    {
        return tcerror_code_new(TCError_IllegalSize, 1);
    }

    if (nClasses < 2)
    {
        return tcerror_code_new(TCError_IllegalSize, 3);
    }

    if (ValidateArray1D(outcomes, nSamples) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }

    if (ValidateArray2D(featureCount, &nFeatures, NULL) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }

    if (ValidateArray1D(classCount, nClasses) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 4);
    }

    TCBool bInitialize = true;

    switch(outcomes->m_tag)
    {
    case TC_UINT8:
        return do_nb_training<TCUInt8>(trainingData,outcomes,featureCount,classCount,bInitialize);
    case TC_UINT16:
        return do_nb_training<TCUInt16>(trainingData,outcomes,featureCount,classCount,bInitialize);
    case TC_UINT32:
        return do_nb_training<TCUInt32>(trainingData,outcomes,featureCount,classCount,bInitialize);
    case TC_UINT64:
        return do_nb_training<TCUInt64>(trainingData,outcomes,featureCount,classCount,bInitialize);
    case TC_BOOL:
        return do_nb_training<TCBool>(trainingData,outcomes,featureCount,classCount,bInitialize);
    default:
        return tcerror_code_new(TCError_IllegalInput, 2);
    }
}

template <typename T>
TCErrorCode nb_predict_multinomial_t(
    const TCArray* featureCounts, 
    const TCArray* classCounts,
    const TCArray* sample,
    TCDouble threshold,
    const TCArray* featuresToIgnore,
    TCArray* prediction,
    TCArray* probabilities)
{
    const TCSize_t nFeatures = featureCounts->m_dims[0];
    const TCSize_t nClasses = featureCounts->m_dims[1];
    const TCSize_t nElements = featureCounts->m_numelt;

    std::vector<TCDouble> vecProbs(nElements);
    TCDouble* fprobs = &vecProbs[0];
    
    TCUInt64* ptr_feature_cnt = (TCUInt64*)featureCounts->m_data;
    TCUInt64* ptr_class_cnt = (TCUInt64*)classCounts->m_data;
    TCDouble* idx_fprobs = fprobs;
    TCUInt64 tot_class_cnt = 0;

    for (TCSize_t i = 0; i < nClasses; ++i)
    {
        TCUInt64 Nc = 0;
        TCUInt64* idx_fc = ptr_feature_cnt + (i * nFeatures);
        tot_class_cnt += ptr_class_cnt[i];

        for (TCSize_t j = 0; j < nFeatures; ++j)
        {
            Nc += *idx_fc;
            idx_fc++;
        }

        idx_fc = ptr_feature_cnt + (i * nFeatures);
        for (TCSize_t j = 0; j < nFeatures; ++j)
        {
            *idx_fprobs = (TCDouble)(*idx_fc + threshold);
            *idx_fprobs /= (TCDouble)(Nc + threshold * (TCDouble)nFeatures);
            idx_fc++;
            idx_fprobs++;
        }
    }

    T* ptr_samples = (T*)sample->m_data;
    TCUInt64* ptr_predictions = (TCUInt64*)prediction->m_data;
    TCDouble* ptr_score = probabilities == NULL ? NULL : (TCDouble*)probabilities->m_data;
    TCBool* ptr_ignore = featuresToIgnore == NULL ? NULL : (TCBool*)featuresToIgnore->m_data;

    const TCDouble dmin = std::numeric_limits<TCDouble>::max();

    for (TCSize_t n = 0; n < sample->m_dims[1]; ++n)
    {
        TCSize_t i_max = 0;
        TCDouble ll_max = -dmin; 

        T* sample_data = ptr_samples + (n * nFeatures);

        for (TCSize_t i = 0; i < nClasses; ++i)
        {
            TCDouble ll_value = log(((TCDouble)ptr_class_cnt[i])/(TCDouble)tot_class_cnt);
            idx_fprobs = fprobs + (i * nFeatures);

            for (TCSize_t j = 0; j < nFeatures; ++j)
            {
                if ((ptr_ignore == NULL) || !ptr_ignore[j])
                {
                    TCDouble f_lprob = (TCDouble)sample_data[j] * log(idx_fprobs[j]);

                    ll_value += f_lprob;
                }
            }

            if(ptr_score != NULL) 
            {
                ptr_score[i] = ll_value;
            }

            if (ll_value > ll_max)
            {
                ll_max = ll_value;
                i_max = i;
            }
        }

        if(ptr_score != NULL)
        {
            ptr_score += nClasses;
        }

        ptr_predictions[n] = (TCUInt64) i_max;
    }

    return tcerror_code_new(TCError_NoError, 0);
}

template <typename T>
TCErrorCode nb_predict_binary_t(
    const TCArray* featureCounts, 
    const TCArray* classCounts,
    const TCArray* samples,
    TCDouble threshold,
    const TCArray* featuresToIgnore,
    TCArray* predictions,
    TCArray* probabilities)
{
    const TCSize_t nFeatures = featureCounts->m_dims[0];
    const TCSize_t nClasses = featureCounts->m_dims[1];
    const TCSize_t nElements = featureCounts->m_numelt;

    TCUInt64* ptr_feature_cnt = (TCUInt64*)featureCounts->m_data;
    TCUInt64* ptr_class_cnt = (TCUInt64*)classCounts->m_data;
    
    std::vector<TCDouble>vecProbs(nElements);
    TCDouble* ptr_cond_prob = &vecProbs[0];

    nb_initialize_array(ptr_cond_prob, nElements, 0.0);
    TCUInt64 tot_class_cnt = 0;

    for (TCSize_t i = 0; i < nClasses; ++i)
    {
        tot_class_cnt += ptr_class_cnt[i];
    }

    for (TCSize_t i = 0; i < nClasses; ++i)
    {
        TCSize_t offset_c = i * nFeatures;
        TCDouble* idx_cp = ptr_cond_prob + offset_c;
        TCUInt64* idx_fc = ptr_feature_cnt + offset_c;
        
        for (TCSize_t j = 0; j < nFeatures; ++j)
        {
            TCDouble value = (TCDouble)(idx_fc[j]) + threshold * 0.5;
            value /= (TCDouble)(ptr_class_cnt[i]) + threshold;
            idx_cp[j] = value;
        }
    }
   
    T* ptr_samples = (T*)samples->m_data;
    TCUInt64* ptr_predictions = (TCUInt64*)predictions->m_data;
    TCDouble* ptr_score = probabilities == NULL ? NULL : (TCDouble*)probabilities->m_data;
    TCBool* ptr_ignore = featuresToIgnore == NULL ? NULL : (TCBool*)featuresToIgnore->m_data;

    const TCDouble dmin = std::numeric_limits<TCDouble>::min();

    for (TCSize_t n = 0; n < samples->m_dims[1]; ++n)
    {
        TCSize_t i_max = 0;    
        TCDouble p_max = dmin; 
        TCDouble p_total = 0.0;

        T* sample_data = ptr_samples + (n * nFeatures);
        TCDouble* score_data = ptr_score + (n * nClasses);

        for (TCSize_t i = 0; i < nClasses; ++i)
        {
            TCSize_t offset_c = i * nFeatures;
            TCDouble* idx_cp = ptr_cond_prob + offset_c;

            TCDouble p_value = (TCDouble)ptr_class_cnt[i];
            p_value /= (TCDouble)tot_class_cnt;

            for (TCSize_t j = 0; j < nFeatures; ++j)
            {
                if ((ptr_ignore == NULL) || !(ptr_ignore[j]))
                {
                    if (sample_data[j])
                    {
                        p_value *= idx_cp[j];
                    }
                    else
                    {
                        p_value *= (1.0 - idx_cp[j]);
                    }
                }
            }

            if(ptr_score != NULL)
            {
                p_total += p_value;
                score_data[i] = p_value;
            }

            if (p_value > p_max)
            {
                p_max = p_value;
                i_max = i;
            }
        }

        if (ptr_score != NULL)
        {
            for (TCSize_t i = 0; i < nClasses; ++i)
            {
                score_data[i] /= p_total;
            }
        }

        ptr_predictions[n] = (TCUInt64)i_max;
    }

    return tcerror_code_new(TCError_NoError, 0);
}

TCErrorCode nb_predict_multinomial(
    const TCArray* featureCounts, 
    const TCArray* classCounts,
    const TCArray* samples,
    TCDouble threshold,
    const TCArray* featuresToIgnore, 
    TCArray* predictions,
    TCArray* probabilities)
{
    switch(samples->m_tag)
    {
    case TC_UINT8:
        return nb_predict_multinomial_t<TCUInt8>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT16:
        return nb_predict_multinomial_t<TCUInt16>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT32:
        return nb_predict_multinomial_t<TCUInt32>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT64:
        return nb_predict_multinomial_t<TCUInt64>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_BOOL:
        return tcerror_code_new(TCError_IllegalInput, 3);
    default:
        return tcerror_code_new(TCError_IllegalInput, 3);
    }
}

TCErrorCode nb_predict_binary(
    const TCArray* featureCounts, 
    const TCArray* classCounts,
    const TCArray* samples,
    TCDouble threshold,
    const TCArray* featuresToIgnore, 
    TCArray* predictions,
    TCArray* probabilities)
{
    
    switch(samples->m_tag)
    {
    case TC_UINT8:
        return nb_predict_binary_t<TCUInt8>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT16:
        return nb_predict_binary_t<TCUInt16>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT32:
        return nb_predict_binary_t<TCUInt32>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_UINT64:
        return nb_predict_binary_t<TCUInt64>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case TC_BOOL:
        return nb_predict_binary_t<TCBool>(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    default:
        return tcerror_code_new(TCError_IllegalInput, 3);
    }
}

TCErrorCode do_nb_predict(
    const TCArray* featureCounts, 
    const TCArray* classCounts,
    const TCArray* samples,
    TCDouble threshold,
    TCArray* featuresToIgnore,
    TCBool bMultinomial,
    TCArray* predictions,
    TCArray* probabilities
    )
{
    switch(bMultinomial)
    {
    case true:
        return nb_predict_multinomial(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    case false:
        return nb_predict_binary(featureCounts, classCounts, samples, threshold, featuresToIgnore, predictions, probabilities);
    default:
        return tcerror_code_new(TCError_IllegalInput, 6);
    }
}

TCErrorCode stats_predict_naive_bayes(
    TCArray* featureCounts, 
    TCArray* classCounts,
    TCArray* sample,
    TCDouble threshold,
    TCArray* featuresToIgnore,
    TCBool bUseMultinomialModel,
    TCArray* predictions,
    TCArray* probabilities)
{
    if (featureCounts->m_tag != TC_UINT64)
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (classCounts->m_tag != TC_UINT64)
    {
        return tcerror_code_new(TCError_IllegalInput, 2);
    }

    if (featuresToIgnore != NULL)
    {
        if (featuresToIgnore->m_tag != TC_UINT64)
        {
            return tcerror_code_new(TCError_IllegalInput, 5);
        }
    }

    if (predictions->m_tag != TC_UINT64)
    {
        return tcerror_code_new(TCError_IllegalInput, 7);
    }

    if (probabilities != NULL)
    {
        if (probabilities->m_tag != TC_DOUBLE)
        {
            return tcerror_code_new(TCError_IllegalInput, 8);
        }
    }

    const TCSize_t nFeatures = featureCounts->m_dims[0];
    const TCSize_t nClasses = featureCounts->m_dims[1];
    const TCSize_t nSamples = sample->m_dims[1];

    if ((nFeatures < 1) || (nClasses < 2))
    {
        return tcerror_code_new(TCError_IllegalInput, 1);
    }

    if (nSamples < 1)
    {
        return tcerror_code_new(TCError_IllegalInput, 3);
    }

    if (ValidateArray1D(classCounts, nClasses) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 2);
    }

    if (ValidateArray2D(sample, &nFeatures, NULL) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 3);
    }

    if (featuresToIgnore)
    {
        if (ValidateArray1D(featuresToIgnore, nFeatures) == false)
        {
            return tcerror_code_new(TCError_IllegalSize, 5);
        }
    }
    
    if (ValidateArray1D(predictions, nSamples) == false)
    {
        return tcerror_code_new(TCError_IllegalSize, 7);
    }

    if (probabilities)
    {
        if (ValidateArray2D(probabilities, &nClasses, &nSamples) == false)
        {
            return tcerror_code_new(TCError_IllegalSize, 8);
        }
    }

    return do_nb_predict(featureCounts, classCounts, sample, threshold, featuresToIgnore, bUseMultinomialModel, predictions, probabilities);
}
