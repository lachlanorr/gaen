//------------------------------------------------------------------------------
// RaycastCamera.cpp - Utility to keep track of a ray casting camera attributes
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

#include "engine/stdafx.h"

#include "engine/RaycastCamera.h"

namespace gaen
{

RaycastCamera::RaycastCamera(u32 width, u32 height, f32 nearZ, f32 farZ)
    , mWidth(width)
    , mHeight(height)
    , mNearZ(nearZ)
    , mFarZ(farZ)
{
    ASSERT(width == 0,  "Width is zero");
    ASSERT(height == 0, "Height is zero");

    init();
}
    

void RaycastCamera::applyTransform(const Mat34 & transform)
{
    // Reset points to our default unmoved positions, and
    // than transform each point.
    reset();

    for (u32 ct = 0; ct < kCOR_COUNT; ++ct)
    {
        mCorners[ct].near = transform * mCorners[ct].near;
        mCorners[ct].far = transform * mCorners[ct].far;
    }

    calcPlanes();
}

RaycastCamera::init()
{
    for (u32 ct = 0; ct <= kCOR_COUNT; ++ct)
    {
        mCornersInit[ct].type = ct;
    }

    f32 vNear = 1.0f;          
    f32 hNear = mWidth / (f32)mHeight;

    // camera to nearz to find angles
    f32 vFov = atan(vNear / mNearZ);
    f32 hFov = atan(hNear / mNearZ);

    f32 vFar = tan(vFov) * mFarZ;
    f32 hFar = tan(hFov) * mFarZ;


    // calculate rays for each corner
    mCornersInit[kCOR_BottomLeft].near  = Vec3(-hNear, -vNear, mNearZ);
    mCornersInit[kCOR_BottomLeft].far    = Vec3(-hFar,  -vFar,  mFarZ);

    mCornersInit[kCOR_BottomRight].near = Vec3( hNear, -vNear, mNearZ);
    mCornersInit[kCOR_BottomRight].far   = Vec3( hFar,  -vFar,  mFarZ);

    mCornersInit[kCOR_TopLeft].near     = Vec3(-hNear, vNear, mNearZ);
    mCornersInit[kCOR_TopLeft].far       = Vec3(-hFar,  vFar,  mFarZ);

    mCornersInit[kCOR_TopRight].near    = Vec3( hNear, vNear, mNearZ);
    mCornersInit[kCOR_TopRight].far      = Vec3( hFar,  vFar,  mFarZ);

    reset();
    calcPlanes();
}

void RaycastCorners::reset()
{
    memcpy(mCorners, mCornersInit, sizeof(mCorners));
}

void RaycastCamera::calcPlanes()
{
    mPlanes[kPLA_Near] = Plane(mCorners[kCOR_BottomLeft].near,
                               mCorners[kCOR_TopLeft].near,
                               mCorners[kCOR_TopRight].near);

    mPlanes[kPLA_Far] = Plane(mCorners[kCOR_BottomLeft].far,
                              mCorners[kCOR_BottomRight].far,
                              mCorners[kCOR_TopRight].far);

    mPlanes[kPLA_Left] = Plane(mCorners[kCOR_BottomLeft].near,
                               mCorners[kCOR_BottomLeft].far,
                               mCorners[kCOR_TopLeft].far);

    mPlanes[kPLA_Right] = Plane(mCorners[kCOR_BottomRight].near,
                                mCorners[kCOR_BottomRight].far,
                                mCorners[kCOR_TopRight].far);

    mPlanes[kPLA_Top] = Plane(mCorners[kCOR_TopLeft].near,
                              mCorners[kCOR_TopLeft].far,
                              mCorners[kCOR_TopRight].far);
    
    mPlanes[kPLA_Bottom] = Plane(mCorners[kCOR_BottomLeft].near,
                                 mCorners[kCOR_BottomRight].near,
                                 mCorners[kCOR_BottomRight].far);
}

} // namespace gaen
