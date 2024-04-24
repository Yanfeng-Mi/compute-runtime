/*
 * Copyright (C) 2020-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef _OCLOC_API_H
#define _OCLOC_API_H
#if defined(__cplusplus)
#pragma once
#endif

#include <cstdint>

#ifndef OCLOC_MAKE_VERSION
/// Generates ocloc API versions
#define OCLOC_MAKE_VERSION(_major, _minor) ((_major << 16) | (_minor & 0x0000ffff))
#endif // OCLOC_MAKE_VERSION

// NOLINTBEGIN(readability-identifier-naming)
typedef enum _ocloc_version_t {
    OCLOC_VERSION_1_0 = OCLOC_MAKE_VERSION(1, 0),     ///< version 1.0
    OCLOC_VERSION_CURRENT = OCLOC_MAKE_VERSION(1, 0), ///< latest known version
    OCLOC_VERSION_FORCE_UINT32 = 0x7fffffff
} ocloc_version_t;

typedef enum _ocloc_error_t {
    OCLOC_SUCCESS = 0,
    OCLOC_OUT_OF_HOST_MEMORY = -6,
    OCLOC_BUILD_PROGRAM_FAILURE = -11,
    OCLOC_INVALID_DEVICE = -33,
    OCLOC_INVALID_PROGRAM = -44,
    OCLOC_INVALID_COMMAND_LINE = -5150,
    OCLOC_INVALID_FILE = -5151,
    OCLOC_COMPILATION_CRASH = -5152,
} ocloc_error_t;

#define OCLOC_NAME_VERSION_MAX_NAME_SIZE 64
typedef struct _ocloc_name_version {
    unsigned int version;
    char name[OCLOC_NAME_VERSION_MAX_NAME_SIZE];
} ocloc_name_version;
// NOLINTEND(readability-identifier-naming)

#ifdef _WIN32
#define SIGNATURE __declspec(dllexport) int __cdecl
#else
#define SIGNATURE int
#endif

extern "C" {
/// Invokes ocloc API using C interface. Supported commands match
/// the functionality of ocloc executable (check ocloc's "help"
/// for reference : shared/offline_compiler/source/ocloc_api.cpp).
///
/// numArgs and argv params represent the command line.
/// Remaining params represent I/O.
/// Output params should be freed using oclocFreeOutput when
/// no longer needed.
/// List and names of outputs match outputs of ocloc executable.
///
/// \param numArgs is the number of arguments to pass to ocloc
///
/// \param argv is an array of arguments to be passed to ocloc
///
/// \param numSources is the number of in-memory representations
/// of source files to be passed to ocloc
///
/// \param dataSources is an array of in-memory representations
/// of source files to be passed to ocloc
///
/// \param lenSources is an array of sizes of in-memory representations
/// of source files passed to ocloc as dataSources
///
/// \param nameSources is an array of names of in-memory representations
/// of source files passed to ocloc as dataSources
///
/// \param numInputHeaders is the number of in-memory representations
/// of header files to be passed to ocloc
///
/// \param dataInputHeaders is an array of in-memory representations
/// of header files to be passed to ocloc
///
/// \param lenInputHeaders is an array of sizes of in-memory representations
/// of header files passed to ocloc as dataInputHeaders
///
/// \param nameInputHeaders is an array of names of in-memory representations
/// of header files passed to ocloc as dataInputHeaders
///
/// \param numOutputs returns the number of outputs
///
/// \param dataOutputs returns an array of in-memory representations
/// of output files
///
/// \param lenOutputs returns an array of sizes of in-memory representations
/// of output files
///
/// \param nameOutputs returns an array of names of in-memory representations
/// of output files. Special name stdout.log describes output that contains
/// messages generated by ocloc (e.g. compiler errors/warnings)
///
/// \returns 0 on succes. Returns non-0 in case of failure.
SIGNATURE oclocInvoke(unsigned int numArgs, const char *argv[],
                      const uint32_t numSources, const uint8_t **dataSources, const uint64_t *lenSources, const char **nameSources,
                      const uint32_t numInputHeaders, const uint8_t **dataInputHeaders, const uint64_t *lenInputHeaders, const char **nameInputHeaders,
                      uint32_t *numOutputs, uint8_t ***dataOutputs, uint64_t **lenOutputs, char ***nameOutputs);

/// Frees results of oclocInvoke
///
/// \param numOutputs is number of outputs as returned by oclocInvoke
///
/// \param dataOutputs is array of outputs as returned by oclocInvoke
///
/// \param lenOutputs is array of sizes of outputs as returned by oclocInvoke
///
/// \param nameOutputs is array of names of outputs as returned by oclocInvoke
///
/// \returns 0 on succes. Returns non-0 in case of failure.
SIGNATURE oclocFreeOutput(uint32_t *numOutputs, uint8_t ***dataOutputs, uint64_t **lenOutputs, char ***nameOutputs);

/// Returns the current version of oclock
SIGNATURE oclocVersion();
}
#endif //_OCLOC_API_H
