#
# Copyright (C) 2020-2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

target_sources(${TARGET_NAME} PRIVATE
               ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
               ${CMAKE_CURRENT_SOURCE_DIR}/test_context.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/test_context_${DRIVER_MODEL}.cpp
)

add_subdirectories()
