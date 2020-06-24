//------------------------------------------------------------------------------
// SpriteMgr.h - Management of Sprite lifetimes, animations and collisions
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#ifndef GAEN_RENDER_SUPPORT_SPRITEMGR_H
#define GAEN_RENDER_SUPPORT_SPRITEMGR_H

#include "core/HashMap.h"
#include "core/List.h"
#include "math/mat43.h"

#include "engine/Handle.h"
#include "render_support/Sprite.h"
#include "render_support/SpritePhysics.h"

namespace gaen
{

class SpriteMgr
{
public:
    typedef HashMap<kMEM_Engine, u32, SpriteInstanceUP> SpriteMap;
    typedef HashMap<kMEM_Engine, task_id, List<kMEM_Engine, u32>> SpriteOwners;

    ~SpriteMgr();

    void update(f32 delta);

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    SpriteMap mSpriteMap;
    SpriteOwners mSpriteOwners;

    SpritePhysics mPhysics;
};

// Compose API
class Entity;
namespace system_api
{
i32 sprite_create(AssetHandleP pAssetHandle, i32 stageHash, i32 passHash, const mat43 & transform, Entity * pCaller);
void sprite_play_anim(i32 spriteUid, i32 animHash, f32 duration, bool loop, i32 doneMessage, Entity * pCaller);
void sprite_set_velocity(i32 spriteUid, const vec2 & velocity, Entity * pCaller);
void sprite_init_body(i32 spriteUid, f32 mass, i32 group, ivec4 mask03, ivec4 mask47, Entity * pCaller);

void sprite_stage_show(i32 stageHash, Entity * pCaller);
void sprite_stage_hide(i32 stageHash, Entity * pCaller);
void sprite_stage_remove(i32 stageHash, Entity * pCaller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_SPRITEMGR_H
