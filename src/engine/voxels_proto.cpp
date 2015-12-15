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
    VoxelRef vrLeftBottomBack   = voxelWorld.getVoxelRef(0, 0, SubVoxel::LeftBottomBack);
    VoxelRef vrLeftBottomFront  = voxelWorld.getVoxelRef(0, 0, SubVoxel::LeftBottomFront);
    VoxelRef vrLeftTopBack      = voxelWorld.getVoxelRef(0, 0, SubVoxel::LeftTopBack);
    VoxelRef vrLeftTopFront     = voxelWorld.getVoxelRef(0, 0, SubVoxel::LeftTopFront);
    VoxelRef vrRightBottomBack  = voxelWorld.getVoxelRef(0, 0, SubVoxel::RightBottomBack);
    VoxelRef vrRightBottomFront = voxelWorld.getVoxelRef(0, 0, SubVoxel::RightBottomFront);
    VoxelRef vrRightTopBack     = voxelWorld.getVoxelRef(0, 0, SubVoxel::RightTopBack);
    VoxelRef vrRightTopFront    = voxelWorld.getVoxelRef(0, 0, SubVoxel::RightTopFront);


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

class SphereHitTest
{
public:
    SphereHitTest(f32 radius)
      : mRadius(radius) {}

    bool operator()(const AABB_MinMax & aabb) const
    {
        f32 aabbLen = aabb.center().length();
        return aabbLen < mRadius || abs(aabbLen - mRadius) <= (aabb.max.x() - aabb.min.x()) / 1.5f;
    }
private:
    f32 mRadius;
};

template <class HitTestType>
static VoxelRef set_shape_recursive(VoxelWorld & voxelWorld, u32 imageIdx, u32 & voxelIdx, u32 depth, const AABB_MinMax & aabb, HitTestType hitTest)
{
    if (depth == 0)
    {
        if (hitTest(aabb))
            return VoxelRef::terminal_full(1);
        else
            return VoxelRef::terminal_empty();
    }

    AABB_MinMax aabbLbb = voxel_subspace(aabb, SubVoxel::LeftBottomBack);
    AABB_MinMax aabbLbf = voxel_subspace(aabb, SubVoxel::LeftBottomFront);
    AABB_MinMax aabbLtb = voxel_subspace(aabb, SubVoxel::LeftTopBack);
    AABB_MinMax aabbLtf = voxel_subspace(aabb, SubVoxel::LeftTopFront);
    AABB_MinMax aabbRbb = voxel_subspace(aabb, SubVoxel::RightBottomBack);
    AABB_MinMax aabbRbf = voxel_subspace(aabb, SubVoxel::RightBottomFront);
    AABB_MinMax aabbRtb = voxel_subspace(aabb, SubVoxel::RightTopBack);
    AABB_MinMax aabbRtf = voxel_subspace(aabb, SubVoxel::RightTopFront);

    bool lbbIn = hitTest(aabbLbb);
    bool lbfIn = hitTest(aabbLbf);
    bool ltbIn = hitTest(aabbLtb);
    bool ltfIn = hitTest(aabbLtf);
    bool rbbIn = hitTest(aabbRbb);
    bool rbfIn = hitTest(aabbRbf);
    bool rtbIn = hitTest(aabbRtb);
    bool rtfIn = hitTest(aabbRtf);

    VoxelRef lbb = lbbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLbb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef lbf = lbfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLbf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef ltb = ltbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLtb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef ltf = ltfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbLtf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rbb = rbbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRbb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rbf = rbfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRbf, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rtb = rtbIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRtb, hitTest) : VoxelRef::terminal_empty();
    VoxelRef rtf = rtfIn ? set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth-1, aabbRtf, hitTest) : VoxelRef::terminal_empty();


    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftBottomBack,   lbb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftBottomFront,  lbf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftTopBack,      ltb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::LeftTopFront,     ltf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightBottomBack,  rbb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightBottomFront, rbf);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightTopBack,     rtb);
    voxelWorld.setVoxelRef(imageIdx, voxelIdx, SubVoxel::RightTopFront,    rtf);

    VoxelRef ret = VoxelRef(1, imageIdx, voxelIdx);
    voxelIdx++;
    return ret;
}

template <class HitTestType>
static VoxelRoot set_shape_generic(VoxelWorld & voxelWorld, u32 imageIdx, u32 voxelIdx, u32 depth, const Vec3 & pos, f32 rad, const Mat3 & rot, HitTestType hitTest)
{
    VoxelRoot voxelRoot;
    voxelRoot.pos = pos;
    voxelRoot.rad = rad;
    voxelRoot.rot = rot;
    voxelRoot.children = set_shape_recursive(voxelWorld, imageIdx, voxelIdx, depth, AABB_MinMax(rad), hitTest);
    return voxelRoot;
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
    voxelRoot = set_shape_generic(voxelWorld, 0, 0, 5, Vec3(-3.0f, -2.0f, -20.0f), kRad, Mat3::rotation(Vec3(0.0f, 0.0f, 0.0f)), SphereHitTest(kRad));
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
