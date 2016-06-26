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
#include "engine/hashes.h"
#include "engine/messages/Handle.h"
#include "engine/MessageQueue.h"
#include "engine/HandleMgr.h"

namespace gaen
{

template <typename T>
MessageResult HandleMgr::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::request_asset:
    {
        PANIC_IF(msg.blockCount < 2, "Too few bytes for request_asset message");

        u32 requestorTaskId = msg.source;
        u32 nameHash = msgAcc[0].cells[0].u;
        
        const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[1]);

        PANIC_IF(pBlockData->type != kBKTY_String, "No path string sent in request_asset message");
        
        u32 requiredBlockCount = pBlockData->blockCount;

        // -1 below is for the block 0 in the message required to send the requestorTaskId
        PANIC_IF(msg.blockCount - 1 < requiredBlockCount, "Too few bytes for request_asset path string");

        Address addr = mBlockMemory.allocCopy(pBlockData);
        CmpString pathCmpString = mBlockMemory.string(addr);
        const char * pathStr = pathCmpString.c_str();

        // LORRTODO: Really load assets
        
        messages::HandleQW msgw(HASH::asset_ready__, kMessageFlag_None, kHandleMgrTaskId, msg.source, requestorTaskId);
        msgw.setNameHash(nameHash);
        msgw.setHandle(nullptr);

        break;
    }
    default:
        PANIC("Unknown HandleMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;

	/*
	switch(msg.msgId)
    {

    case HASH::renderer_insert_model_instance:
    {
        messages::InsertModelInstanceR<T> msgr(msgAcc);
        mpModelMgr->insertModelInstance(msgAcc.message().source,
                                        msgr.uid(),
                                        msgr.model(),
                                        msgr.transform(),
                                        msgr.isAssetManaged());
        break;
    }
    case HASH::renderer_transform_model_instance:
    {
        messages::TransformIdR<T> msgr(msgAcc);
        ModelInstance * pModelInst = mpModelMgr->findModelInstance(msgr.id());
        ASSERT(pModelInst);
        pModelInst->transform = msgr.transform();
        break;
    }
    case HASH::renderer_remove_model_instance:
    {
        mpModelMgr->removeModelInstance(msg.source, msg.payload.u);
        break;
    }
    case HASH::renderer_insert_light_directional:
    {
        messages::InsertLightDirectionalR<T> msgr(msgAcc);
        glm::vec3 normDir = glm::normalize(msgr.direction());
        glm::vec3 relDir = -normDir; // flip direction of vector relative to objects
        mDirectionalLights.emplace_back(msgAcc.message().source,
                                        msgr.uid(),
                                        relDir,
                                        msgr.color());
        break;
    }
    case HASH::renderer_update_light_directional:
    {
        messages::InsertLightDirectionalR<T> msgr(msgAcc);
        mDirectionalLights.emplace_back(msgAcc.message().source,
                                        msgr.uid(),
                                        msgr.direction(),
                                        msgr.color());
        break;
    }
    case HASH::renderer_move_camera:
    {
        messages::MoveCameraR<T> msgr(msgAcc);
        mRaycastCamera.move(msgr.position(), msgr.direction());
        break;
    }
    default:
        PANIC("Unknown renderer message: %d", msg.msgId);
    }
*/
    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult HandleMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);

} // namespace gaen
