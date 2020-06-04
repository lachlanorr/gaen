//------------------------------------------------------------------------------
// Sprite.cpp - 2d graphics object
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
#include "assets/Gatl.h"

#include "math/common.h"

#include "engine/AssetMgr.h"
#include "engine/messages/NotifyWatcherMat43.h"
#include "engine/messages/RegisterWatcher.h"
#include "engine/messages/SpriteInstance.h"
#include "engine/messages/SpriteAnim.h"
#include "engine/messages/UidTransform.h"

#include "render_support/Sprite.h"

namespace gaen
{

Sprite::Sprite(task_id owner, const Asset * pGsprAsset)
  : RenderObject(owner)
  , mpGsprAsset(pGsprAsset)
{
    VALIDATE_ASSET(Gspr, pGsprAsset);
    AssetMgr::addref_asset(0, mpGsprAsset);
    mpGspr = Gspr::instance(mpGsprAsset->buffer(), mpGsprAsset->size());
    mpGatl = mpGspr->atlas();
    ASSERT(mpGatl);
}

Sprite::Sprite(const Sprite& rhs)
  : RenderObject(rhs.owner(), rhs.uid())
  , mpGsprAsset(rhs.mpGsprAsset)
{
    mpGspr = Gspr::instance(mpGsprAsset->buffer(), mpGsprAsset->size());
    mpGatl = mpGspr->atlas();
}

Sprite::~Sprite()
{
    // LORRTODO: Cleanup is causing crash on exit... need to redesign how we release assets
    //AssetMgr::release_asset(0, mpGsprAsset);
}

const GlyphVert * Sprite::verts() const
{
    return mpGatl->verts();
}

u64 Sprite::vertsSize() const
{
    return mpGatl->vertsSize();
}

const GlyphTri * Sprite::tris() const
{
    return mpGatl->tris();
}

u64 Sprite::trisSize() const
{
    return mpGatl->trisSize();
}

const void * Sprite::triOffset(u32 idx) const
{
    idx = idx % mpGatl->glyphCount();
    return (void*)(sizeof(GlyphTri) * (&mpGatl->tris()[idx] - mpGatl->tris()));
}

vec3 Sprite::halfExtents() const
{
    const AnimInfoSpr * pAnimInfo = mpGspr->getAnim(mpGspr->defaultAnimHash());
    uintptr_t frameElemsOffset = (uintptr_t)mpGspr->getFrameElemsOffset(pAnimInfo, 0);
    u16 triIdx = (u16)(frameElemsOffset / sizeof(GlyphTri));
    const GlyphTri & tri = mpGatl->tri(triIdx);
    vec3 rmin(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), 0.0f);
    vec3 rmax(std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest(), 0.0f);

    const GlyphVert & vert0 = mpGatl->vert(tri.p0);
    rmin.x = min(rmin.x, vert0.position.x);
    rmin.y = min(rmin.y, vert0.position.y);
    rmax.x = max(rmax.x, vert0.position.x);
    rmax.y = max(rmax.y, vert0.position.y);

    const GlyphVert & vert1 = mpGatl->vert(tri.p1);
    rmin.x = min(rmin.x, vert1.position.x);
    rmin.y = min(rmin.y, vert1.position.y);
    rmax.x = max(rmax.x, vert1.position.x);
    rmax.y = max(rmax.y, vert1.position.y);

    const GlyphVert & vert2 = mpGatl->vert(tri.p2);
    rmin.x = min(rmin.x, vert2.position.x);
    rmin.y = min(rmin.y, vert2.position.y);
    rmax.x = max(rmax.x, vert2.position.x);
    rmax.y = max(rmax.y, vert2.position.y);

    vec3 ext = rmax - rmin;

    return ext / 2.0f;
}

const Gimg & Sprite::gimg() const
{
    return *mpGatl->gimg();
}


// SpriteInstance methods

SpriteInstance::SpriteInstance(Sprite * pSprite, u32 stageHash, RenderPass pass, const mat43 & transform)
  : mpSprite(pSprite)
  , mStageHash(stageHash)
  , mPass(pass)
  , mHasBody(false)
  , mTransform(transform)
  , mIsAnimating(false)
  , mIsLooping(false)
  , mDoneMessage(0)
{
    mDurationPerFrame = 0.0f;
    mDurationSinceFrameChange = 0.0f;
    mAnimHash = 0;
    animate(mpSprite->mpGspr->defaultAnimHash(), 0);
}

