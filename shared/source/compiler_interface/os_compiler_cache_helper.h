/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include <string>

namespace NEO {
class EnvironmentVariableReader;
int64_t defaultCacheEnabled();
bool checkDefaultCacheDirSettings(std::string &cacheDir, NEO::EnvironmentVariableReader &reader);
time_t getFileModificationTime(const std::string &path);
size_t getFileSize(const std::string &path);
} // namespace NEO