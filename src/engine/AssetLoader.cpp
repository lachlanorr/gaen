//------------------------------------------------------------------------------
// AssetLoader.cpp - Loads assets from disk and manages lifetimes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "assets/file_utils.h"
#include "engine/MessageQueue.h"
#include "engine/Asset.h"
#include "engine/AssetType.h"
#include "engine/AssetTypes.h"

#include "engine/messages/Asset.h"

#include "engine/AssetLoader.h"

namespace gaen
{

AssetLoader::AssetLoader(u32 loaderId,
                         const String<kMEM_Engine> & assetsRootPath,
                         const AssetTypes & assetTypes)
  : mLoaderId(loaderId)
  , mAssetsRootPath(assetsRootPath)
  , mAssetTypes(assetTypes)
{
    mCreatorThreadId = active_thread_id();

    mQueueSize = 0;

    mpRequestQueue = GNEW(kMEM_Engine, MessageQueue, kMaxAssetMessages);
    mpReadyQueue = GNEW(kMEM_Engine, MessageQueue, kMaxAssetMessages);

    mThread = std::thread(&AssetLoader::threadProc, this);
}

AssetLoader::~AssetLoader()
{
    ASSERT(!mIsRunning);

    GDELETE(mpRequestQueue);
    GDELETE(mpReadyQueue);
}

template <typename T>
void AssetLoader::queueRequest(const T & msgAcc)
{
    ASSERT(mCreatorThreadId == active_thread_id());
    mpRequestQueue->transcribeMessage(msgAcc);
}

void AssetLoader::stopAndJoin()
{
    mIsRunning = false;
    mThread.join();
}


void AssetLoader::threadProc()
{
    init_time(); // must be initialized on every thread to support logging timestamps
    set_active_thread_id(mLoaderId); // set thread id for tracking purposes

    mIsRunning = true;

    MessageQueueAccessor msgAcc;

    while (mIsRunning)
    {
        while (mpRequestQueue->popBegin(&msgAcc))
        {
            message(msgAcc);
            mpRequestQueue->popCommit(msgAcc);
        }
        sleep(10);
    }
}

template <typename T>
MessageResult AssetLoader::message(const T& msgAcc)
{
    Message msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::request_asset__:
    {
        CmpString pathCmpString;
        u32 subTaskId;
        u32 nameHash;

        extract_request_asset(msgAcc,
                              mBlockMemory,
                              pathCmpString,
                              subTaskId,
                              nameHash);

        char fullPath[kMaxPath+1];
        strcpy(fullPath, mAssetsRootPath.c_str());
        strcat(fullPath, pathCmpString.c_str());

        const AssetType * pAT = mAssetTypes.assetTypeFromExt(get_ext(pathCmpString.c_str()));
        Asset * pAsset = pAT->construct(pathCmpString.c_str(), fullPath);

        LOG_INFO("ASSET READ: %s", pathCmpString.c_str());

        messages::AssetQW msgw(HASH::asset_ready__, kMessageFlag_None, kAssetMgrTaskId, kAssetMgrTaskId, msg.source, mpReadyQueue);
        msgw.setSubTaskId(subTaskId);
        msgw.setNameHash(nameHash);
        msgw.setAsset(pAsset);

        return MessageResult::Consumed;
    }
    default:
        PANIC("Unhandled message id sent to AssetLoader: %u", msg.msgId);
        return MessageResult::Consumed;
    }
}

template <typename T>
void AssetLoader::extract_request_asset(const T & msgAcc,
                                        BlockMemory & blockMemory,
                                        CmpString & pathCmpString,
                                        u32 & subTaskid,
                                        u32 & nameHash)
{
    Message msg = msgAcc.message();

    ASSERT(msg.msgId == HASH::request_asset__);
    PANIC_IF(msg.blockCount < 2, "Too few bytes for request_asset message");

    subTaskid = msg.payload.u;
    nameHash = msgAcc[0].cells[0].u;

    const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[1]);

    PANIC_IF(pBlockData->type != kBKTY_String, "No path string sent in request_asset message");

    u32 requiredBlockCount = pBlockData->blockCount;

    // -1 below is for the block 0 in the message required to send the nameHash
    PANIC_IF(msg.blockCount - 1 < requiredBlockCount, "Too few bytes for request_asset path string");

    Address addr = blockMemory.allocCopy(pBlockData);
    pathCmpString = blockMemory.string(addr);
}


// Template decls so we can define message func here in the .cpp
template MessageResult AssetLoader::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult AssetLoader::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

template void AssetLoader::queueRequest<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template void AssetLoader::queueRequest<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

template void AssetLoader::extract_request_asset<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc,
                                                                       BlockMemory & blockMemory,
                                                                       CmpString & pathCmpString,
                                                                       u32 & subTaskid,
                                                                       u32 & nameHash);
template void AssetLoader::extract_request_asset<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc,
                                                                       BlockMemory & blockMemory,
                                                                       CmpString & pathCmpString,
                                                                       u32 & subTaskid,
                                                                       u32 & nameHash);

} // namespace gaen
