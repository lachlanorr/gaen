//------------------------------------------------------------------------------
// ModelMgr.h - Provides material ordered mesh iteration and model/material release semantics
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

#ifndef GAEN_RENDER_SUPPORT_MODELMGR_H
#define GAEN_RENDER_SUPPORT_MODELMGR_H

#include "gaen/core/HashMap.h"
#include "gaen/core/List.h"
#include "gaen/math/mat43.h"
#include "gaen/math/vec3.h"

#include "gaen/engine/Task.h"
#include "gaen/engine/Handle.h"
#include "gaen/render_support/Model.h"
#include "gaen/render_support/ModelPhysics.h"

namespace gaen
{

class ModelMgr
{
public:
    typedef HashMap<kMEM_Engine, u32, ModelInstanceUP> ModelMap;
    typedef HashMap<kMEM_Engine, task_id, List<kMEM_Engine, u32>> ModelOwners;

    ~ModelMgr();

    void update(f32 delta);

    template <typename T>
    MessageResult message(const T& msgAcc);

    void transformModel(i32 modelUid, const mat43 & transform);
private:
    ModelMap mModelMap;
    ModelOwners mModelOwners;

    ModelPhysics mPhysics;
};

// Compose API
class Entity;
namespace system_api
{
i32 model_anim_create(AssetHandleP pAssetHandleGmdl,
                      AssetHandleP pAssetHandleGaim,
                      i32 stageHash,
                      i32 passHash,
                      const mat43 & transform,
                      Entity * pCaller);

i32 model_create(AssetHandleP pAssetHandleGmdl,
                 i32 stageHash,
                 i32 passHash,
                 const mat43 & transform,
                 Entity * pCaller);

void model_init_body(i32 modelUid,
                     f32 mass,
                     f32 friction,
                     i32 flags,
                     i32 collisionShape,
                     vec3 linearFactor,
                     vec3 angularFactor,
                     i32 message,
                     i32 group,
                     ivec4 mask03,
                     ivec4 mask47,
                     Entity * pCaller);

void model_show(i32 modelUid, Entity * pCaller);
void model_hide(i32 modelUid, Entity * pCaller);

void model_remove_body(i32 modelUid, Entity * pCaller);

void model_set_velocity(i32 modelUid, const vec3 & velocity, Entity * pCaller);
void model_set_angular_velocity(i32 modelUid, const vec3 & velocity, Entity * pCaller);

void model_stage_show(i32 stageHash, Entity * pCaller);
void model_stage_hide(i32 stageHash, Entity * pCaller);
void model_stage_hide_all(Entity * pCaller);
void model_stage_remove(i32 stageHash, Entity * pCaller);
void model_stage_remove_all(Entity * pCaller);

i32 model_stage_camera_create_persp(i32 stageHash,
                                    f32 fov,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity * pCaller);

i32 model_stage_camera_create_ortho(i32 stageHash,
                                    f32 bounds,
                                    f32 nearClip,
                                    f32 farClip,
                                    const mat43 & view,
                                    Entity * pCaller);

void model_stage_camera_scale(i32 cameraUid,
                              f32 scale,
                              Entity * pCaller);

void model_stage_camera_view(i32 cameraUid,
                             const mat43 & view,
                             Entity * pCaller);

void model_stage_camera_scale_and_view(i32 cameraUid,
                                       f32 scale,
                                       const mat43 & view,
                                       Entity * pCaller);

void model_stage_camera_activate(i32 cameraUid, Entity * pCaller);
void model_stage_camera_remove(i32 cameraUid, Entity * pCaller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_MODELMGR_H
