//------------------------------------------------------------------------------
// AssetLoader.h - Loads assets from disk and manages lifetimes.
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

#ifndef GAEN_ENGINE_ASSET_LOADER_H
#define GAEN_ENGINE_ASSET_LOADER_H

#include "core/mem.h"
#include "core/threading.h"
#include "engine/MessageQueue.h"
#include "engine/BlockMemory.h"

namespace gaen
{

class AssetLoader
{
public:
    static const u32 kMaxAssetMessages = 4096;

    AssetLoader(u32 loaderId);
    ~AssetLoader();

    void queueRequest(const MessageQueueAccessor & msgAcc);
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

    static MemType mem_type_from_ext(const char * ext);

private:
    void threadProc();

    MessageResult message(const MessageQueueAccessor& msgAcc);

    // Track creator's thread id so we can ensure no other thread calls us.
    // If they do, our SPSC queue design breaks down.
    thread_id mCreatorThreadId;
    
    u32 mLoaderId;
    bool mIsRunning = false;

    u32 mQueueSize;

    std::thread mThread;

    BlockMemory mBlockMemory;

    MessageQueue * mpRequestQueue;
    MessageQueue * mpReadyQueue;
}; // AssetLoader

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_ASSET_LOADER_H
