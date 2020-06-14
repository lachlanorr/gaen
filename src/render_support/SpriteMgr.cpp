//------------------------------------------------------------------------------
// SpriteMgr.cpp - Management of Sprite lifetimes, animations and collisions
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

#include "render_support/stdafx.h"

#include "assets/Gspr.h"

#include "engine/Handle.h"
#include "engine/Asset.h"
#include "engine/Entity.h"

#include "engine/messages/SpriteInstance.h"
#include "engine/messages/SpritePlayAnim.h"
#include "engine/messages/SpriteVelocity.h"
#include "engine/messages/SpriteBody.h"

#include "render_support/SpriteMgr.h"

namespace gaen
{

static const u32 kMaxMessages = 4096;

SpriteMgr::SpriteMgr()
{
    mTimePrev = mTimeCurr = now();
}


SpriteMgr::~SpriteMgr()
{
    // LORRTODO: Cleanup is causing crash on exit... need to redesign how we release assets
    //for (auto & spritePair : mSpriteMap)
    //{
    //    SpriteInstance::sprite_remove(kSpriteMgrTaskId, kRendererTaskId, spritePair.second->sprite().uid());
    //}
}

void SpriteMgr::update()
{
    mTimeCurr = now();
    f32 delta = (f32)(mTimeCurr - mTimePrev);
    mPhysics.update();

    for (auto & spritePair : mSpriteMap)
    {
        SpriteInstance * pSpriteInst = spritePair.second.get();
        /*
        if (pSpriteInst->mVelocity != vec2{0.0})
        {
            vec3 offset = vec3(pSpriteInst->mVelocity * delta, 0.0f);
            pSpriteInst->mTransform = to_mat4x3(translate(mat4(1.0f), offset) * mat4(pSpriteInst->mTransform));
            SpriteInstance::send_sprite_transform(kSpriteMgrTaskId, kRendererTaskId, pSpriteInst->sprite().uid(), pSpriteInst->mTransform);
            {
                messages::PropertyMat43BW msgw(HASH::transform, kMessageFlag_None, kSpriteMgrTaskId, pSpriteInst->sprite().mOwner, HASH::transform);
                msgw.setValue(pSpriteInst->mTransform);
                TaskMaster::task_master_for_active_thread().message(msgw.accessor());
            }
        }
        */
        if (pSpriteInst->mIsAnimating && pSpriteInst->advanceAnim(delta))
        {
            // update renderer with new frame
            SpriteInstance::sprite_anim(kSpriteMgrTaskId,
                                        kRendererTaskId,
                                        pSpriteInst->sprite().uid(),
                                        pSpriteInst->mAnimHash,
                                        pSpriteInst->mAnimFrameIdx);
        }
    }
    mTimePrev = mTimeCurr;
}

template <typename T>
MessageResult SpriteMgr::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::sprite_insert:
    {
        messages::SpriteInstanceR<T> msgr(msgAcc);

        SpriteInstance * pSpriteInst = msgr.spriteInstance();
        ASSERT(pSpriteInst);
        ASSERT(mSpriteMap.find(pSpriteInst->sprite().uid()) == mSpriteMap.end());
        mSpriteMap.emplace(pSpriteInst->sprite().uid(), pSpriteInst);
        mSpriteOwners[pSpriteInst->sprite().owner()].push_back(pSpriteInst->sprite().uid());

        // Send a copy to the renderer
        Sprite * pSpriteRenderer = GNEW(kMEM_Renderer, Sprite, pSpriteInst->sprite());
        SpriteInstance * pSpriteInstRenderer = GNEW(kMEM_Renderer, SpriteInstance, pSpriteRenderer, pSpriteInst->stageHash(), pSpriteInst->pass(), pSpriteInst->mTransform);
        SpriteInstance::sprite_insert(kSpriteMgrTaskId, kRendererTaskId, pSpriteInstRenderer);

        return MessageResult::Consumed;
    }
    case HASH::sprite_play_anim:
    {
        messages::SpritePlayAnimR<T> msgr(msgAcc);

        auto spritePair = mSpriteMap.find(msgr.uid());
        if (spritePair != mSpriteMap.end())
        {
            spritePair->second->playAnim(msgr.animHash(), msgr.duration(), msgr.loop(), msgr.doneMessage());
        }
        else
        {
            ERR("sprite_play_anim for unknown animation, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::sprite_set_velocity:
    {
        messages::SpriteVelocityR<T> msgr(msgAcc);
        auto spritePair = mSpriteMap.find(msgr.uid());
        if (spritePair != mSpriteMap.end())
        {
            if (spritePair->second->mHasBody)
            {
                mPhysics.setVelocity(msgr.uid(), msgr.velocity());
            }
            else
            {
                ERR("sprite_set_velocity for non rigid body sprite, uid: %u", msgr.uid());
            }
        }
        else
        {
            ERR("sprite_set_velocity for unknown sprite, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
    case HASH::sprite_init_body:
    {
        messages::SpriteBodyR<T> msgr(msgAcc);
        auto spritePair = mSpriteMap.find(msgr.uid());
        if (spritePair != mSpriteMap.end())
        {
            mPhysics.insert(*spritePair->second, msgr.mass(), msgr.group(), msgr.mask03(), msgr.mask47());
        }
        else
        {
            ERR("sprite_init_body for unknown sprite, uid: %u", msgr.uid());
        }
        return MessageResult::Consumed;
    }
	case HASH::remove_task__:
	{
        task_id taskIdToRemove = msg.payload.u;
        auto itL = mSpriteOwners.find(taskIdToRemove);
        // It's ok if we don't find it, it just means this task had no sprites
        if (itL != mSpriteOwners.end())
        {
            for (u32 uid : itL->second)
            {
                auto spritePair = mSpriteMap.find(uid);
                if (spritePair != mSpriteMap.end())
                {
                    // remove from physics simulation if necessary
                    if (spritePair->second->mHasBody)
                    {
                        mPhysics.remove(uid);
                    }
                    
                    // send sprite_remove to renderer who in turn will send it back to us once
                    SpriteInstance::sprite_remove(kSpriteMgrTaskId, kRendererTaskId, uid);
                }
                else
                {
                    ERR("remove_task for task_id: %u has non-existent sprite uid: %u", taskIdToRemove, uid);
                }
            }
            mSpriteOwners.erase(itL);
        }
        return MessageResult::Consumed;
	}
    case HASH::sprite_remove:
    {
        u32 uid = msg.payload.u;

        auto spritePair = mSpriteMap.find(uid);
        if (spritePair != mSpriteMap.end())
        {
            spritePair->second->destroySprite();
            mSpriteMap.erase(spritePair);
        }
        else
        {
            ERR("sprite_remove for unknown animation, uid: %u", uid);
        }
        return MessageResult::Consumed;
    }
    default:
        PANIC("Unknown SpriteMgr message: %d", msg.msgId);
    }
    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult SpriteMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult SpriteMgr::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

namespace system_api
{

i32 sprite_create(AssetHandleP pAssetHandle, i32 stageHash, i32 passHash, const mat43 & transform, Entity * pCaller)
{
    ASSERT(pAssetHandle->typeHash() == HASH::asset);
    const Asset * pAsset = pAssetHandle->data<Asset>();

    Sprite * pSprite = GNEW(kMEM_Engine, Sprite, pCaller->task().id(), pAsset);
    SpriteInstance * pSpriteInst = GNEW(kMEM_Engine, SpriteInstance, pSprite, stageHash, pass_from_hash(passHash), transform);

    SpriteInstance::sprite_insert(pCaller->task().id(), kSpriteMgrTaskId, pSpriteInst);

    return pSprite->uid();
}

void sprite_play_anim(i32 spriteUid, i32 animHash, f32 duration, bool loop, i32 doneMessage, Entity * pCaller)
{
    messages::SpritePlayAnimBW msgw(HASH::sprite_play_anim, kMessageFlag_None, pCaller->task().id(), kSpriteMgrTaskId, spriteUid);
    msgw.setAnimHash(animHash);
    msgw.setDuration(duration);
    msgw.setLoop(loop);
    msgw.setDoneMessage(doneMessage);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void sprite_set_velocity(i32 spriteUid, const vec2 & velocity, Entity * pCaller)
{
    messages::SpriteVelocityBW msgw(HASH::sprite_set_velocity, kMessageFlag_None, pCaller->task().id(), kSpriteMgrTaskId, spriteUid);
    msgw.setVelocity(velocity);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void sprite_init_body(i32 spriteUid, f32 mass, i32 group, ivec4 mask03, ivec4 mask47, Entity * pCaller)
{
    messages::SpriteBodyBW msgw(HASH::sprite_init_body, kMessageFlag_None, pCaller->task().id(), kSpriteMgrTaskId, spriteUid);
    msgw.setMass(mass);
    msgw.setGroup(group);
    msgw.setMask03(mask03);
    msgw.setMask47(mask47);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void sprite_stage_show(i32 stageHash, Entity * pCaller)
{
    MessageQueueWriter msgw(HASH::sprite_stage_show, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

void sprite_stage_hide(i32 stageHash, Entity * pCaller)
{
    MessageQueueWriter msgw(HASH::sprite_stage_hide, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

void sprite_stage_remove(i32 stageHash, Entity * pCaller)
{
    MessageQueueWriter msgw(HASH::sprite_stage_remove, kMessageFlag_None, pCaller->task().id(), kRendererTaskId, to_cell(stageHash), 0);
}

} // namespace system_api

} // namespace gaen
