//------------------------------------------------------------------------------
// SpriteMgr.h - Management of Sprite lifetimes, animations and collisions
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

#ifndef GAEN_ENGINE_SPRITEMGR_H
#define GAEN_ENGINE_SPRITEMGR_H

#include "core/HashMap.h"
#include "core/List.h"

#include "engine/Handle.h"
#include "engine/Sprite.h"
#include "engine/SpritePhysics.h"

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
i32 sprite_create(AssetHandleP pAssetHandle, i32 stageHash, const glm::mat4x3 & transform, Entity & caller);
void sprite_play_anim(i32 spriteUid, i32 animHash, f32 duration, bool loop, i32 doneMessage, Entity & caller);
void sprite_set_velocity(i32 spriteUid, const glm::vec2 & velocity, Entity & caller);
void sprite_init_body(i32 spriteUid, f32 mass, i32 group, glm::ivec4 mask03, glm::ivec4 mask47, Entity & caller);

void sprite_show_stage(i32 stageHash, Entity & caller);
void sprite_hide_stage(i32 stageHash, Entity & caller);
void sprite_destroy_stage(i32 stageHash, Entity & caller);

}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITEMGR_H
