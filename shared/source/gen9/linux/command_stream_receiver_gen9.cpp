/*
 * Copyright (C) 2018-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/command_stream/command_stream_receiver_with_aub_dump.inl"
#include "shared/source/gen9/hw_cmds.h"
#include "shared/source/os_interface/linux/device_command_stream.inl"
#include "shared/source/os_interface/linux/drm_command_stream.inl"

namespace NEO {

template class DeviceCommandStreamReceiver<Gen9Family>;
template class DrmCommandStreamReceiver<Gen9Family>;
template class CommandStreamReceiverWithAUBDump<DrmCommandStreamReceiver<Gen9Family>>;
} // namespace NEO
