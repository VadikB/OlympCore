#<copyright>
# Copyright (c) Microsoft Corporation.  All rights reserved.
#</copyright>

# build stats library

TARGET_NAME = OlympCore
DLL=1
FFI=1
PLACE=Client

!include "..\stats.defs"

DEFINES = $(DEFINES) /DOLYMPCORE_EXPORTS

INCLUDES =  \
  $(INCLUDES) \

LIBS = $(LIBS) $(TARGET_DIR_COMMON)\TC_mkl_sequential.lib  $(TARGET_DIR_COMMON)\TC_mkl_omp.lib

# note: since TCMath.h may be rebuilt whenever Math is rebuilt,
# all objects in this directory may be rebuilt whenever Math is rebuilt

DEPS = \
  $(DEPS) \

OBJS = \
  $(OBJS)

!include "..\stats.targets"

