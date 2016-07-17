//------------------------------------------------------------------------------
// Asset.h - Manages asset loading
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#ifndef GAEN_ENGINE_ASSET_MGR_H
#define GAEN_ENGINE_ASSET_MGR_H

#include "core/HashMap.h"
#include "core/String.h"
#include "core/Vector.h"
#include "core/threading.h"
#include "assets/AssetTypes.h"
#include "engine/Message.h"
#include "engine/BlockMemory.h"
#include "engine/Asset.h"

#define VALIDATE_ASSETS WHEN(HAS(DEV_BUILD))

#if HAS(VALIDATE_ASSETS)
#define IS_VALID(assetType, assetPtr) do {PANIC_IF(!assetType::is_valid(assetPtr->buffer(), assetPtr->size()), "Invalid asset of type: " #assetType);} while(0)
#else  // #if HAS(ASSERTS)
#define IS_VALID(assetType, assetPtr) do {} while(0)
#endif // #if HAS(ASSERTS)

namespace gaen
{

class AssetLoader;

class AssetMgr
{
public:
    AssetMgr(u32 assetLoaderCount);
    ~AssetMgr();

    void process();

    template <typename T>
    MessageResult message(const T& msgAcc);

    static void addref_asset(task_id source, Asset * pAsset);
    static void release_asset(task_id source, Asset * pAsset);
private:
    AssetLoader * findLeastBusyAssetLoader();
    void sendAssetReadyHandle(Asset * pAsset,
                              task_id entityTask,
                              task_id entitySubTask,
                              u32 nameHash);

    // Track creator's thread id so we can ensure no other thread calls us.
    // If they do, our SPSC queue design breaks down.
    thread_id mCreatorThreadId;

    AssetTypes mAssetTypes;

    BlockMemory mBlockMemory;

    String<kMEM_Engine> mAssetsRootPath;

    u32 mAssetLoaderCount;
    Vector<kMEM_Engine, AssetLoader*> mAssetLoaders;

    HashMap<kMEM_Engine, String<kMEM_Engine>, Asset*> mAssets;
    HashMap<kMEM_Engine, String<kMEM_Engine>, std::list<std::tuple<task_id, task_id, u32>>> mDuplicateRequestTargets;

    HashMap<kMEM_Engine, String<kMEM_Engine>, std::list<std::tuple<u32, Asset*, task_id, task_id, u32>>> mAssetsWaitingForDependent;

}; // AssetMgr

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_MGR_H