void SpriteInstance::registerTransformWatcher(task_id taskId)
{
    messages::RegisterWatcherBW msgw(HASH::register_watcher, kMessageFlag_None, taskId, mpSprite->owner(), HASH::sprite_transform);
    msgw.setProperty(HASH::transform);
    msgw.setUid(mpSprite->uid());
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void SpriteInstance::destroySprite()
{
    if (mpSprite)
    {
        GDELETE(mpSprite);
        mpSprite = nullptr;
    }
}

const AnimInfoSpr & SpriteInstance::currentAnim()
{
    ASSERT(mpAnimInfo);
    return *mpAnimInfo;
}

void SpriteInstance::playAnim(u32 animHash, f32 duration, bool loop, u32 doneMessage)
{
    ASSERT(duration > 0.0f);
    mIsAnimating = true;
    mIsLooping = loop;
    mDoneMessage = doneMessage;
    animate(animHash, 0);
    mDurationPerFrame = duration / mpAnimInfo->frameCount;
    mDurationSinceFrameChange = 0.0f;

    // Send the renderer the message immediately so there isn't a
    // delay in frame switch until next frame times out
    SpriteInstance::sprite_anim(kSpriteMgrTaskId,
                                kRendererTaskId,
                                mpSprite->uid(),
                                mAnimHash,
                                mAnimFrameIdx);
}

bool SpriteInstance::advanceAnim(f32 delta)
{
    mDurationSinceFrameChange += delta;

    if (mDurationSinceFrameChange > mDurationPerFrame)
    {
        u32 framesToAdvance = (u32)(mDurationSinceFrameChange / mDurationPerFrame);
        u32 newFrameIdx = (mAnimFrameIdx + framesToAdvance) % mpAnimInfo->frameCount;
        mDurationSinceFrameChange = fmod(delta, mDurationPerFrame);

        if (mAnimFrameIdx + framesToAdvance >= mpAnimInfo->frameCount)
        {
            if (!mIsLooping)
            {
                mIsAnimating = false;
            }
            if (mDoneMessage != 0)
            {
                MessageQueueWriter msgw(mDoneMessage, kMessageFlag_None, kSpriteMgrTaskId, mpSprite->owner(), to_cell(0), 0);
            }
        }

        return animate(mAnimHash, newFrameIdx);
    }
    return false;
}

bool SpriteInstance::animate(u32 animHash, u32 animFrameIdx)
{
    if (animHash != mAnimHash)
    {
        mAnimHash = animHash;
        mpAnimInfo = mpSprite->mpGspr->getAnim(animHash);
        mAnimFrameIdx = 0;
        mpCurrentFrameElemsOffset = mpSprite->mpGspr->getFrameElemsOffset(mpAnimInfo, mAnimFrameIdx);
        return true;
    }
    else if (animFrameIdx != mAnimFrameIdx)
    {
        mAnimFrameIdx = animFrameIdx;
        mpCurrentFrameElemsOffset = mpSprite->mpGspr->getFrameElemsOffset(mpAnimInfo, mAnimFrameIdx);
        return true;
    }
    return false;
}

void SpriteInstance::sprite_insert(task_id source, task_id target, SpriteInstance * pSpriteInst)
{
    messages::SpriteInstanceBW msgw(HASH::sprite_insert, kMessageFlag_None, source, target);
    msgw.setSpriteInstance(pSpriteInst);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void SpriteInstance::sprite_anim(task_id source, task_id target, u32 uid, u32 animHash, u32 animFrameIdx)
{
    messages::SpriteAnimBW msgw(HASH::sprite_anim, kMessageFlag_None, source, target, uid);
    msgw.setAnimHash(animHash);
    msgw.setAnimFrameIdx(animFrameIdx);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void SpriteInstance::sprite_transform(task_id source, task_id target, u32 uid, const mat43 & transform)
{
    messages::NotifyWatcherMat43BW msgw(HASH::sprite_transform, kMessageFlag_None, source, target, uid);
    msgw.setProperty(HASH::transform);
    msgw.setValueType(HASH::mat43);
    msgw.setValue(transform);
    TaskMaster::task_master_for_active_thread().message(msgw.accessor());
}

void SpriteInstance::sprite_remove(task_id source, task_id target, u32 uid)
{
    MessageQueueWriter msgw(HASH::sprite_remove, kMessageFlag_None, source, target, to_cell(uid), 0);
}

} // namespace gaen
