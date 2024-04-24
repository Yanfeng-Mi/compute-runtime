/*
 * Copyright (C) 2022-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/command_stream/linear_stream.h"
#include "shared/source/helpers/aligned_memory.h"
#include "shared/source/helpers/gfx_core_helper.h"
#include "shared/source/helpers/hw_info.h"
#include "shared/source/helpers/per_thread_data.h"
#include "shared/source/kernel/local_ids_cache.h"
#include "shared/test/common/helpers/default_hw_info.h"
#include "shared/test/common/mocks/mock_graphics_allocation.h"
#include "shared/test/common/test_macros/test.h"

class MockLocalIdsCache : public NEO::LocalIdsCache {
  public:
    using Base = NEO::LocalIdsCache;
    using Base::Base;
    using Base::cache;
    MockLocalIdsCache(size_t cacheSize) : MockLocalIdsCache(cacheSize, 32u){};
    MockLocalIdsCache(size_t cacheSize, uint8_t simd) : Base(cacheSize, {0, 1, 2}, simd, 32, false){};
};
struct LocalIdsCacheFixture {
    void setUp() {
        localIdsCache = std::make_unique<MockLocalIdsCache>(1);
    }
    void tearDown() {}

    std::array<uint8_t, 2048> perThreadData = {0};
    Vec3<uint16_t> groupSize = {128, 2, 1};
    std::unique_ptr<MockLocalIdsCache> localIdsCache;
};

using LocalIdsCacheTests = Test<LocalIdsCacheFixture>;
TEST_F(LocalIdsCacheTests, GivenCacheMissWhenGetLocalIdsForGroupThenNewEntryIsCommitedIntoLeastUsedEntry) {
    localIdsCache->cache.resize(2);
    localIdsCache->cache[0].accessCounter = 2U;
    auto gfxCoreHelper = NEO::GfxCoreHelper::create(NEO::defaultHwInfo->platform.eRenderCoreFamily);
    localIdsCache->setLocalIdsForGroup(groupSize, perThreadData.data(), *gfxCoreHelper.get());

    EXPECT_EQ(groupSize, localIdsCache->cache[1].groupSize);
    EXPECT_NE(nullptr, localIdsCache->cache[1].localIdsData);
    EXPECT_EQ(1536U, localIdsCache->cache[1].localIdsSize);
    EXPECT_EQ(1536U, localIdsCache->cache[1].localIdsSizeAllocated);
    EXPECT_EQ(1U, localIdsCache->cache[1].accessCounter);
}

TEST_F(LocalIdsCacheTests, GivenEntryInCacheWhenGetLocalIdsForGroupThenEntryFromCacheIsUsed) {
    localIdsCache->cache[0].groupSize = groupSize;
    localIdsCache->cache[0].localIdsData = static_cast<uint8_t *>(alignedMalloc(512, 32));
    localIdsCache->cache[0].localIdsSize = 512U;
    localIdsCache->cache[0].localIdsSizeAllocated = 512U;
    localIdsCache->cache[0].accessCounter = 1U;
    auto gfxCoreHelper = NEO::GfxCoreHelper::create(NEO::defaultHwInfo->platform.eRenderCoreFamily);
    localIdsCache->setLocalIdsForGroup(groupSize, perThreadData.data(), *gfxCoreHelper.get());
    EXPECT_EQ(2U, localIdsCache->cache[0].accessCounter);
}

TEST_F(LocalIdsCacheTests, GivenEntryWithBiggerBufferAllocatedWhenGetLocalIdsForGroupThenBufferIsReused) {
    localIdsCache->cache[0].groupSize = {4, 1, 1};
    localIdsCache->cache[0].localIdsData = static_cast<uint8_t *>(alignedMalloc(512, 32));
    localIdsCache->cache[0].localIdsSize = 512U;
    localIdsCache->cache[0].localIdsSizeAllocated = 512U;
    localIdsCache->cache[0].accessCounter = 2U;
    const auto localIdsData = localIdsCache->cache[0].localIdsData;

    groupSize = {2, 1, 1};
    auto gfxCoreHelper = NEO::GfxCoreHelper::create(NEO::defaultHwInfo->platform.eRenderCoreFamily);
    localIdsCache->setLocalIdsForGroup(groupSize, perThreadData.data(), *gfxCoreHelper.get());
    EXPECT_EQ(1U, localIdsCache->cache[0].accessCounter);
    EXPECT_EQ(192U, localIdsCache->cache[0].localIdsSize);
    EXPECT_EQ(512U, localIdsCache->cache[0].localIdsSizeAllocated);
    EXPECT_EQ(localIdsData, localIdsCache->cache[0].localIdsData);
}

TEST_F(LocalIdsCacheTests, GivenValidLocalIdsCacheWhenGettingLocalIdsSizePerThreadThenCorrectValueIsReturned) {
    auto localIdsSizePerThread = localIdsCache->getLocalIdsSizePerThread();
    EXPECT_EQ(192U, localIdsSizePerThread);
}

TEST_F(LocalIdsCacheTests, GivenValidLocalIdsCacheWhenGettingLocalIdsSizeForGroupThenCorrectValueIsReturned) {
    auto gfxCoreHelper = NEO::GfxCoreHelper::create(NEO::defaultHwInfo->platform.eRenderCoreFamily);
    auto localIdsSizePerThread = localIdsCache->getLocalIdsSizeForGroup(groupSize, *gfxCoreHelper.get());
    EXPECT_EQ(1536U, localIdsSizePerThread);
}

TEST(LocalIdsCacheTest, givenSimd1WhenGettingLocalIdsSizeForGroupThenCorrectValueIsReturned) {
    auto gfxCoreHelper = NEO::GfxCoreHelper::create(NEO::defaultHwInfo->platform.eRenderCoreFamily);
    auto localIdsCache = std::make_unique<MockLocalIdsCache>(1u, 1u);
    Vec3<uint16_t> groupSize = {128, 2, 1};
    auto localIdsSizePerThread = localIdsCache->getLocalIdsSizeForGroup(groupSize, *gfxCoreHelper.get());
    auto expectedLocalIdsSizePerThread = groupSize[0] * groupSize[1] * groupSize[2] * localIdsCache->getLocalIdsSizePerThread();
    EXPECT_EQ(expectedLocalIdsSizePerThread, localIdsSizePerThread);
}