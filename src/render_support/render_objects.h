//------------------------------------------------------------------------------
// render_objects.h - Classes shared by renderers and the engine
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

#ifndef GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H
#define GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "core/base_defines.h"

#include "engine/Entity.h"
#include "engine/glm_ext.h"

#include "assets/Color.h"
#include "engine/task.h"

namespace gaen
{

typedef i32 ruid;

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
    Camera(task_id owner, ruid uid, u32 stageHash, glm::mat4 & projection, glm::mat4 & view)
      : RenderObject(owner, uid)
      , mStageHash(stageHash)
      , mProjection(projection)
    {
        setView(view);
    }

    Camera(task_id owner, u32 stageHash, glm::mat4 & projection, glm::mat4 & view)
      : Camera(owner, RenderObject::next_uid(), stageHash, projection, view)
    {}

    u32 stageHash() { return mStageHash; }

    const glm::mat4 & projection() const { return mProjection; }

    const glm::mat4 & view(const glm::mat4 & view) { return mView; }
    void setView(const glm::mat4 & view)
    {
        mView = view;
        mViewProjection = mProjection * mView;
    }
    
    const glm::mat4 & viewProjection() const { return mViewProjection; }

private:
    u32 mStageHash;
    glm::mat4 mProjection;
    glm::mat4 mView;
    glm::mat4 mViewProjection;
};

class LightDistant : public RenderObject
{
public:
    LightDistant(task_id owner,
                 ruid uid,
                 u32 stageHash,
                 Color color,
                 f32 ambient,
                 const glm::vec3 & direction)
      : RenderObject(owner, uid)
      , mStageHash(stageHash)
      , mColor(Color::build_vec4(color))
      , mAmbient(ambient)
    {
        setDirection(direction);
    }

    LightDistant(task_id owner,
                 u32 stageHash,
                 Color color,
                 f32 ambient,
                 const glm::vec3 & direction)
      : LightDistant(owner,
                     RenderObject::next_uid(),
                     stageHash,
                     color,
                     ambient,
                     direction)
    {}

    const glm::vec3 & direction() const { return mDirection; }
    void setDirection(const glm::vec3 & direction)
    {
        mDirection = direction;
        mIncidence = -glm::normalize(direction);
    }

    const glm::vec3 & incidence() const { return mIncidence; }

    const glm::vec3 & color() const { return mColor; }
    void setColor(glm::vec3 & color) { mColor = color; }
    void setColor(Color color) { mColor = color.toVec3(); }

    const f32 ambient() const { return mAmbient; }
    void setAmbient(f32 ambient) { mAmbient = ambient; }
private:
    u32 mStageHash;
    glm::vec3 mColor;
    f32 mAmbient;
    glm::vec3 mDirection;
    glm::vec3 mIncidence;
};

} // namespace gaen


#endif // #ifndef GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H
