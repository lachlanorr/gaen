//------------------------------------------------------------------------------
// voxels.cpp - CPU side voxel support
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
#include "core/logging.h"

#include "engine/voxels_proto.h"

namespace gaen
{

ShaderSimulator::ShaderSimulator()
  : mpRaycastCamera(nullptr)
  , color(0, 0, 0)
{
    mIsInit = false;
    mFrameBuffer = nullptr;
    mDepthBuffer = nullptr;
    mDepthBufferBlank = nullptr;
}

ShaderSimulator::~ShaderSimulator()
{
    if (mIsInit)
    {
        if (mFrameBuffer)
            GDELETE(mFrameBuffer);

        if (mDepthBuffer)
            GDELETE(mDepthBuffer);

        if (mDepthBufferBlank)
            GDELETE(mDepthBufferBlank);
    }
}

static void set_shape_0(VoxelWorld & voxelWorld)
{
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomBack,   VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomFront,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopBack,      VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopFront,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomBack,  VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomFront, VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopBack,     VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopFront,    VoxelRef::terminal_empty());
}

static void set_shape_1(VoxelWorld & voxelWorld)
{
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomBack,   VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomFront,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopBack,      VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopFront,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomBack,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomFront, VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopBack,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopFront,    VoxelRef::terminal_empty());
}

static void set_shape_2(VoxelWorld & voxelWorld)
{
    VoxelRef vrLeftBottomBack   = voxelWorld.voxelRef(0, 0, SubVoxel::LeftBottomBack);
    VoxelRef vrLeftBottomFront  = voxelWorld.voxelRef(0, 0, SubVoxel::LeftBottomFront);
    VoxelRef vrLeftTopBack      = voxelWorld.voxelRef(0, 0, SubVoxel::LeftTopBack);
    VoxelRef vrLeftTopFront     = voxelWorld.voxelRef(0, 0, SubVoxel::LeftTopFront);
    VoxelRef vrRightBottomBack  = voxelWorld.voxelRef(0, 0, SubVoxel::RightBottomBack);
    VoxelRef vrRightBottomFront = voxelWorld.voxelRef(0, 0, SubVoxel::RightBottomFront);
    VoxelRef vrRightTopBack     = voxelWorld.voxelRef(0, 0, SubVoxel::RightTopBack);
    VoxelRef vrRightTopFront    = voxelWorld.voxelRef(0, 0, SubVoxel::RightTopFront);


    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomBack,   VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomFront,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopBack,      VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopFront,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomBack,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomFront, VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopBack,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopFront,    VoxelRef::terminal_empty());
}
static void set_shape_3(VoxelWorld & voxelWorld)
{
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomBack,   VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomFront,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopBack,      VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopFront,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomBack,  VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomFront, VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopBack,     VoxelRef::terminal_empty());
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopFront,    VoxelRef::terminal_empty());
}

static void set_shape_full(VoxelWorld & voxelWorld)
{
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomBack,   VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftBottomFront,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopBack,      VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::LeftTopFront,     VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomBack,  VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightBottomFront, VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopBack,     VoxelRef::terminal_full(1));
    voxelWorld.setVoxelRef(0, 0, SubVoxel::RightTopFront,    VoxelRef::terminal_full(1));
}



void ShaderSimulator::init(u32 outputImageSize, RaycastCamera * pRaycastCamera)
{
    mFrameBuffer = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(Pix_RGB8));
    mDepthBuffer = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(Pix_R32F));
    mDepthBufferBlank = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(Pix_R32F));
    mpRaycastCamera = pRaycastCamera;

    // prepare blanked out depth buffer
    for (u16 y = 0; y < outputImageSize; ++y)
    {
        for (u16 x = 0; x < outputImageSize; ++x)
        {
            mDepthBufferBlank->imageStoreR32F(x, y, Pix_R32F(-FLT_MAX));
        }
    }

    // prep camera
    cameraPos = Vec3(0.0f, 100.0f, 10.0f);
    windowSize = Vec2((f32)outputImageSize, (f32)outputImageSize);
    nearZ = 5.0f;
    farZ = 10000.0f;

    static const f32 kRad = 2.0f;
    voxelRoot = set_shape_generic(voxelWorld, 0, 0, 5, Vec3(1.0f, 2.0f, -20.0f), kRad, Mat3::rotation(Vec3(0.0f, 0.0f, 0.0f)), SphereHitTest(kRad));
}

