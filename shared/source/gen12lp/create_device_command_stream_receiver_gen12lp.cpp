/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/command_stream/device_command_stream.h"
#include "shared/source/execution_environment/execution_environment.h"
#include "shared/source/gen12lp/hw_cmds.h"

#include "create_command_stream_receiver.inl"

namespace NEO {

template <>
CommandStreamReceiver *createDeviceCommandStreamReceiver<Gen12LpFamily>(bool withAubDump,
                                                                        ExecutionEnvironment &executionEnvironment,
                                                                        uint32_t rootDeviceIndex,
                                                                        const DeviceBitfield deviceBitfield) {
    return createCommandStreamReceiver<Gen12LpFamily>(withAubDump, executionEnvironment, rootDeviceIndex, deviceBitfield);
}

} // namespace NEO
