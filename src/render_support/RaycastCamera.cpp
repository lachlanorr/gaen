//------------------------------------------------------------------------------
// RaycastCamera.cpp - Utility to keep track of a ray casting camera attributes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "render_support/stdafx.h"

#include "core/logging.h"

#include "math/common.h"
#include "math/matrices.h"

#include "render_support/RaycastCamera.h"

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

    mProjection = perspective(radians(60.0f),
                              mAspectRatio,
                              mNearZ,
                              mFarZ);
    mProjectionInv = ~mProjection;

    move(vec3(0.0f, 0.0f, 10.0f), quat(0, vec3(0.0f, 0.0f, -1.0f)));

    vec3 rayBottomLeft  = normalize(vec3(mProjectionInv * vec4(-1.0f, -1.0f, 0.0f, 1.0f)));
    vec3 rayBottomRight = vec3(-rayBottomLeft.x,  rayBottomLeft.y, rayBottomLeft.z);
    vec3 rayTopLeft     = vec3( rayBottomLeft.x, -rayBottomLeft.y, rayBottomLeft.z);
    vec3 rayTopRight    = vec3(-rayBottomLeft.x, -rayBottomLeft.y, rayBottomLeft.z);

    f32 cosAngle = dot(rayBottomLeft, normalize(vec3(0.0f, 0.0f, mNearZ)));

    f32 nearLen = -mNearZ / cosAngle;
    f32 farLen = -mFarZ / cosAngle;

    // calculate each corner
    vec3 corBLN = rayBottomLeft * nearLen;
    vec3 corBLF = rayBottomLeft * farLen;

    mCornersInit[kCOR_BottomLeft].nearPos  = corBLN;
    mCornersInit[kCOR_BottomLeft].farPos   = corBLF;

    mCornersInit[kCOR_BottomRight].nearPos = vec3(-corBLN.x, corBLN.y, corBLN.z);
    mCornersInit[kCOR_BottomRight].farPos  = vec3(-corBLF.x, corBLF.y, corBLF.z);

    mCornersInit[kCOR_TopLeft].nearPos = vec3(corBLN.x, -corBLN.y, corBLN.z);
    mCornersInit[kCOR_TopLeft].farPos  = vec3(corBLF.x, -corBLF.y, corBLF.z);

    mCornersInit[kCOR_TopRight].nearPos = vec3(-corBLN.x, -corBLN.y, corBLN.z);
    mCornersInit[kCOR_TopRight].farPos  = vec3(-corBLF.x, -corBLF.y, corBLF.z);

    reset();
    calcPlanes();
}

void RaycastCamera::move(const vec3 & pos, const quat & dir)
{
    mPos = pos;
    mDir = dir;

    //mView = mat4::translation(-mPos) * mat4::transpose(mat4(mDir));
    mView = mat4(-mPos) * mat4(mDir);
    //mView = ~mView;
    //mView.setTranslation(-pos);

    // Reset points to our default unmoved positions, and
    // than transform each point.
    reset();

    for (u32 ct = 0; ct < kCOR_COUNT; ++ct)
    {
        mCorners[ct].nearPos = dir * mCorners[ct].nearPos;
        mCorners[ct].farPos = dir * mCorners[ct].farPos;
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
