//------------------------------------------------------------------------------
// Camera.h - Camera containing projection and view matrices
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

#ifndef GAEN_RENDERERGL_CAMERA_H
#define GAEN_RENDERERGL_CAMERA_H

#include <glm/mat4x4.hpp>
#include "engine/glm_ext.h"

namespace gaen
{

class Camera
{
public:
    Camera(glm::mat4 & projection, glm::mat4x3 & transform)
    {
        mProjection = projection;
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

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_CAMERA_H
