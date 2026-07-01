#<copyright>
# Copyright (c) Microsoft Corporation.  All rights reserved.
#</copyright>

# build distributed stats library

TARGET_NAME = OlympCore
DLL=1
PLATFORM=x64
PLACE=Server

!if "$(PLATFORM)" == "Win32" || "$(PLATFORM)" == "win32" || "$(PLATFORM)" == "WIN32"
TARGET=
!endif

!include "..\stats.defs"

EXIE = $(BUILD_ROOT)\Release\Product\Exie
EXIEINC = $(EXIE)\Runtime\Include
EXIEBIN = $(EXIE)\Runtime\Binaries\$(PLATFORM)\$(CONFIGURATION)
LIBCOMMON = $(EXIE)\LibCommon
LIBCOMMONDIST = $(LIBCOMMON)\LibCommonDist

DEFINES = $(DEFINES) /DOLYMPCOREDIST_EXPORTS /DMKL_ILP64

INCLUDES =  \
  $(INCLUDES) \
  /I..\..\Math\Distributed\ReductionsScans \
  /I..\..\..\Public\MicrosoftHPCPack2008R2SDK\Include \
  /I$(EXIEINC) \
  /I$(LIBCOMMON) \
  /I$(LIBCOMMONDIST) \

LIBS = \
  $(LIBS) \
  ..\..\..\Public\MicrosoftHPCPack2008R2SDK\Lib\$(MS_PLATFORM)\msmpi.lib \
  $(TARGET_DIR_COMMON)\TC_mkl_dist.lib \
  $(TARGET_DIR_COMMON)\LibCommonDist.lib \
  $(TARGET_DIR_COMMON)\LibCommon.lib \
  $(EXIEBIN)\ExieRuntime.lib \
  $(EXIEBIN)\ExieDistributedRuntime.lib \

SYSTEM_LIBS = \
  user32.lib

# note: since OlympCore.h may be rebuilt whenever Stats is rebuilt,
# all objects in this directory may be rebuilt whenever Stats is rebuilt

DEPS = \
  $(DEPS) \
  $(TCINCLUDE)\OlympCoreDist.h \
  $(LIBCOMMON)\LibCommon.h \
  $(LIBCOMMONDIST)\LibCommonDist.h \

OBJS = \
  $(OBJS) \
  $(INT_DIR)\slope_dist.obj \
  $(INT_DIR)\intercept_dist.obj \
  $(INT_DIR)\forecast_dist.obj \
  $(INT_DIR)\steyx_dist.obj \
  $(INT_DIR)\regression_dist.obj \
  $(INT_DIR)\median_dist.obj \
  $(INT_DIR)\z_test_dist.obj \
  $(INT_DIR)\median_dev_dist.obj \
  $(INT_DIR)\quantile_exc_dist.obj \
  $(INT_DIR)\quantile_inc_dist.obj \
  $(INT_DIR)\ks_test_dist.obj \
  $(INT_DIR)\percentrank_inc_dist.obj \
  $(INT_DIR)\percentrank_exc_dist.obj \
  $(INT_DIR)\small_dist.obj \
  $(INT_DIR)\large_dist.obj \
  $(INT_DIR)\ks2_test_dist.obj \
  $(INT_DIR)\interquartile_dist_dist.obj \
  $(INT_DIR)\mw_test_dist.obj \
  $(INT_DIR)\anova_single_dist.obj \
  $(INT_DIR)\anova_two_dist.obj \
  $(INT_DIR)\anova_tworep_dist.obj \
  $(INT_DIR)\crosstabulate_numbers_dist.obj \

!include "..\stats.targets"

$(INT_DIR)\slope_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\intercept_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\forecast_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\steyx_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\regression_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\median_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\z_test_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Inference\z_test_dist.h
$(INT_DIR)\ks_test_dist.obj: ..\Distributed\Inference\ks_test_dist.h
$(INT_DIR)\ks2_test_dist.obj: ../Distributed/Inference/ks2_test_dist.h
$(INT_DIR)\median_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h  ..\Distributed\Descriptive\median_dist.h
$(INT_DIR)\median_dev_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h  ..\Distributed\Descriptive\median_dev_dist.h
$(INT_DIR)\small_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h  ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\small_dist.h
$(INT_DIR)\large_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h  ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\large_dist.h
$(INT_DIR)\interquartile_dist_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\interquartile_dist_dist.h
$(INT_DIR)\percentrank_inc_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\percentrank_inc_dist.h
$(INT_DIR)\percentrank_exc_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\percentrank_exc_dist.h
$(INT_DIR)\quantile_exc_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\quantile_exc_dist.h
$(INT_DIR)\quantile_inc_dist.obj: $(TCINCLUDE)\OlympCoreDist.h ..\Distributed\Descriptive\qselect_dist.h ..\Distributed\Descriptive\qselect_utils_dist.h ..\Distributed\Descriptive\quantile_inc_dist.h
$(INT_DIR)\mw_test_dist.obj: ../Distributed/Inference/mw_test_dist.h
$(INT_DIR)\anova_single_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\anova_two_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\anova_tworep_dist.obj: $(TCINCLUDE)\OlympCoreDist.h
$(INT_DIR)\crosstabulate_numbers_dist.obj: $(TCINCLUDE)\OlympCoreDist.h

{..\Distributed\Regression}.cc{$(INT_DIR)}.obj:
    $(ICL) /c $(CPPFLAGS) /Fo$@ $<

{..\Distributed\Descriptive}.cpp{$(INT_DIR)}.obj:
    $(ICL) /c $(CPPFLAGS) /Fo$@ $<

{..\Distributed\Inference}.cpp{$(INT_DIR)}.obj:
    $(ICL) /c $(CPPFLAGS) /Fo$@ $<

{..\Distributed\Inference}.cc{$(INT_DIR)}.obj:
    $(ICL) /c $(CPPFLAGS) /Fo$@ $<

{..\Distributed\Inference}.cpp{$(INT_DIR)}.obj:
    $(ICL) /c $(CPPFLAGS) /Fo$@ $<
