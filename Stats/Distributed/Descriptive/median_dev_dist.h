//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#pragma once

#include <vector>

#include "TCError.h"
#include "../Descriptive/median_dev.h"
#include "qselect_utils_dist.h"
#include "qselect_dist.h"

namespace Descriptive
{

template <typename T>
struct median_dev_dist_t : public median_dev_t<T>, public Sort::operator_temp_copy_dist_t
{
};


} /* namespace Descriptive */ 
