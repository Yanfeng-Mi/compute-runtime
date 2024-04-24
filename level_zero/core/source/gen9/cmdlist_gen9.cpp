/*
 * Copyright (C) 2022-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/command_container/command_encoder.h"
#include "shared/source/gen9/hw_cmds_base.h"
#include "shared/source/helpers/gfx_core_helper.h"
#include "shared/source/helpers/pipe_control_args.h"

#include "level_zero/core/source/cmdlist/cmdlist_hw.h"
#include "level_zero/core/source/cmdlist/cmdlist_hw.inl"
#include "level_zero/core/source/cmdlist/cmdlist_hw_immediate.h"
#include "level_zero/core/source/cmdlist/cmdlist_hw_immediate.inl"
#include "level_zero/core/source/cmdlist/cmdlist_hw_skl_to_tgllp.inl"
#include "level_zero/core/source/device/device.h"

#include "cmdlist_extended.inl"

namespace L0 {

template <>
void CommandListCoreFamily<IGFX_GEN9_CORE>::applyMemoryRangesBarrier(uint32_t numRanges,
                                                                     const size_t *pRangeSizes,
                                                                     const void **pRanges) {
    NEO::PipeControlArgs args;
    args.dcFlushEnable = this->dcFlushSupport;
    NEO::MemorySynchronizationCommands<GfxFamily>::addSingleBarrier(*commandContainer.getCommandStream(), args);
}

template <>
void CommandListCoreFamily<IGFX_GEN9_CORE>::programL3(bool isSLMused) {
    NEO::EncodeL3State<GfxFamily>::encode(commandContainer, isSLMused);
}

template struct CommandListCoreFamily<IGFX_GEN9_CORE>;
template struct CommandListCoreFamilyImmediate<IGFX_GEN9_CORE>;
} // namespace L0
