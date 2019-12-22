//------------------------------------------------------------------------------
// ModelGL.h - OpenGL wrapper for models
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

#ifndef GAEN_RENDERERGL_MODELGL_H
#define GAEN_RENDERERGL_MODELGL_H

#include "core/mem.h"
#include "math/mat43.h"
#include "math/vec2.h"
#include "assets/Gmdl.h"
#include "render_support/Model.h"

#include "renderergl/RenderCollection.h"

namespace gaen
{

class RendererMesh;

class ModelGL
{
public:
    typedef ModelInstance InstanceType;
    
    ModelGL(ModelInstance * pModelInstance, RendererMesh * pRenderer)
      : mpModelInstance(pModelInstance)
      , mpRenderer(pRenderer)
      , mStatus(kRIS_Active)
      , mVertArrayId(0)
      , mVertBufferId(0)
      , mPrimBufferId(0)
      , mTextureId_diffuse(0)
      , mTextureId_animations(0)
      , mFrameOffset(0)
    {}

    void loadGpu();
    void unloadGpu();
    void render();

    void prepareMeshAttributes();

    u32 uid() const { return mpModelInstance->model().uid(); }
    f32 order() const { return 0; } // LORRTODO: Decide what to do with this ordering for models, only make sense for sprites

    u32 shaderHash() const { return mpModelInstance->model().gmdl().mat() ? mpModelInstance->model().gmdl().mat()->shaderHash() : HASH::faceted; }

    void reportDestruction();
    
    const mat43 & transform() const { return mpModelInstance->mTransform; }
    void setTransform(const mat43 & transform) { mpModelInstance->mTransform = transform; }

    RenderItemStatus status() const { return mStatus; }
    void setStatus(RenderItemStatus status) { mStatus = status; }

    bool hasNormal() const { return true; }

    u32 frameOffset() const { return mFrameOffset; }
    void setFrameOffset(u32 offset) { mFrameOffset = offset; }

private:
    UniquePtr<ModelInstance> mpModelInstance;
    RendererMesh * mpRenderer;

    RenderItemStatus mStatus;

    u32 mVertArrayId;
    u32 mVertBufferId;
    u32 mPrimBufferId;

    u32 mTextureId_diffuse;
    u32 mTextureId_animations;

    u32 mFrameOffset;
};

typedef UniquePtr<ModelGL> ModelGLUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_MODELGL_H
