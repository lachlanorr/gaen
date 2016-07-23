//------------------------------------------------------------------------------
// Sprite.cpp - 2d graphics object
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

#include "assets/Gspr.h"
#include "assets/Gatl.h"
#include "engine/AssetMgr.h"
#include "engine/messages/Transform.h"

#include "engine/Sprite.h"

namespace gaen
{

static std::atomic<sprite_id> sNextSpriteId(0);

SpriteInstance::SpriteInstance(Sprite * pSprite, const glm::mat4x3 & transform)
  : mpSprite(pSprite)
  , transform(transform)
{
    mAnimHash = mpSprite->mpGspr->defaultAnimHash();
    mAnimFrame = 0;
}

const AnimInfo & SpriteInstance::currentAnim()
{
    ASSERT(mpAnimInfo);
    return *mpAnimInfo;
}

void SpriteInstance::setAnim(u32 animHash)
{
    mpAnimInfo = mpSprite->mpGspr->getAnim(animHash);
}

void SpriteInstance::setFrame(u32 frameIdx)
{
    mpCurrentFrameElemsOffset = mpSprite->mpGspr->getFrameElemsOffset(mpAnimInfo, frameIdx);
}

Sprite::Sprite(task_id owner, Asset * pGsprAsset)
  : mOwner(owner)
  , mpGsprAsset(pGsprAsset)
{
    IS_VALID(Gspr, pGsprAsset);
    AssetMgr::addref_asset(0, mpGsprAsset);
    mpGspr = Gspr::instance(mpGsprAsset->buffer(), mpGspr->size());
    mpGatl = mpGspr->atlas();
    ASSERT(mpGatl);

    mId = sNextSpriteId.fetch_add(1,std::memory_order_relaxed);
}

Sprite::~Sprite()
{
    AssetMgr::release_asset(0, mpGsprAsset);
}

const GlyphVert * Sprite::verts()
{
    return mpGatl->verts();
}

u64 Sprite::vertsSize()
{
    return mpGatl->vertsSize();
}

const GlyphTri * Sprite::tris()
{
    return mpGatl->tris();
}

u64 Sprite::trisSize()
{
    return mpGatl->trisSize();
}

const void * Sprite::triOffset(u32 idx)
{
    idx = idx % mpGatl->glyphCount();
    return (void*)(sizeof(GlyphTri) * (&mpGatl->tris()[idx] - mpGatl->tris()));
}


} // namespace gaen
