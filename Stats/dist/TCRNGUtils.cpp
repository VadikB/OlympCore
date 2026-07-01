//<copyright>
// Copyright (c) Microsoft Corporation.  All rights reserved.
//</copyright>

#include "TCRNGUtils.h"

TCUInt64 RNGStateManager::seed = time(NULL);
TCRNGStatePtr RNGStateManager::stream = NULL;