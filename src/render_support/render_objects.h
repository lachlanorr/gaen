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

#include "engine/glm_ext.h"

#include "assets/Color.h"
#include "engine/task.h"

namespace gaen
{

typedef u32 ruid;

class RenderObject
{
protected:
    RenderObject(task_id owner);
    RenderObject(task_id owner, ruid uid)
      : mOwner(owner)
      , mUid(uid)
    {}

public:    
    task_id owner() const { return mOwner; }
    ruid uid() const { return mUid; }

private:
    static std::atomic<ruid> sNextRuid;
    task_id mOwner;
    ruid mUid;
};

class Camera : public RenderObject
{
public:
    Camera(task_id owner, glm::mat4 & projection, glm::mat4x3 & transform)
      : RenderObject(owner)
      , mProjection(projection)
    {
        setTransform(transform);
    }

    const glm::mat4 & projection() const { return mProjection; }

    const glm::mat4x3 & transform(const glm::mat4x3 & transform) { return mTransform; }
    void setTransform(const glm::mat4x3 & transform)
    {
        mTransform = to_mat4x4(transform);
        mView = inverse(mTransform);
        mViewProjection = mView * mProjection;
    }
    
    const glm::mat4 & view() const { return mView; }
    const glm::mat4 & viewProjection() const { return mViewProjection; }

private:
    glm::mat4 mProjection;
    glm::mat4 mTransform;
    glm::mat4 mView;
    glm::mat4 mViewProjection;
};

struct DirectionalLight : public RenderObject
{
    glm::vec3 direction;
    glm::vec4 color;

    DirectionalLight(task_id owner, const glm::vec3 & direction, Color color)
      : RenderObject(owner)
      , direction(direction)
      , color(Color::build_vec4(color))
    {}
};

struct PointLight : public RenderObject
{
    glm::vec3 location;
    glm::vec4 color;

    PointLight(task_id owner, const glm::vec3 & location, Color color)
      : RenderObject(owner)
      , location(location)
      , color(Color::build_vec4(color))
    {}
};



} // namespace gaen


#endif // #ifndef GAEN_RENDER_SUPPORT_RENDER_OBJECTS_H
