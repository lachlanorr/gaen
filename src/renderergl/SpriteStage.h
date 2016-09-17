//------------------------------------------------------------------------------
// SpriteStage.h - Grouping of sprites in a plane, including a cara layer
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

#ifndef GAEN_RENDERERGL_SPRITESTAGE_H
#define GAEN_RENDERERGL_SPRITESTAGE_H

#include "core/base_defines.h"

#include "renderergl/SpriteGL.h"
#include "renderergl/FrameGL.h"

namespace gaen
{

class RendererMesh;
class SpriteInstance;

class SpriteStage
{
public:
    SpriteStage(RendererMesh * pRenderer)
      : mpRenderer(pRenderer)
      , mIsShown(false)
    {}

    u32 spritesSize() { return mSprites.size(); }
    u32 framesSize() { return mFrames.size(); }

    void insertSprite(SpriteInstance * pSpriteInst);
    bool animateSprite(u32 uid, u32 animHash, u32 animFrameIdx);
    bool transformSprite(u32 uid, const glm::mat4x3 & transform);
    bool destroySprite(u32 uid);

    bool isShown() { return mIsShown; }
    void show() { mIsShown = true; }
    void hide() { mIsShown = false; }
    
    RenderCollection<SpriteGL>::Iter beginSprites()       { return mSprites.begin(); }
    RenderCollection<SpriteGL>::Iter endSprites()         { return mSprites.end(); }
    void eraseSprite(RenderCollection<SpriteGL>::Iter it) { mSprites.erase(it); }

    RenderCollection<FrameGL>::Iter beginFrames()       { return mFrames.begin(); }
    RenderCollection<FrameGL>::Iter endFrames()         { return mFrames.end(); }
    void eraseFrame(RenderCollection<FrameGL>::Iter it) { mFrames.erase(it); }

private:
    RendererMesh * mpRenderer;
    bool mIsShown;

    RenderCollection<SpriteGL> mSprites;
    RenderCollection<FrameGL> mFrames;
    
}; // class SpriteStage

typedef UniquePtr<SpriteStage> SpriteStageUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_SPRITESTAGE_H
