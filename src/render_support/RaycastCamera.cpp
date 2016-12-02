//------------------------------------------------------------------------------
// RaycastCamera.cpp - Utility to keep track of a ray casting camera attributes
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

#include "render_support/stdafx.h"

#include "core/logging.h"

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

    mProjection = glm::perspective(glm::radians(60.0f),
                                   mAspectRatio,
                                   mNearZ,
                                   mFarZ);
    mProjectionInv = glm::inverse(mProjection);

    move(glm::vec3(0.0f, 0.0f, 10.0f), glm::quat(0, glm::vec3(0.0f, 0.0f, -1.0f)));

    glm::vec3 rayBottomLeft  = glm::normalize(glm::vec3(mProjectionInv * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f)));
    glm::vec3 rayBottomRight = glm::vec3(-rayBottomLeft.x,  rayBottomLeft.y, rayBottomLeft.z);
    glm::vec3 rayTopLeft     = glm::vec3( rayBottomLeft.x, -rayBottomLeft.y, rayBottomLeft.z);
    glm::vec3 rayTopRight    = glm::vec3(-rayBottomLeft.x, -rayBottomLeft.y, rayBottomLeft.z);

    f32 cosAngle = glm::dot(rayBottomLeft, glm::normalize(glm::vec3(0.0f, 0.0f, mNearZ)));

    f32 nearLen = -mNearZ / cosAngle;
    f32 farLen = -mFarZ / cosAngle;

    // calculate each corner
    glm::vec3 corBLN = rayBottomLeft * nearLen;
    glm::vec3 corBLF = rayBottomLeft * farLen;

    mCornersInit[kCOR_BottomLeft].nearPos  = corBLN;
    mCornersInit[kCOR_BottomLeft].farPos   = corBLF;

    mCornersInit[kCOR_BottomRight].nearPos = glm::vec3(-corBLN.x, corBLN.y, corBLN.z);
    mCornersInit[kCOR_BottomRight].farPos  = glm::vec3(-corBLF.x, corBLF.y, corBLF.z);

    mCornersInit[kCOR_TopLeft].nearPos = glm::vec3(corBLN.x, -corBLN.y, corBLN.z);
    mCornersInit[kCOR_TopLeft].farPos  = glm::vec3(corBLF.x, -corBLF.y, corBLF.z);

    mCornersInit[kCOR_TopRight].nearPos = glm::vec3(-corBLN.x, -corBLN.y, corBLN.z);
    mCornersInit[kCOR_TopRight].farPos  = glm::vec3(-corBLF.x, -corBLF.y, corBLF.z);

    reset();
    calcPlanes();
}

void RaycastCamera::move(const glm::vec3 & pos, const glm::quat & dir)
{
    mPos = pos;
    mDir = dir;

    //mView = glm::mat4::translation(-mPos) * glm::mat4::transpose(glm::mat4(mDir));
    mView = glm::translate(glm::mat4(1.0f), -mPos) * glm::toMat4(mDir);
    //mView = glm::mat4::inverse(mView);
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
