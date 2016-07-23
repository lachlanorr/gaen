//------------------------------------------------------------------------------
// Sprite.h - 2d graphics object
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

#ifndef GAEN_ENGINE_SPRITE_H
#define GAEN_ENGINE_SPRITE_H

#include <glm/mat4x3.hpp>

#include "engine/task.h"

namespace gaen
{

class Asset;
class Gspr;
class Gatl;
struct GlyphVert;
struct GlyphTri;

typedef u32 sprite_id;

// Immutable properties of a sprite
class Sprite
{
    friend struct SpriteInstance;
    friend class SpriteMgr;
public:
    Sprite(task_id owner, Asset* pGsprAsset);
    ~Sprite();

    sprite_id id() const { return mId; }

    const GlyphVert * verts();
    u64 vertsSize();

    const GlyphTri * tris();
    u64 trisSize();

private:
    const void * triOffset(u32 idx);

    sprite_id mId;

    task_id mOwner;
    Asset * mpGsprAsset;

    // pointers int mpGsprAsset, no need to clean these up
    const Gspr * mpGspr;
    const Gatl * mpGatl;
};

struct AnimInfo;
// Mutable properties of a sprite
struct SpriteInstance
{
    friend class SpriteMgr;
public:
    SpriteInstance(Sprite * pSprite, const glm::mat4x3 & transform);

    const Sprite & sprite() { return *mpSprite; }

    const void * currentFrameElemsOffset() { return mpCurrentFrameElemsOffset; }
    void setAnim(u32 animHash);
    void setFrame(u32 frameIdx);

    const AnimInfo & SpriteInstance::currentAnim();

    glm::mat4x3 transform;
private:
    Sprite * mpSprite;
    const AnimInfo * mpAnimInfo;
    const void * mpCurrentFrameElemsOffset;
    u32 mAnimHash;
    u32 mAnimFrame;
};


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITE_H
