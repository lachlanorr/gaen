//------------------------------------------------------------------------------
// SpriteStage.cpp - Grouping of sprites in a plane, including a cara layer
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

#include "engine/Sprite.h"

#include "renderergl/RendererMesh.h"
#include "renderergl/SpriteGL.h"
#include "renderergl/SpriteStage.h"

namespace gaen
{

void SpriteStage::insertSprite(SpriteInstance * pSpriteInst)
{
    ASSERT(pSpriteInst);
    ASSERT(mSprites.find(pSpriteInst->sprite().uid()) == mSprites.end());

    SpriteGLUP spriteGL(GNEW(kMEM_Renderer, SpriteGL, pSpriteInst, mpRenderer));

    spriteGL->loadGpu();
    
    mOrderedSprites.emplace(pSpriteInst->zdepth(), spriteGL.get());
    mSprites.emplace(pSpriteInst->sprite().uid(), std::move(spriteGL));
    ASSERT(mOrderedSprites.size() == mSprites.size());
}

bool SpriteStage::animateSprite(u32 uid, u32 animHash, u32 animFrameIdx)
{
    auto it = mSprites.find(uid);
    if (it != mSprites.end())
    {
        it->second->mpSpriteInstance->animate(animHash, animFrameIdx);
        return true;
    }
    return false;
}

bool SpriteStage::transformSprite(u32 uid, const glm::mat4x3 & transform)
{
    auto it = mSprites.find(uid);
    if (it != mSprites.end())
    {
        SpriteGL * pOrderedSprite = nullptr;
        // if the z value has changed, we need to reorganize in the ordered map
        if (it->second->mpSpriteInstance->zdepth() != transform[3][2])
        {
            // remove from ordered list
            auto range = mOrderedSprites.equal_range(it->second->mpSpriteInstance->zdepth());
            for (auto itOrd = range.first; itOrd != range.second; ++itOrd)
            {
                if (itOrd->second->mpSpriteInstance->sprite().uid() == uid)
                {
                    pOrderedSprite = it->second.get();
                    mOrderedSprites.erase(itOrd);
                    break;
                }
            }
            ASSERT(pOrderedSprite);
        }
        it->second->mpSpriteInstance->mTransform = transform;
        if (pOrderedSprite)
        {
            // zdepth changed, reinsert into ordered list
            mOrderedSprites.emplace(pOrderedSprite->mpSpriteInstance->zdepth(), pOrderedSprite);
        }

        return true;
    }
    return false;
}

bool SpriteStage::destroySprite(u32 uid)
{
    auto it = mSprites.find(uid);
    if (it != mSprites.end())
    {
        // Mark destroyed, it will get pulled from maps during next render pass.
        it->second->status = kSGLS_Destroyed;

        SpriteInstance::send_sprite_destroy(kRendererTaskId, kSpriteMgrTaskId, uid);

        return true;
    }
    return false;
}

void SpriteStage::erase(Iter it)
{
    mSprites.erase(it->mpSpriteInstance->sprite().uid());
    mOrderedSprites.erase(it.mOsmIt);
    ASSERT(mOrderedSprites.size() == mSprites.size());
}

} // namespace gaen
