//------------------------------------------------------------------------------
// AssetLoader.h - Loads assets from disk and manages lifetimes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_ENGINE_ASSET_LOADER_H
#define GAEN_ENGINE_ASSET_LOADER_H

#include "gaen/core/mem.h"
#include "gaen/core/threading.h"
#include "gaen/core/String.h"
#include "gaen/engine/MessageQueue.h"
#include "gaen/engine/BlockMemory.h"

namespace gaen
{

class AssetTypes;

class AssetLoader
{
public:
    static const u32 kMaxAssetMessages = 4096;

    AssetLoader(u32 loaderId,
                const String<kMEM_Engine> & assetsRootPath,
                const AssetTypes & assetTypes);
    ~AssetLoader();

    template <typename T>
    void queueRequest(const T & msgAcc);
    MessageQueue & requestQueue()
    {
        ASSERT(mCreatorThreadId == active_thread_id());
        return *mpRequestQueue;
    }
    MessageQueue & readyQueue()
    {
        ASSERT(mCreatorThreadId == active_thread_id());
        return *mpReadyQueue;
    }

    void stopAndJoin();

    u32 queueSize()
    {
        ASSERT(mCreatorThreadId == active_thread_id());
        return mQueueSize;
    }
    void incQueueSize()
    {
        ASSERT(mCreatorThreadId == active_thread_id());
        mQueueSize++;
    }
    void decQueueSize()
    {
        ASSERT(mCreatorThreadId == active_thread_id());
        mQueueSize--;
    }

    template <typename T>
    static void extract_request_asset(const T & msgAcc,
                                      BlockMemory & blockMemory,
                                      CmpString & pathCmpString,
                                      u32 & requestorTaskId,
                                      u32 & nameHash);
private:
    void threadProc();

    template <typename T>
    MessageResult message(const T& msgAcc);

    // Track creator's thread id so we can ensure no other thread calls us.
    // If they do, our SPSC queue design breaks down.
    thread_id mCreatorThreadId;
    
    u32 mLoaderId;
    const String<kMEM_Engine> & mAssetsRootPath;
    const AssetTypes & mAssetTypes;

    bool mIsRunning = false;

    u32 mQueueSize;

    std::thread mThread;

    BlockMemory mBlockMemory;

    MessageQueue * mpRequestQueue;
    MessageQueue * mpReadyQueue;
}; // AssetLoader

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_LOADER_H
