//------------------------------------------------------------------------------
// Sprite.h - 2d graphics object
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

#ifndef GAEN_RENDER_SUPPORT_SPRITE_H
#define GAEN_RENDER_SUPPORT_SPRITE_H

#include "math/mat43.h"
#include "math/vec3.h"
#include "math/matrices.h"
#include "engine/task.h"
#include "render_support/render_objects.h"

namespace gaen
{

class Asset;
class Gspr;
class Gatl;
class Gimg;
struct GlyphVert;
struct GlyphTri;

// Immutable properties of a sprite
class Sprite : public RenderObject
{
    friend class SpriteInstance;
    friend class SpriteMgr;
public:
    Sprite(task_id owner, const Asset* pGsprAsset);
    Sprite(const Sprite& rhs);
    ~Sprite();

    const GlyphVert * verts() const;
    u64 vertsSize() const;

    const GlyphTri * tris() const;
    u64 trisSize() const;

    vec3 halfExtents() const;

    const Gimg & gimg() const;

private:
    // Delete these to make sure w construct through the asset->addref path
    Sprite(Sprite&&)                  = delete;
    Sprite & operator=(const Sprite&) = delete;
    Sprite & operator=(Sprite&&)      = delete;
    
    const void * triOffset(u32 idx) const;

    const Asset * mpGsprAsset;

    // pointers into mpGsprAsset, no need to clean up
    const Gspr * mpGspr;
    const Gatl * mpGatl;
};

struct AnimInfo;
class SpriteBody;
// Mutable properties of a sprite
class SpriteInstance
{
    friend class SpriteMgr;
    friend class SpriteBody;
    friend class SpriteMotionState;
    friend class SpritePhysics;
public:
    SpriteInstance(Sprite * pSprite, u32 stageHash, RenderPass pass, const mat43 & transform);

    const Sprite & sprite() { return *mpSprite; }
    u32 stageHash() { return mStageHash; }
    RenderPass pass() { return mPass; }

    ruid uid() const { return mpSprite->uid(); }

    void registerTransformListener(task_id taskId);

    void destroySprite();

    const void * currentFrameElemsOffset() { return mpCurrentFrameElemsOffset; }
    bool animate(u32 animHash, u32 animFrameIdx);

    const AnimInfo & SpriteInstance::currentAnim();

    static void sprite_insert(task_id source, task_id target, SpriteInstance * pSpriteInst);
    static void sprite_anim(task_id source, task_id target, u32 uid, u32 animHash, u32 animFrameIdx);
    static void sprite_transform(task_id source, task_id target, u32 uid, const mat43 & transform);
    static void sprite_remove(task_id source, task_id target, u32 uid);

    vec3 position() { return gaen::position(mTransform); }
    f32 zdepth() { return gaen::position(mTransform).z; }

    mat43 mTransform;
private:
    // Delete these to make sure we construct through the asset->addref path
    SpriteInstance(const SpriteInstance&)             = delete;
    SpriteInstance(SpriteInstance&&)                  = delete;
    SpriteInstance & operator=(const SpriteInstance&) = delete;
    SpriteInstance & operator=(SpriteInstance&&)      = delete;
    
    void playAnim(u32 animHash, f32 duration, bool loop, u32 doneMessage);
    bool advanceAnim(f32 delta);

    Sprite * mpSprite;
    u32 mStageHash;
    RenderPass mPass;

    bool mHasBody;
    
    const AnimInfo * mpAnimInfo;
    const void * mpCurrentFrameElemsOffset;

    u32 mAnimHash;
    u32 mAnimFrameIdx;
    f32 mDurationPerFrame;
    f32 mDurationSinceFrameChange;

    bool mIsAnimating;
    bool mIsLooping;
    u32 mDoneMessage;
};

typedef UniquePtr<SpriteInstance> SpriteInstanceUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SPRITE_H
