//------------------------------------------------------------------------------
// SpriteGL.h - OpenGL wrapper for sprites
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include "gaen/core/mem.h"
#include "gaen/math/mat43.h"
#include "gaen/render_support/Sprite.h"

#include "gaen/renderergl/RenderCollection.h"

namespace gaen
{

class RendererMesh;

class SpriteGL
{
public:
    typedef SpriteInstance InstanceType;

    SpriteGL(SpriteInstance * pSpriteInstance, RendererMesh * pRenderer)
      : mpSpriteInstance(pSpriteInstance)
      , mpRenderer(pRenderer)
      , mStatus(kRIS_Active)
      , mVertArrayId(0)
      , mVertBufferId(0)
      , mPrimBufferId(0)
      , mTextureId(0)
      , mTextureUnit(0)
    {}

    void loadGpu();
    void unloadGpu();
    void render();

    void animate(u32 animHash, u32 animFrameIdx);

    void prepareMeshAttributes();

    u32 uid() const { return mpSpriteInstance->sprite().uid(); }
    u32 renderFlags() const { return mpSpriteInstance->renderFlags(); }
    f32 order() const { return mpSpriteInstance->zdepth(); }

    u32 shaderHash() const { return HASH::sprite; }

    void reportDestruction();

    const mat43 & transform() { return mpSpriteInstance->mTransform; }
    void setTransform(const mat43 & transform) { mpSpriteInstance->mTransform = transform; }

    RenderItemStatus status() { return mStatus; }
    void setStatus(RenderItemStatus status) { mStatus = status; }

    bool isVisible() const { return mpSpriteInstance->isVisible(); }
    void hide() { mpSpriteInstance->hide(); }
    void show() { mpSpriteInstance->show(); }

    bool hasNormal() { return false; }

    u32 frameOffset() const { return 0; }
    void setFrameOffset(u32 offset) { }

private:
    UniquePtr<SpriteInstance> mpSpriteInstance;
    RendererMesh * mpRenderer;

    RenderItemStatus mStatus;

    u32 mVertArrayId;
    u32 mVertBufferId;
    u32 mPrimBufferId;

    u32 mTextureId;
    u32 mTextureUnit;
};

typedef UniquePtr<SpriteGL> SpriteGLUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_SPRITEGL_H
