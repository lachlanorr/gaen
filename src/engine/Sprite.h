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
class Gimg;
struct GlyphVert;
struct GlyphTri;

typedef u32 sprite_id;

// Immutable properties of a sprite
class Sprite
{
    friend class SpriteInstance;
    friend class SpriteMgr;
public:
    Sprite(task_id owner, const Asset* pGsprAsset);
    Sprite(const Sprite& rhs);
    ~Sprite();

    sprite_id uid() const { return mUid; }
    task_id owner() const { return mOwner; }

    const GlyphVert * verts() const;
    u64 vertsSize() const;

    const GlyphTri * tris() const;
    u64 trisSize() const;

    const Gimg & image() const;

private:
    // Delete these to make sure w construct through the asset->addref path
    Sprite(Sprite&&)                  = delete;
    Sprite & operator=(const Sprite&) = delete;
    Sprite & operator=(Sprite&&)      = delete;
    
    const void * triOffset(u32 idx) const;

    sprite_id mUid;

    task_id mOwner;
    const Asset * mpGsprAsset;

    // pointers int mpGsprAsset, no need to clean these up
    const Gspr * mpGspr;
    const Gatl * mpGatl;
};

struct AnimInfo;
// Mutable properties of a sprite
class SpriteInstance
{
    friend class SpriteMgr;
public:
    SpriteInstance(Sprite * pSprite, const glm::mat4x3 & transform);

    const Sprite & sprite() { return *mpSprite; }

    void desroySprite();

    const void * currentFrameElemsOffset() { return mpCurrentFrameElemsOffset; }
    bool animate(u32 animHash, u32 animFrameIdx);

    const AnimInfo & SpriteInstance::currentAnim();

    static void send_sprite_insert(task_id source, task_id target, SpriteInstance * pSpriteInst);
    static void send_sprite_anim(task_id source, task_id target, u32 uid, u32 animHash, u32 animFrameIdx);
    static void send_sprite_transform(task_id source, task_id target, u32 uid, const glm::mat4x3 & transform);
    static void send_sprite_destroy(task_id source, task_id target, u32 uid);

    glm::mat4x3 mTransform;
    glm::vec2 mVelocity;

private:
    // Delete these to make sure we construct through the asset->addref path
    SpriteInstance(const SpriteInstance&)             = delete;
    SpriteInstance(SpriteInstance&&)                  = delete;
    SpriteInstance & operator=(const SpriteInstance&) = delete;
    SpriteInstance & operator=(SpriteInstance&&)      = delete;
    
    void playAnim(u32 animHash, f32 duration);
    bool advanceAnim(f32 deltaSecs);

    Sprite * mpSprite;
    
    const AnimInfo * mpAnimInfo;
    const void * mpCurrentFrameElemsOffset;
    u32 mAnimHash;
    u32 mAnimFrameIdx;
    f32 mDurationPerFrame;
    f32 mDurationSinceFrameChange;

    bool mIsAnimating;
};

typedef UniquePtr<SpriteInstance> SpriteInstanceUP;

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITE_H
