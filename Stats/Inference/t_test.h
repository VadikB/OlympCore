



#include "Descriptive\devsq.h"
#include "t.h"
#include <math.h>


namespace Inference 
{   
    template <typename T>
    struct SampleInfo
    {
    public:
        SampleInfo() : _mean(static_cast<T>(0)), _std(static_cast<T>(0)), _count(0) {}
        SampleInfo(T mean, T std, TCSize_t count) : _mean(mean), _std(std), _count(count) {}
        SampleInfo(const SampleInfo & sampinf) : _mean(sampinf._mean), _std(sampinf._std), _count(sampinf._count) {}
        T _mean;
        T _std;
        TCSize_t _count;
    };

    template <typename Workset>
    struct sampleMeanFinal
    {
        static void Final(Workset & ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param0Element(ind) /= ws.Count(ind);
                (ws.Param1Element(ind))._mean = ws.Param0Element(ind);
                (ws.Param1Element(ind))._count = ws.Count(ind);
            }
        }
    };

    template <typename Workset>
    struct sampleInfoFinal
    {
        static void Final(Workset & ws)
        {
            for (TCSize_t ind = 0; ind < ws.NumBlocks(); ++ind)
            {
                ws.Param1Element(ind) /= (ws.Count(ind) - 1);
                (ws.Param2Element(ind))._std = Utils::sqrt(ws.Param1Element(ind));
                (ws.Param2Element(ind))._mean = ws.Param0Element(ind);
                (ws.Param2Element(ind))._count = ws.Count(ind);
            }
        }
    };

    template <typename T>
    TCErrorCode ComputeSampleMean(const TCArray *sample, TCSize_t narrays, SampleInfo<T> &info)
    {
        typedef ReductionOperator<Workset1<T, SampleInfo<T> >, Descriptive::AveReduce, sampleMeanFinal, DoCount, Descriptive::AveCombine, OneInputReduce> AveOp;
        typedef OnePassReduction<AveOp> TTestMeanOps;

        Workset1<T, SampleInfo<T> > ws(SampleInfo<T>(), static_cast<T>(0));
        return reduce<TTestMeanOps>(ws, sample, narrays, &info);
    }

    template <typename T>
    TCErrorCode CollectSampleStats(const TCArray *sample, TCSize_t narrays, SampleInfo<T> &info)
    {
        typedef Descriptive::AverageOperator<Workset2<T, SampleInfo<T> > >::type AveOp;
        typedef ReductionOperator<Workset2<T, SampleInfo<T> >, Descriptive::DevsqReduce, sampleInfoFinal, NoCount, Descriptive::DevsqCombine, OneInputReduce> StdOp;
        typedef TwoPassReduction<AveOp, StdOp> TTestOps;

        Workset2<T, SampleInfo<T> > ws(SampleInfo<T>(), static_cast<T>(0), static_cast<T>(0));
        return reduce<TTestOps>(ws, sample, narrays, &info);
    }

    template <typename T>
    T student_t(const T x, const T deg_freedom)
    {
        const T deg_half = deg_freedom / (T)2.0;
        
        T z;
        if (x == (T)0.0)
        {
            z = (T)0.5;
        }
        else if (x > (T)0.0)
        {
            T tmp = Utils::sqrt(deg_freedom / (x * x) + (T)1.0);
            z = ((T)1.0 + tmp) / ((T)2.0 * tmp); 
        }
        else
        {
            T tmp = Utils::sqrt(x * x + deg_freedom);
            z = deg_half / (x * x + deg_freedom - tmp * x);
        }
            
        return Utils::betainc(z, deg_half, deg_half);
    }

    template <typename T>
    TCErrorCode ComputePairedTTest(const TCArray* sample1, const TCArray* sample2, const TCHypothesis tail, T* p_value)
    {
        if (sample1->m_numelt != sample2->m_numelt)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        TCArray sampleDiff;
        sampleDiff.m_tag = sample1->m_tag;
        sampleDiff.m_ndims = 1;
        sampleDiff.m_strides = sample1->m_strides;
        sampleDiff.m_dims = sample1->m_dims;
        sampleDiff.m_numelt = sample1->m_numelt;
        sampleDiff.m_isref = 0;
        sampleDiff.m_tcHandle = 0;
        
        std::vector<T> vdata(sample1->m_numelt, static_cast<T>(0)); 

        T *data = static_cast<T*>(&vdata[0]);
        T *data1 = static_cast<T*>(sample1->m_data);
        T *data2 = static_cast<T*>(sample2->m_data);

        const T data1_ref = *data1;
        const T data2_ref = *data2;
        TCBool isSame1 = true;
        TCBool isSame2 = true;

        sampleDiff.m_data = data;

        for (TCSize_t i = 0; i < sample1->m_numelt; ++i)
        {
            if (TCError::IsMissing(&data1[i]))
            {
                if (data1_ref != data1[i])
                {
                    isSame1 = false;
                }
                data[i] = data1[i];
            }
            else if (TCError::IsMissing(&data2[i]))
            {
                if (data2_ref != data2[i])
                {
                    isSame2 = false;
                }
                data[i] = data2[i];
            }
            else
            {
                if (data1_ref != data1[i])
                {
                    isSame1 = false;
                }
                if (data2_ref != data2[i])
                {
                    isSame2 = false;
                }
                data[i] = data2[i] - data1[i];
            }
        }

        if (isSame1)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        if (isSame2)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        SampleInfo<T> info;
        CollectSampleStats(&sampleDiff, 1, info);

        const T t = info._mean * sqrt(static_cast<T>(info._count)) / info._std;
        const T df = info._count - 1;

        switch (tail)
        {
        case OneTailGT:
            {
                *p_value = Dist::StudentT<T>::CDF(t, df);
                 return tcerror_code_new(TCError_NoError, 0);
            }
        case TwoTail:
            {
                *p_value = Dist::StudentT<T>::TwoT( Utils::abs(t), df);
                 return tcerror_code_new(TCError_NoError, 0);
            }
        default:
            {
                return tcerror_code_new(TCError_IllegalInput, 3);
            }
        }
    }

    template <typename T>
    TCErrorCode ComputeTwoSampleTTest(const TCArray* sample1, const TCArray* sample2, const TCHypothesis tail, T* p_value)
    {
        SampleInfo<T> info1;
        CollectSampleStats(sample1, 1, info1);

        if (info1._std == 0.0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        SampleInfo<T> info2;
        CollectSampleStats(sample2, 1, info2);

        if (info2._std == 0.0)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        T var1sq = info1._std; 
        var1sq *= info1._std;
        var1sq /= static_cast<T>(info1._count);

        T var2sq = info2._std;
        var2sq *= info2._std;
        var2sq /= static_cast<T>(info2._count);

        T varcmb = var1sq;
        varcmb += var2sq;

        var1sq *= var1sq;
        var2sq *= var2sq;
        
        const T df = (varcmb * varcmb) / ((var1sq / static_cast<T>(info1._count - 1)) + (var2sq / static_cast<T>(info2._count - 1)));
        const T t = (info1._mean - info2._mean) / sqrt(varcmb);

        switch (tail)
        {
        case OneTailGT:
            {
                *p_value = student_t(-t, df);
                return tcerror_code_new(TCError_NoError, 0);
            }
        case TwoTail:
            {
               
                T x = student_t(-t, df);
                T y = student_t(t, df);
                *p_value = x < y ? x : y;
                *p_value *= 2.0;
                return tcerror_code_new(TCError_NoError, 0);
            }
        default:
            {
                return tcerror_code_new(TCError_IllegalInput, 3);
            }
        }
    }

    template <typename T>
    TCErrorCode ComputePooledTTest(const TCArray* sample1, const TCArray* sample2, const TCHypothesis tail, T* p_value)
    {
        TCArray arrays[2];
        arrays[0] = *sample1;
        arrays[1] = *sample2;

        SampleInfo<T> info;
        ComputeSampleMean(arrays, 2, info);

        SampleInfo<T> info1;
        CollectSampleStats(sample1, 1, info1);

        if (info1._std == 0.0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        T invct1 = static_cast<T>(1) / static_cast<T>(info1._count);

        SampleInfo<T> info2;
        CollectSampleStats(sample2, 1, info2);

        if (info2._std == 0.0)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        T invct2 = static_cast<T>(1) / static_cast<T>(info2._count);
       
        info1._std *= info1._std;
        info2._std *= info2._std;
        T sp = (info1._count - 1) * info1._std;
        sp += (info2._count - 1) * info2._std;
        sp /= info._count - 2;

        const T t = (info1._mean - info2._mean) / (sqrt(sp) * sqrt(invct1 + invct2)); 
        const TCSize_t df = info._count - 2;

        switch (tail)
        {
        case OneTailGT:
            {
                *p_value = Dist::StudentT<T>::RT(t, df);
                 return tcerror_code_new(TCError_NoError, 0);
            }
        case TwoTail:
            {
                *p_value = Dist::StudentT<T>::TwoT(Utils::abs(t), df);
                 return tcerror_code_new(TCError_NoError, 0);
            }
        default:
            {
                return tcerror_code_new(TCError_IllegalInput, 3);
            }
        }
    }

    template <typename T>
    TCErrorCode stats_inference_t_test(const TCArray* sample1, const TCArray* sample2, const TCHypothesis tail, const TCTestType test, T* p_value)
    {
        const TCSize_t ndims1 = sample1->m_ndims;
        const TCSize_t ndims2 = sample2->m_ndims;

        if (ndims1 == 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        if (ndims1 > 2)
        {
            return tcerror_code_new(TCError_IllegalSize, 1);
        }

        if (ndims2 == 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        if (ndims2 > 2)
        {
            return tcerror_code_new(TCError_IllegalSize, 2);
        }

        const TCSize_t dims1_1 = sample1->m_dims[0];
        const TCSize_t dims1_2 = sample2->m_dims[0];

        if (dims1_1 == 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        if (dims1_2 == 0)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        const TCSize_t nelem_1 = sample1->m_numelt;
        const TCSize_t nelem_2 = sample2->m_numelt;

        if (nelem_1 < 2)
        {
            return tcerror_code_new(TCError_IllegalInput, 1);
        }

        if (nelem_2 < 2)
        {
            return tcerror_code_new(TCError_IllegalInput, 2);
        }

        switch (test)
        {
        case Paired:
            {
                return ComputePairedTTest(sample1, sample2, tail, p_value);
            }
        case TwoSample:
            {
                return ComputeTwoSampleTTest(sample1, sample2, tail, p_value);
            }
        case Pooled:
            {
                return ComputePooledTTest(sample1, sample2, tail, p_value);
            }
        default:
            {
                return tcerror_code_new(TCError_IllegalInput, 4);
            }
        };
    }
}
