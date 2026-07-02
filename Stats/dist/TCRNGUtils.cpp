



#include "TCRNGUtils.h"

TCUInt64 RNGStateManager::seed = time(NULL);
TCRNGStatePtr RNGStateManager::stream = NULL;