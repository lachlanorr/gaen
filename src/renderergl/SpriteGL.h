//------------------------------------------------------------------------------
// SpriteGL.h - OpenGL wrapper for sprites
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

#ifndef GAEN_RENDERERGL_SPRITEGL_H
#define GAEN_RENDERERGL_SPRITEGL_H

#include <glm/mat4x3.hpp>

#include "core/mem.h"
#include "engine/Sprite.h"

#include "renderergl/RenderCollection.h"

namespace gaen
{

class RendererMesh;

class SpriteGL
{
public:
    typedef SpriteInstance InstanceT;
    
    SpriteGL(SpriteInstance * pSpriteInstance, RendererMesh * pRenderer)
      : mpSpriteInstance(pSpriteInstance)
      , mpRenderer(pRenderer)
      , mStatus(kRIS_Active)
      , vertArrayId(0)
      , vertBufferId(0)
      , primBufferId(0)
      , textureId(0)
      , textureUnit(0)
    {}

    void loadGpu();
    void unloadGpu();
    void render();

    void animate(u32 animHash, u32 animFrameIdx);

    void prepareMeshAttributes();

    u32 uid() const { return mpSpriteInstance->sprite().uid(); }
    f32 order() const { return mpSpriteInstance->zdepth(); }

    u32 shaderHash() const { return HASH::sprite; }

    void reportDestruction();
    
    const glm::mat4x3 & transform() { return mpSpriteInstance->mTransform; }
    void setTransform(const glm::mat4x3 & transform) { mpSpriteInstance->mTransform = transform; }

    RenderItemStatus status() { return mStatus; }
    void setStatus(RenderItemStatus status) { mStatus = status; }

private:
    UniquePtr<SpriteInstance> mpSpriteInstance;
    RendererMesh * mpRenderer;

    RenderItemStatus mStatus;

    u32 vertArrayId;
    u32 vertBufferId;
    u32 primBufferId;

    u32 textureId;
    u32 textureUnit;
};

typedef UniquePtr<SpriteGL> SpriteGLUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_SPRITEGL_H
