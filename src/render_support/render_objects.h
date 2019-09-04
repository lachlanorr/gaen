//------------------------------------------------------------------------------
// render_objects.h - Classes shared by renderers and the engine
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

#ifndef GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H
#define GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H

#include "core/base_defines.h"

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "math/mat43.h"

#include "engine/Entity.h"

#include "assets/Color.h"
#include "engine/task.h"

namespace gaen
{

typedef i32 ruid;

enum RenderPass
{
    kRP_Opaque      = 0,
    kRP_Alpha       = 1,
    kRP_Engine      = 2,
    kRP_Debug       = 3,

    kRP_COUNT
};

RenderPass pass_from_hash(i32 hash);

class RenderObject
{
protected:
    RenderObject(task_id owner)
      : mOwner(owner)
      , mUid(next_uid())
    {}
    RenderObject(task_id owner, ruid uid)
      : mOwner(owner)
      , mUid(uid)
    {}

public:
    task_id owner() const { return mOwner; }
    ruid uid() const { return mUid; }

    static ruid next_uid();

private:
    static std::atomic<ruid> sNextRuid;
    task_id mOwner;
    ruid mUid;
};

class Camera : public RenderObject
{
public:
    Camera(task_id owner, ruid uid, u32 stageHash, f32 scale, const mat4 & projection, const mat43 & view)
      : RenderObject(owner, uid)
      , mStageHash(stageHash)
      , mScale(scale)
      , mProjection(projection)
      , mView(view)
    {
        updateViewProjection();
    }

    Camera(task_id owner, u32 stageHash, f32 scale, const mat4 & projection, const mat43 & view)
      : Camera(owner, RenderObject::next_uid(), stageHash, scale, projection, view)
    {}

    u32 stageHash() const { return mStageHash; }

    const f32 scale() const { return mScale; }
    void setScale(f32 scale)
    {
        mScale = scale;
        updateViewProjection();
    }

    const mat4 & projection() const { return mProjection; }

    const mat43 & view() const { return mView; }
    void setView(const mat43 & view)
    {
        mView = view;
        updateViewProjection();
    }

    void setScaleAndView(f32 scale, const mat43 & view)
    {
        mScale = scale;
        mView = view;
        updateViewProjection();
    }

    const mat4 & viewProjection() const { return mViewProjection; }

private:
    void updateViewProjection()
    {
        mViewProjection = mat4::from_scale(mScale) * mProjection * mView;
    }

    u32 mStageHash;
    f32 mScale;
    mat4 mProjection;
    mat43 mView;
    mat4 mViewProjection;
};

class Light : public RenderObject
{
public:
    Light(task_id owner,
          ruid uid,
          u32 stageHash,
          Color color,
          f32 ambient,
          const vec3 & direction)
      : RenderObject(owner, uid)
      , mStageHash(stageHash)
      , mColor(Color::build_vec4(color))
      , mAmbient(ambient)
    {
        setDirection(direction);
    }

    Light(task_id owner,
          u32 stageHash,
          Color color,
          f32 ambient,
          const vec3 & direction)
      : Light(owner,
              RenderObject::next_uid(),
              stageHash,
              color,
              ambient,
              direction)
    {}

    const vec3 & direction() const { return mDirection; }
    void setDirection(const vec3 & direction)
    {
        mDirection = direction;
        mIncidence = -normalize(direction);
    }

    const vec3 & incidence() const { return mIncidence; }

    const vec3 & color() const { return mColor; }
    void setColor(vec3 & color) { mColor = color; }
    void setColor(Color color) { mColor = color.toVec3(); }

    const f32 ambient() const { return mAmbient; }
    void setAmbient(f32 ambient) { mAmbient = ambient; }
private:
    u32 mStageHash;
    vec3 mColor;
    f32 mAmbient;
    vec3 mDirection;
    vec3 mIncidence;
};

} // namespace gaen


#endif // #ifndef GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H
