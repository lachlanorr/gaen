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

#include "core/mem.h"
#include "engine/Sprite.h"

namespace gaen
{

class RendererMesh;

enum SpriteGLStatus
{
    kSGLS_Active,
    kSGLS_Destroyed
};

class SpriteGL
{
public:
    SpriteGL(SpriteInstance * pSpriteInstance, RendererMesh * pRenderer)
      : mpSpriteInstance(pSpriteInstance)
      , mpRenderer(pRenderer)
      , status(kSGLS_Active)
      , vertArrayId(0)
      , vertBufferId(0)
      , primBufferId(0)
      , textureId(0)
      , textureUnit(0)
    {}

    void loadGpu();
    void render();

    void prepareMeshAttributes();
    
    UniquePtr<SpriteInstance> mpSpriteInstance;
    RendererMesh * mpRenderer;

    SpriteGLStatus status;

    u32 vertArrayId;
    u32 vertBufferId;
    u32 primBufferId;

    u32 textureId;
    u32 textureUnit;
};

typedef UniquePtr<SpriteGL> SpriteGLUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_SPRITEGL_H
