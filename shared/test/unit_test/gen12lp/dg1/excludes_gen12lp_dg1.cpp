/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/test/common/test_macros/hw_test_base.h"

HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, givenProductHelperWhenAskedIfPipeControlWAIsRequiredThenFalseIsReturned, IGFX_DG1);
HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, givenProductHelperWhenAskedIfImagePitchAlignmentWAIsRequiredThenFalseIsReturned, IGFX_DG1);
HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, givenProductHelperWhenAskedIfForceEmuInt32DivRemSPWAIsRequiredThenFalseIsReturned, IGFX_DG1);
HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, givenProductHelperWhenAskedIf3DPipelineSelectWAIsRequiredThenFalseIsReturned, IGFX_DG1);
HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, givenProductHelperWhenAskedIfStorageInfoAdjustmentIsRequiredThenFalseIsReturned, IGFX_DG1);
HWTEST_EXCLUDE_PRODUCT(ProductHelperTest, whenOverrideGfxPartitionLayoutForWslThenReturnFalse, IGFX_DG1);