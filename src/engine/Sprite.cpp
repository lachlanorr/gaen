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
#include "engine/AssetMgr.h"
#include "engine/messages/Transform.h"

#include "engine/Sprite.h"

namespace gaen
{

Sprite::Sprite(task_id owner, Asset * pGsprAsset, const glm::mat4x3 & transform)
  : mOwner(owner)
  , mpGsprAsset(pGsprAsset)
  , mTransform(transform)
{
    IS_VALID(Gspr, pGsprAsset);
    AssetMgr::addref_asset(0, mpGsprAsset);
}

Sprite::~Sprite()
{
    AssetMgr::release_asset(0, mpGsprAsset);
}

void Sprite::setTransform(const glm::mat4x3 & transform)
{
    mTransform = transform;
    
    // Notify owning task that its transform has been updated
    messages::TransformQW msgw(HASH::transform,
                               kMessageFlag_None,
                               kSpriteMgrTaskId,
                               mOwner,
                               false);
    msgw.setTransform(transform);
}

} // namespace gaen
