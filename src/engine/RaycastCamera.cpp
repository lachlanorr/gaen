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

void RaycastCamera::init(u32 screenWidth, u32 screenHeight, f32 fov, f32 nearZ, f32 farZ)
{
    mScreenWidth = (f32)screenWidth;
    mScreenHeight = (f32)screenHeight;
    mFov = fov;
    mNearZ = nearZ;
    mFarZ = farZ;

    mAspectRatio = mScreenWidth / mScreenHeight;

    mProjection = Mat4::perspective(60.0f,
                                    mAspectRatio,
                                    mNearZ,
                                    mFarZ);
    mProjectionInv = Mat4::inverse(mProjection);

    Vec3 rayBottomLeft  = Vec3::normalize(Mat4::multiply(mProjectionInv, Vec3(-1.0f, -1.0f, 0.0f)));
    Vec3 rayBottomRight = Vec3(-rayBottomLeft.x(),  rayBottomLeft.y(), rayBottomLeft.z());
    Vec3 rayTopLeft     = Vec3( rayBottomLeft.x(), -rayBottomLeft.y(), rayBottomLeft.z());
    Vec3 rayTopRight    = Vec3(-rayBottomLeft.x(), -rayBottomLeft.y(), rayBottomLeft.z());

    f32 cosAngle = Vec3::dot(rayBottomLeft, Vec3::normalize(Vec3(0.0f, 0.0f, mNearZ)));

    f32 nearLen = -mNearZ / cosAngle;
    f32 farLen = -mFarZ / cosAngle;

    // calculate each corner
    Vec3 corBLN = rayBottomLeft * nearLen;
    Vec3 corBLF = rayBottomLeft * farLen;

    mCornersInit[kCOR_BottomLeft].nearPos  = corBLN;
    mCornersInit[kCOR_BottomLeft].farPos   = corBLF;

    mCornersInit[kCOR_BottomRight].nearPos = Vec3(-corBLN.x(), corBLN.y(), corBLN.z());
    mCornersInit[kCOR_BottomRight].farPos  = Vec3(-corBLF.x(), corBLF.y(), corBLF.z());

    mCornersInit[kCOR_TopLeft].nearPos = Vec3(corBLN.x(), -corBLN.y(), corBLN.z());
    mCornersInit[kCOR_TopLeft].farPos  = Vec3(corBLF.x(), -corBLF.y(), corBLF.z());

    mCornersInit[kCOR_TopRight].nearPos = Vec3(-corBLN.x(), -corBLN.y(), corBLN.z());
    mCornersInit[kCOR_TopRight].farPos  = Vec3(-corBLF.x(), -corBLF.y(), corBLF.z());

    reset();
    calcPlanes();
}

void RaycastCamera::applyTransform(const Mat34 & transform)
{
    // Reset points to our default unmoved positions, and
    // than transform each point.
    reset();

    for (u32 ct = 0; ct < kCOR_COUNT; ++ct)
    {
        mCorners[ct].nearPos = transform * mCorners[ct].nearPos;
        mCorners[ct].farPos = transform * mCorners[ct].farPos;
    }

    calcPlanes();
}

void RaycastCamera::reset()
{
    memcpy(mCorners, mCornersInit, sizeof(mCorners));
}

void RaycastCamera::calcPlanes()
{
    mPlanes[kPLA_Near] = Plane(mCorners[kCOR_BottomLeft].nearPos,
                               mCorners[kCOR_TopLeft].nearPos,
                               mCorners[kCOR_TopRight].nearPos);

    mPlanes[kPLA_Far] = Plane(mCorners[kCOR_BottomLeft].farPos,
                              mCorners[kCOR_BottomRight].farPos,
                              mCorners[kCOR_TopRight].farPos);

    mPlanes[kPLA_Left] = Plane(mCorners[kCOR_BottomLeft].nearPos,
                               mCorners[kCOR_BottomLeft].farPos,
                               mCorners[kCOR_TopLeft].farPos);

    mPlanes[kPLA_Right] = Plane(mCorners[kCOR_BottomRight].nearPos,
                                mCorners[kCOR_BottomRight].farPos,
                                mCorners[kCOR_TopRight].farPos);

    mPlanes[kPLA_Top] = Plane(mCorners[kCOR_TopLeft].nearPos,
                              mCorners[kCOR_TopLeft].farPos,
                              mCorners[kCOR_TopRight].farPos);
    
    mPlanes[kPLA_Bottom] = Plane(mCorners[kCOR_BottomLeft].nearPos,
                                 mCorners[kCOR_BottomRight].nearPos,
                                 mCorners[kCOR_BottomRight].farPos);
}

} // namespace gaen
