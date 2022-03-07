//------------------------------------------------------------------------------
// RaycastCamera.h - Utility to keep track of a ray casting camera attributes
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

#ifndef GAEN_RENDER_SUPPORT_RAYCAST_CAMERA
#define GAEN_RENDER_SUPPORT_RAYCAST_CAMERA

#include "gaen/math/vec3.h"
#include "gaen/math/quat.h"
#include "gaen/math/mat4.h"

#include "gaen/render_support/collision.h"

namespace gaen
{

class RaycastCamera
{
public:
    enum CornerType
    {
        kCOR_BottomLeft = 0,
        kCOR_BottomRight = 1,
        kCOR_TopLeft = 2,
        kCOR_TopRight = 3,

        kCOR_COUNT = 4
    };

    enum PlaneType
    {
        kPLA_Near   = 0,
        kPLA_Far    = 1,
        kPLA_Left   = 2,
        kPLA_Right  = 3,
        kPLA_Top    = 4,
        kPLA_Bottom = 5,

        kPLA_COUNT  = 6
    };

    struct Corner
    {
        vec3 nearPos;
        vec3 farPos;
    };

    void init(u32 screenWidth, u32 screenHeight, f32 fov, f32 nearZ, f32 farZ);

    const Corner & corner(CornerType cornerType)
    {
        PANIC_IF(cornerType < 0 || cornerType >= kCOR_COUNT, "Invalid corner index: %d", cornerType);
        return mCorners[cornerType];
    }

    const Plane & plane(PlaneType planeType)
    {
        PANIC_IF(planeType < 0 || planeType >= kPLA_COUNT, "Invalid plane index: %d", planeType);
        return mPlanes[planeType];
    }
    
    f32 screenWidth() const  { return mScreenWidth; }
    f32 screenHeight() const { return mScreenHeight; }
    f32 aspectRatio() const { return mAspectRatio; }
    f32 nearZ() const  { return mNearZ; }
    f32 farZ() const   { return mFarZ; }

    const mat4 & projection() const { return mProjection; }
    const mat4 & projectionInv() const { return mProjectionInv; }

    const vec3 & position() const { return mPos; }
    const quat & direction() const { return mDir; }

    const mat4 & view() const { return mView; }

    void move(const vec3 & pos, const quat & dir);
    void moveFps(const vec3 & pos, f32 pitch, f32 yaw);

private:
    void reset();
    void calcPlanes();
    
    f32 mScreenWidth;
    f32 mScreenHeight;

    f32 mNearZ;
    f32 mFarZ;

    f32 mFov;
    f32 mAspectRatio;

    mat4 mProjection;
    mat4 mProjectionInv;

    vec3 mPos;
    quat mDir;
    mat4 mView;

    // Initial, zero'd position corners and planes
    // Current, transformed corners and plans
    Corner mCornersInit[kCOR_COUNT];
    Corner mCorners[kCOR_COUNT];
    Plane  mPlanes[kPLA_COUNT];
};

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_RAYCAST_CAMERA
