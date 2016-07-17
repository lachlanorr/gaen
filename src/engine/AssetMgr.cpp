//------------------------------------------------------------------------------
// Asset.cpp - Manages asset loading
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
#include "assets/file_utils.h"
#include "hashes/hashes.h"
#include "engine/messages/Handle.h"
#include "engine/MessageQueue.h"
#include "engine/AssetLoader.h"
#include "engine/TaskMaster.h"

#include "engine/messages/Asset.h"

#include "engine/AssetMgr.h"


namespace gaen
{

// Easily distinguish AssetLoader threads from TaskManager threads
static const u32 kInitialAssetLoaderThreadId = 0xA55E70; // 10837616

void asset_handle_free(Handle & handle)
{
    ASSERT(handle.typeHash() == HASH::asset);
    if (handle.data())
    {
        Asset * pAsset = reinterpret_cast<Asset*>(handle.data());
        messages::AssetQW msgw(HASH::release_asset__, kMessageFlag_None, handle.owner(), kAssetMgrTaskId, handle.owner());
        msgw.setSubTaskId(0);
        msgw.setNameHash(0);
        msgw.setAsset(pAsset);
    }
}

AssetMgr::AssetMgr(u32 assetLoaderCount)
{
    mCreatorThreadId = active_thread_id();

    ASSERT(assetLoaderCount > 0 && assetLoaderCount <= 4);
    mAssetLoaderCount = assetLoaderCount;

    // find assets path
    char assetsPath[kMaxPath+1];
    find_assets_runtime_dir(assetsPath);
    mAssetsRootPath = assetsPath;

    mAssetLoaders.reserve(mAssetLoaderCount);

    for (u32 i = 0; i < mAssetLoaderCount; ++i)
    {
        mAssetLoaders.push_back(GNEW(kMEM_Engine, AssetLoader, i + kInitialAssetLoaderThreadId, mAssetsRootPath, mAssetTypes));
    }
}

AssetMgr::~AssetMgr()
{
    for (auto pLdr : mAssetLoaders)
    {
        pLdr->stopAndJoin();
        GDELETE(pLdr);
    }
}

void AssetMgr::process()
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

void AssetMgr::sendAssetReadyHandle(Asset * pAsset,
                                    task_id entityTask,
                                    task_id entitySubTask,
                                    u32 nameHash)
{
    ASSERT(mCreatorThreadId == active_thread_id());

    // Dependent entities will have an entityTask of 0, and in those
    // cases we don't need to notify anyone as the parent asset that
    // loaded the dependency will be sent to the requesting task.
    if (entitySubTask != 0)
    {
        // Every time we send this asset to an Entity, we increase
        // reference count.
        pAsset->addRef();

        // Prep a Handle wrapper for the asset
        Handle * pHandle = GNEW(kMEM_Engine,
                                Handle,
                                HASH::asset,
                                pAsset->pathHash(),
                                entityTask,
                                pAsset,
                                asset_handle_free);

        messages::HandleQW msgw(HASH::asset_ready__, kMessageFlag_None, kAssetMgrTaskId, entityTask, entitySubTask);
        msgw.setNameHash(nameHash);
        msgw.setHandle(pHandle);
    }
}


template <typename T>
MessageResult AssetMgr::message(const T & msgAcc)
{
    ASSERT(mCreatorThreadId == active_thread_id());

    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::request_asset__:
    {
        CmpString pathCmpString;
        u32 subTaskId;
        u32 nameHash;

        AssetLoader::extract_request_asset(msgAcc,
                                           mBlockMemory,
                                           pathCmpString,
                                           subTaskId,
                                           nameHash);

        const char * pathStr = pathCmpString.c_str();

        auto it = mAssets.find(pathStr);

        // Only conduct load if asset hasn't already started loading
        if (it == mAssets.end())
        {
            // insert null placeholder so we know it is already
            // loading in case it is requested again before loading
            // finishes.
            mAssets[pathStr] = nullptr;

            AssetLoader * pLdr = findLeastBusyAssetLoader();
            pLdr->queueRequest(msgAcc);
            pLdr->incQueueSize();
        }
        else if (it->second != nullptr)
        {
            // Asset is already loaded
            sendAssetReadyHandle(it->second, msg.source, subTaskId, nameHash);
        }
        else
        {
            // Asset is in the process of loading, started by some other entity.
            // Record the requestor's info so we can send them asset_ready__
            // when loading is complete.
            mDuplicateRequestTargets[pathStr].emplace_back(msg.source, subTaskId, nameHash);
        }

        return MessageResult::Consumed;
    }
    case HASH::asset_ready__:
    {
        messages::AssetR<T> msgr(msgAcc);
        Asset * pAsset = msgr.asset();

        mAssets[pAsset->path()] = pAsset;


        if (pAsset->hadError() || pAsset->isLoaded())
        {
            // If asset starts as full loaded or has an error upon
            // entering this handler, notify all tasks that are
            // waiting for it.

            sendAssetReadyHandle(pAsset, msgr.taskId(), msgr.subTaskId(), msgr.nameHash());

            auto dupIt = mDuplicateRequestTargets.find(pAsset->path());
            if (dupIt != mDuplicateRequestTargets.end())
            {
                for (auto & req : dupIt->second)
                {
                    task_id targetTaskId = std::get<0>(req);
                    task_id requestorTaskId = std::get<1>(req);
                    u32 nameHash = std::get<2>(req);

                    sendAssetReadyHandle(pAsset, targetTaskId, requestorTaskId, nameHash);
                }
                mDuplicateRequestTargets.erase(dupIt);
            }


            // If asset is loaded or there was an error, check the
            // AssetsWaitingForDependent list to see if any other asset
            // was waiting on it as a dependent, and update that asset. If
            // an asset that was waiting on us is now complete, resend to
            // asset_ready__ so we can process it again with this message
            // handler logic.

            // check to see if any assets are waiting on this as a dependent
            auto itList = mAssetsWaitingForDependent.find(pAsset->path());
            if (itList != mAssetsWaitingForDependent.end())
            {
                for (auto tup : itList->second)
                {
                    u32 dependentNameHash = std::get<0>(tup);
                    Asset * pWaitingAsset = std::get<1>(tup);

                    if (pAsset->isLoaded())
                    {
                        ASSERT(pWaitingAsset->mpBuffer && pWaitingAsset->mSize > 0 && pAsset->mpBuffer && pAsset->mSize > 0);
                        // set the dependent
                        pWaitingAsset->mpAssetType->setDependent(dependentNameHash,
                                                                 pWaitingAsset->mpBuffer,
                                                                 pWaitingAsset->mSize,
                                                                 pAsset->mpBuffer,
                                                                 pAsset->mSize);
                    }
                    else if (pAsset->hadError())
                    {
                        pWaitingAsset->mHadError = true;
                    }
                    else
                    {
                        PANIC("Invalid branch, sanity check");
                    }


                    // If it's now full loaded or an error, send us the
                    // asset_ready__ message again so we can properly
                    // process it.
                    //
                    // NOTE: We don't simply send asset_ready__ to the
                    // requestor because it's possible there are items
                    // in the mDuplicateRequestTargets that are also
                    // waiting for it, so the simplest thing is to
                    // just pass it through this message handler
                    // again.
                    if (pWaitingAsset->isLoaded())
                    {
                        task_id targetTaskId = std::get<2>(tup);
                        task_id subTaskId = std::get<3>(tup);
                        u32 nameHash = std::get<4>(tup);

                        messages::AssetQW msgw(HASH::asset_ready__,
                                               kMessageFlag_None,
                                               kAssetMgrTaskId,
                                               kAssetMgrTaskId,
                                               targetTaskId);
                        msgw.setSubTaskId(subTaskId);
                        msgw.setNameHash(nameHash);
                        msgw.setAsset(pWaitingAsset);
                    }
                }
                mAssetsWaitingForDependent.erase(pAsset->path());
            }
        }


        else if (!pAsset->isLoaded())
        {
            // If  the asset isn't fully loaded, deal with dependents:
            // 1. For each loaded dependent, set it on the asset
            // 2. For each non-loaded dependent, send request_asset__
            //    messages to ourselves and add this asset to the
            //    AssetsWaitingForDependent list for each asset is is
            //    waiting on.
            ASSERT(pAsset->mpBuffer && pAsset->mSize > 0);
            DependentVecUP deps = pAsset->mpAssetType->dependents(pAsset->mpBuffer, pAsset->mSize);

            if (deps.get() != nullptr)
            {
                ASSERT(deps->size() > 0);
            
                for (const Dependent & dep : *deps)
                {
                    // check to see if it's already loaded
                    auto it = mAssets.find(dep.path);

                    // Only conduct load if asset hasn't already started loading
                    if (it == mAssets.end())
                    {
                        // insert null placeholder so we know it is already
                        // loading in case it is requested again before loading
                        // finishes.
                        mAssets[dep.path] = nullptr;

                        AssetLoader * pLdr = findLeastBusyAssetLoader();
                        pLdr->incQueueSize();
                        CmpString pathStr = mBlockMemory.stringAlloc(dep.path);
                        MessageQueueWriter msgw(HASH::request_asset__, kMessageFlag_None, kAssetMgrTaskId, kAssetMgrTaskId, to_cell(0), pathStr.blockCount() + 1, &pLdr->requestQueue());
                        msgw[0].cells[0].u = 0;
                        pathStr.writeMessage(msgw.accessor(), 1);
                    }
                    else if (it->second != nullptr)
                    {
                        // Asset is already loaded, set the dependent
                        pAsset->mpAssetType->setDependent(dep.nameHash,
                                                          pAsset->mpBuffer,
                                                          pAsset->mSize,
                                                          it->second->mpBuffer,
                                                          it->second->mSize);
                    }

                    if (it == mAssets.end() || it->second == nullptr)
                    {
                        // Asset is in the process of loading, started by some other entity.
                        // Record the requestor's info so we can send them asset_ready__
                        // when loading is complete.
                        mAssetsWaitingForDependent[dep.path].emplace_back(dep.nameHash, pAsset, msgr.taskId(), msgr.subTaskId(), msgr.nameHash());
                    }
                }

                if (pAsset->isLoaded())
                {
                    // We got lucky, all dependents were already loaded,
                    // send us an asset_ready__ back to ourselves so we
                    // can process this asset again immediately.
                    messages::AssetQW msgw(HASH::asset_ready__,
                                           kMessageFlag_None,
                                           kAssetMgrTaskId,
                                           kAssetMgrTaskId,
                                           msgr.taskId());
                    msgw.setSubTaskId(msgr.subTaskId());
                    msgw.setNameHash(msgr.nameHash());
                    msgw.setAsset(msgr.asset());
                }
            }
        }


        return MessageResult::Consumed;
    }
    case HASH::release_asset__:
    {
        messages::AssetR<T> msgr(msgAcc);
        Asset * pAsset = msgr.asset();
        ASSERT(pAsset && pAsset->refCount() > 0);
        if (pAsset->release())
        {
            mAssets.erase(pAsset->path());
            GDELETE(pAsset);
        }

        return MessageResult::Consumed;
    }
    default:
        PANIC("Unknown AssetMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;
}

AssetLoader * AssetMgr::findLeastBusyAssetLoader()
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
template MessageResult AssetMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);

} // namespace gaen
