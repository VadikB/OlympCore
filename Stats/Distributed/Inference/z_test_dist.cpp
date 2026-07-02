



#include "z_test_dist.h"

using namespace Inference;

TCErrorCode stats_inference_z_test_d_dist(const TCDistArray* sample, TCDouble mean, TCDouble* p_value)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_DOUBLE);

    return stats_inference_z_test_dist<TCDouble>(sample, mean, p_value);
}

TCErrorCode stats_inference_z_test_f_dist(const TCDistArray* sample, TCFloat mean, TCFloat* p_value)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_FLOAT);

    return stats_inference_z_test_dist<TCFloat>(sample, mean, p_value);
}

TCErrorCode stats_inference_z_test_std_d_dist(const TCDistArray* sample, TCDouble mean, TCDouble std, TCDouble* p_value)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_DOUBLE);

    return stats_inference_z_test_std_dist<TCDouble>(sample, mean, std, p_value);
}

TCErrorCode stats_inference_z_test_std_f_dist(const TCDistArray* sample, TCFloat mean, TCFloat std, TCFloat* p_value)
{
    assert(TCDistArray_Get_TypeTag(*sample) == TC_FLOAT);

    return stats_inference_z_test_std_dist<TCFloat>(sample, mean, std, p_value);
}