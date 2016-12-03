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

#include <glm/gtx/transform.hpp>

#include "render_support/Sprite.h"

#include "renderergl/RendererMesh.h"
#include "renderergl/SpriteGL.h"
#include "renderergl/SpriteStage.h"

namespace gaen
{
SpriteStage::SpriteStage(u32 stageHash, RendererMesh * pRenderer)
  : Stage(stageHash,
          pRenderer,
          Camera(kRendererTaskId,
                 stageHash,
                 glm::ortho(pRenderer->screenWidth() * -0.5f,
                            pRenderer->screenWidth() * 0.5f,
                            pRenderer->screenHeight() * -0.5f,
                            pRenderer->screenHeight() * 0.5f),
                 glm::mat4(1.0)))
{}

bool SpriteStage::animateItem(u32 uid, u32 animHash, u32 animFrameIdx)
{
    auto it = mItems.find(uid);
    if (it != mItems.end())
    {
        it->animate(animHash, animFrameIdx);
        return true;
    }
    return false;
}

} // namespace gaen
