//------------------------------------------------------------------------------
// RaycastCamera.h - Utility to keep track of a ray casting camera attributes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_ENGINE_RAYCAST_CAMERA
#define GAEN_ENGINE_RAYCAST_CAMERA

#include "engine/math.h"

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
        Vec3 near;
        Vec3 far;
    };

    RaycastCamera(u32 width, u32 height, f32 nearZ, f32 farZ);

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
    
    u32 width() const  { return mWidth; }
    u32 height() const { return mHeight; }
    f32 nearZ() const  { return mNearZ; }
    f32 farZ() const   { return mFarZ; }

    void applyTransform(const Mat34 & transform);

private:
    void init();
    void reseet();
    void calcPlanes();
    
    u32 mWidth;
    u32 mHeight;
    f32 mNearZ;
    f32 mFarZ;

    // Initial, zero'd position corners and planes
    // Current, transformed corners and plans
    Corner mCornersInit[kCOR_COUNT];
    Corner mCorners[kCOR_COUNT];
    Plane  mPlanes[kPLA_COUNT];
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_RAYCAST_CAMERA