void ShaderSimulator::render(const RaycastCamera & camera, const List<kMEM_Renderer, DirectionalLight> & lights)
{
    projectionInv = camera.projectionInv();

    ASSERT(mpRaycastCamera);
    cameraPos = mpRaycastCamera->position();

    if (lights.size() > 0)
    {
        lightDir = lights.front().direction;
        lightColor = lights.front().color;
    }

    mDepthBuffer->copy(*mDepthBufferBlank);

    Pix_RGB8 * pix = reinterpret_cast<Pix_RGB8*>(mFrameBuffer->buffer());
    Pix_R32F * dpix = reinterpret_cast<Pix_R32F*>(mDepthBuffer->buffer());

    gl_FragCoord.z = 0;

    for (gl_FragCoord.y = 0; gl_FragCoord.y < mFrameBuffer->size(); ++gl_FragCoord.y)
    {
        for (gl_FragCoord.x = 0; gl_FragCoord.x < mFrameBuffer->size(); ++gl_FragCoord.x)
        {
            color.r = 0;
            color.g = 0;
            color.b = 0;
            zDepth = -FLT_MAX;

            //fragShader_Blue();
            fragShader_Raycast();

            if (zDepth > dpix->r)
            {
                *pix = color;
                dpix->r = zDepth;
            }
            dpix++;
            pix++;
        }
    }

    uniform0++;
}

void ShaderSimulator::fragShader_Blue()
{
    static u8 g = 0;
    color.r = 255;
    color.g = (u8)uniform0;
    color.b = 255;
}

void ShaderSimulator::fragShader_Raycast()
{
    Vec3 rayScreenPos(2.0f * gl_FragCoord.x / windowSize.x() - 1.0f,
                      2.0f * gl_FragCoord.y / windowSize.y() - 1.0f,
                      0.0f);

    Vec3 rayDirProj = Vec3::normalize(Mat4::multiply(projectionInv, rayScreenPos));

    Vec3 rayDir = Quat::multiply(mpRaycastCamera->direction(), rayDirProj);
    Vec3 rayPos = cameraPos;
    
    VoxelRef voxelRef;
    Vec3 normal;
    VoxelFace face;
    Vec2 faceUv;
    u32 hit = (u32)test_ray_voxel(&voxelRef, &normal, &zDepth, &face, &faceUv, voxelWorld, rayPos, rayDir, voxelRoot, 16);

    if (hit)
    {
        // LORRTEMP
        //LOG_INFO("HIT: pRi->searchOrder[pRi->searchIndex]: %d", pRi->searchOrder[pRi->searchIndex]);
        //LOG_INFO("normal: %f, %f, %f", normal.x(), normal.y(), normal.z());

        f32 intensity = maxval(Vec3::dot(normal, lightDir), 0.0f);

        switch (face)
        {
        case VoxelFace::Left:
            color.r = 150;
            color.g = (u8)minval(150.0f, 150 * faceUv.x());
            color.b = (u8)minval(150.0f, 150 * faceUv.y());
            break;
        case VoxelFace::Right:
            color.r = 255;
            color.g = (u8)minval(255.0f, 255 * faceUv.x());
            color.b = (u8)minval(255.0f, 255 * faceUv.y());
            break;
        case VoxelFace::Bottom:
            color.r = (u8)minval(150.0f, 150 * faceUv.x());
            color.g = 150;
            color.b = (u8)minval(150.0f, 150 * faceUv.y());
            break;
        case VoxelFace::Top:
            color.r = (u8)minval(255.0f, 255 * faceUv.x());
            color.g = 255;
            color.b = (u8)minval(255.0f, 255 * faceUv.y());
            break;
        case VoxelFace::Back:
            color.r = (u8)minval(150.0f, 150 * faceUv.x());
            color.g = (u8)minval(150.0f, 150 * faceUv.y());
            color.b = 150;
            break;
        case VoxelFace::Front:
            color.r = (u8)minval(255.0f, 255 * faceUv.x());
            color.g = (u8)minval(255.0f, 255 * faceUv.y());
            color.b = 255;
            break;
        }

        //color.r = (u8)maxval(intensity * 255, 10.0f);
        //color.g = (u8)maxval(intensity * 255, 10.0f);
        //color.b = (u8)maxval(intensity * 255, 10.0f);
    }
    else
    {
        color.r = 100;
        color.g = 125;
        color.b = 255;
        zDepth = -(FLT_MAX * 0.90f);
    }

/*
    color.r = (u8)(abs(rayDir.x()) * 512.0f);
    color.g = (u8)(abs(rayDir.y()) * 1024.0f);
    color.b = (u8)(abs(rayDir.z()) * 255.0f);
    */
//    color.r = (u8)((rayDir.x() + 1.0f) * 127.5f);
//    color.g = (u8)((rayDir.y() + 1.0f) * 127.5f);

//    color.r = (u8)(gl_FragCoord.x / windowSize.x() * 255.0f);
//    color.g = (u8)(gl_FragCoord.y / windowSize.y() * 255.0f);
}

} // namespace gaen
