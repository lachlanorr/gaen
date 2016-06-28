//------------------------------------------------------------------------------
// HandleMgr.cpp - Manages access to Handle data, enabling lockless concurrency
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

#include "engine/stdafx.h"

#include "core/logging.h"
#include "core/mem.h"
#include "engine/hashes.h"
#include "engine/messages/Handle.h"
#include "engine/MessageQueue.h"
#include "engine/AssetLoader.h"
#include "engine/TaskMaster.h"
#include "engine/HandleMgr.h"

namespace gaen
{

// Easily distinguish AssetLoader threads from TaskManager threads
static const u32 kInitialAssetLoaderThreadId = 0xA55E70; // 10837616

HandleMgr::HandleMgr(u32 assetLoaderCount)
{
    mCreatorThreadId = active_thread_id();

    ASSERT(assetLoaderCount > 0 && assetLoaderCount <= 4);
    mAssetLoaderCount = assetLoaderCount;

    mAssetLoaders.reserve(mAssetLoaderCount);

    for (u32 i = 0; i < mAssetLoaderCount; ++i)
    {
        mAssetLoaders.push_back(GNEW(kMEM_Engine, AssetLoader, i + kInitialAssetLoaderThreadId));
    }
}

HandleMgr::~HandleMgr()
{
    for (auto pLdr : mAssetLoaders)
    {
        pLdr->stopAndJoin();
        GDELETE(pLdr);
    }
}

void HandleMgr::process()
{
    ASSERT(mCreatorThreadId == active_thread_id());

    MessageQueueAccessor msgAcc;

    for (auto pLdr : mAssetLoaders)
    {
        while (pLdr->readyQueue().popBegin(&msgAcc))
        {
            message(msgAcc);
            pLdr->readyQueue().popCommit(msgAcc);
            pLdr->decQueueSize();
        }
    }
}

template <typename T>
MessageResult HandleMgr::message(const T & msgAcc)
{
    ASSERT(mCreatorThreadId == active_thread_id());

    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::request_asset__:
    {
        AssetLoader * pLdr = findLeastBusyAssetLoader();
        pLdr->queueRequest(msgAcc);
        pLdr->incQueueSize();
        return MessageResult::Consumed;
    }
    case HASH::asset_ready__:
    {
        MessageQueue * pMsgQ = get_message_queue(msgAcc.message().source,
                                                 msgAcc.message().source);
        pMsgQ->transcribeMessage(msgAcc);
        return MessageResult::Consumed;
    }
    default:
        PANIC("Unknown HandleMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;
}

AssetLoader * HandleMgr::findLeastBusyAssetLoader()
{
    ASSERT(mCreatorThreadId == active_thread_id());

    AssetLoader * pLoader = nullptr;
    u32 queueSize = std::numeric_limits<u32>::max();;

    for (auto pLdr : mAssetLoaders)
    {
        if (pLdr->queueSize() < queueSize)
        {
            queueSize = pLdr->queueSize();
            pLoader = pLdr;
        }
    }

    ASSERT(pLoader);
    return pLoader;
}


// Template decls so we can define message func here in the .cpp
template MessageResult HandleMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);

} // namespace gaen
