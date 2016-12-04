//------------------------------------------------------------------------------
// voxel_proto.cpp - CPU side voxel support
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

#include "render_support/voxel_proto.h"

namespace gaen
{

struct VoxelRefGpu
{
    u32 type;
    u32 material;
    u32 filledNeighbors;
    u32 imageIdx;
    u32 voxelIdx;
};

struct VoxelRootGpu
{
    glm::mat3 rot;
    glm::vec3 pos;
    VoxelRefGpu children;
    f32 rad;
};


inline VoxelRefGpu unpack_voxel_ref(const RG32U pix)
{
    VoxelRefGpu ref;

    ref.type = pix.r & 0x3; // 2 bits
    ref.material = (pix.r & 0xffc) >> 2; // 10 bits
    
    ref.imageIdx = (pix.r & 0x7000) >> 12; // 3 bits
    ref.voxelIdx = ((pix.g & 0x3f) << 17) | ((pix.r & 0xffff8000) >> 15); // 23 bits (17 in .r, 6 in .g)

    ref.filledNeighbors = (pix.g & 0xffffffc0) >> 6; // 26 bits

    return ref;
}

inline VoxelRootGpu extract_voxel_root(const ImageBuffer * voxelRoots, u32 voxelRoot)
{
    // logic here depends on rg32u pixels, 64 byte voxel roots, etc.
    static_assert(sizeof(VoxelRoot) == 8 * sizeof(RG32U), "VoxelRoot isn't same size as 8 pixels");

    u32 pixStart = voxelRoot * VoxelWorld::kPixelsPerVoxel;

    RG32U pix0 = voxelRoots->imageLoad<RG32U>(pixStart);
    RG32U pix1 = voxelRoots->imageLoad<RG32U>(pixStart + 1);
    RG32U pix2 = voxelRoots->imageLoad<RG32U>(pixStart + 2);
    RG32U pix3 = voxelRoots->imageLoad<RG32U>(pixStart + 3);
    RG32U pix4 = voxelRoots->imageLoad<RG32U>(pixStart + 4);
    RG32U pix5 = voxelRoots->imageLoad<RG32U>(pixStart + 5);
    RG32U pix6 = voxelRoots->imageLoad<RG32U>(pixStart + 6);
    RG32U pix7 = voxelRoots->imageLoad<RG32U>(pixStart + 7);

    VoxelRootGpu root;

    root.pos.x = glm::uintBitsToFloat(pix0.r);
    root.pos.y = glm::uintBitsToFloat(pix0.g);

    root.pos.z = glm::uintBitsToFloat(pix1.r);
    root.rot[0][0] = glm::uintBitsToFloat(pix1.g);

    root.rot[0][1] = glm::uintBitsToFloat(pix2.r);
    root.rot[0][2] = glm::uintBitsToFloat(pix2.g);

    root.rot[1][0] = glm::uintBitsToFloat(pix3.r);
    root.rot[1][1] = glm::uintBitsToFloat(pix3.g);

    root.rot[1][2] = glm::uintBitsToFloat(pix4.r);
    root.rot[2][0] = glm::uintBitsToFloat(pix4.g);

    root.rot[2][1] = glm::uintBitsToFloat(pix5.r);
    root.rot[2][2] = glm::uintBitsToFloat(pix5.g);

    root.rad = glm::uintBitsToFloat(pix6.r);
    // pix6.g is padding in C struct

    root.children = unpack_voxel_ref(pix7);

    return root;
}

//------------------------------------------------------------------------------
// GLSL Functions (END)
//------------------------------------------------------------------------------

ComputeShaderSimulator::~ComputeShaderSimulator()
{
    if (un_FrameBuffer)
        GDELETE(un_FrameBuffer);
}

void ComputeShaderSimulator::init(glm::uvec3 workGroupSize,
                                  glm::uvec3 numWorkGroups)
{
    static const u32 kFrameBufferSize = 512;
    ASSERT(workGroupSize.x * numWorkGroups.x <= kFrameBufferSize);
    ASSERT(workGroupSize.y * numWorkGroups.y <= kFrameBufferSize);

    static const f32 kRad = 2.0f;
    set_shape_generic(mVoxelWorld, 0, 0, 3, glm::vec3(1.0f, 2.0f, -20.0f), kRad, glm::mat3(1.0f), SphereHitTest(kRad));

    un_FrameBuffer = GNEW(kMEM_Engine, ImageBuffer, kFrameBufferSize, sizeof(RGB8));

    un_VoxelData = mVoxelWorld.voxelImage(0);
    un_VoxelRoots = mVoxelWorld.voxelRoots();
    un_VoxelRootCount = mVoxelWorld.voxelRootCount();

    gl_WorkGroupSize = workGroupSize;
    gl_NumWorkGroups = numWorkGroups;
}

void ComputeShaderSimulator::render(const RaycastCamera & camera, const List<kMEM_Renderer, LightDistant> & lights)
{
    // prpare uniforms
    un_CameraPos = camera.position();
    un_CameraDir = camera.direction();
    un_CameraProjectionInv = camera.projectionInv();

    for (gl_WorkGroupID.z = 0; gl_WorkGroupID.z < gl_NumWorkGroups.z; ++gl_WorkGroupID.z)
    {
        for (gl_WorkGroupID.y = 0; gl_WorkGroupID.y < gl_NumWorkGroups.y; ++gl_WorkGroupID.y)
        {
            for (gl_WorkGroupID.x = 0; gl_WorkGroupID.x < gl_NumWorkGroups.x; ++gl_WorkGroupID.x)
            {
                for (gl_LocalInvocationID.z = 0; gl_LocalInvocationID.z < gl_WorkGroupSize.z; ++gl_LocalInvocationID.z)
                {
                    for (gl_LocalInvocationID.y = 0; gl_LocalInvocationID.y < gl_WorkGroupSize.y; ++gl_LocalInvocationID.y)
                    {
                        for (gl_LocalInvocationID.x = 0; gl_LocalInvocationID.x < gl_WorkGroupSize.x; ++gl_LocalInvocationID.x)
                        {
                            gl_GlobalInvocationID = gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID;
                            gl_LocalInvocationIndex = (gl_LocalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y +
                                                       gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x);

                            

//                            LOG_INFO("%2u, %2u   %2u, %2u", gl_WorkGroupID.y, gl_WorkGroupID.x, gl_LocalInvocationID.y, gl_LocalInvocationID.x);
//                            LOG_INFO("%u, %u     %u, %u", gl_GlobalInvocationID.y, gl_GlobalInvocationID.x, gl_LocalInvocationID.y, gl_LocalInvocationID.x);
//                            LOG_INFO("%u, %u", gl_GlobalInvocationID.y, gl_GlobalInvocationID.x);

                            compShader_Raycast_gpu();
                        }
                    }
                }
            }
        }
    }
}

void ComputeShaderSimulator::compShader_Test()
{
    f32 rCol = (f32)gl_LocalInvocationID.x / (f32)gl_WorkGroupSize.x;
    f32 gCol = (f32)gl_LocalInvocationID.y / (f32)gl_WorkGroupSize.y;

    un_FrameBuffer->imageStore2d(gl_GlobalInvocationID.x,
                                 gl_GlobalInvocationID.y,
                                 RGB8((f32)gl_LocalInvocationID.x / (f32)gl_WorkGroupSize.x,
                                      (f32)gl_LocalInvocationID.y / (f32)gl_WorkGroupSize.y,
                                      0.0f));
}

void ComputeShaderSimulator::compShader_Raycast_gpu()
{
    // LORRTODO: Consider moving to uniform
    glm::vec2 windowSize((f32)(gl_WorkGroupSize.x * gl_NumWorkGroups.x),
                         (f32)(gl_WorkGroupSize.y * gl_NumWorkGroups.y));

    glm::vec4 rayScreenPos(2.0f * gl_GlobalInvocationID.x / windowSize.x - 1.0f,
                           2.0f * gl_GlobalInvocationID.y / windowSize.y - 1.0f,
                           0.0f,
                           1.0f);

    glm::vec3 rayDirProj = glm::vec3(glm::normalize(un_CameraProjectionInv * rayScreenPos));

    glm::vec3 rayDir = un_CameraDir * rayDirProj;
    glm::vec3 rayPos = un_CameraPos;

    glm::vec3 rayDirCol = (glm::vec3(rayScreenPos) + glm::vec3(1.0f, 1.0f, 1.0f)) / glm::vec3(2.0f, 2.0f, 2.0f);

    un_FrameBuffer->imageStore2d(gl_GlobalInvocationID.x,
                                 gl_GlobalInvocationID.y,
                                 RGB8(rayDirCol.x, rayDirCol.y, rayDirCol.z));

    for (u32 rootId = 0; rootId < un_VoxelRootCount; ++rootId)
    {
        VoxelRootGpu voxelRoot = extract_voxel_root(un_VoxelRoots, rootId);
    }
    /*
        VoxelRef voxelRef;
        glm::vec3 normal;
        VoxelFace face;
        glm::vec2 faceUv;
        u32 hit = (u32)test_ray_voxel_gpu(&voxelRef, &normal, &zDepth, &face, &faceUv, voxelWorld, rayPos, rayDir, voxelRoot, 16);

    if (hit)
    {
        f32 intensity = glm::max(glm::vec3::dot(normal, lightDir), 0.0f);

        switch (face)
        {
        case VoxelFace::Left:
            color.r = 150;
            color.g = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.b = (u8)glm::min(150.0f, 150 * faceUv.y);
            break;
        case VoxelFace::Right:
            color.r = 255;
            color.g = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.b = (u8)glm::min(255.0f, 255 * faceUv.y);
            break;
        case VoxelFace::Bottom:
            color.r = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.g = 150;
            color.b = (u8)glm::min(150.0f, 150 * faceUv.y);
            break;
        case VoxelFace::Top:
            color.r = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.g = 255;
            color.b = (u8)glm::min(255.0f, 255 * faceUv.y);
            break;
        case VoxelFace::Back:
            color.r = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.g = (u8)glm::min(150.0f, 150 * faceUv.y);
            color.b = 150;
            break;
        case VoxelFace::Front:
            color.r = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.g = (u8)glm::min(255.0f, 255 * faceUv.y);
            color.b = 255;
            break;
        }
    }
    else
    {
        color.r = 100;
        color.g = 125;
        color.b = 255;
        zDepth = -(FLT_MAX * 0.90f);
    }
*/
}


FragmentShaderSimulator::FragmentShaderSimulator()
  : mpRaycastCamera(nullptr)
  , color((u8)0, (u8)0, (u8)0)
{
    mIsInit = false;
    mFrameBuffer = nullptr;
    mDepthBuffer = nullptr;
    mDepthBufferBlank = nullptr;
}

FragmentShaderSimulator::~FragmentShaderSimulator()
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



void FragmentShaderSimulator::init(u32 outputImageSize, RaycastCamera * pRaycastCamera)
{
    mFrameBuffer = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(RGB8));
    mDepthBuffer = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(R32F));
    mDepthBufferBlank = GNEW(kMEM_Engine, ImageBuffer, outputImageSize, sizeof(R32F));
    mpRaycastCamera = pRaycastCamera;

    // prepare blanked out depth buffer
    for (u16 y = 0; y < outputImageSize; ++y)
    {
        for (u16 x = 0; x < outputImageSize; ++x)
        {
            mDepthBufferBlank->imageStore2d(x, y, R32F(-FLT_MAX));
        }
    }

    // prep camera
    cameraPos = glm::vec3(0.0f, 100.0f, 10.0f);
    windowSize = glm::vec2((f32)outputImageSize, (f32)outputImageSize);
    nearZ = 5.0f;
    farZ = 10000.0f;

    static const f32 kRad = 2.0f;
    voxelRoot = set_shape_generic(voxelWorld, 0, 0, 3, glm::vec3(1.0f, 2.0f, -20.0f), kRad, glm::mat3(1.0f), SphereHitTest(kRad));
}

void FragmentShaderSimulator::render(const RaycastCamera & camera, const List<kMEM_Renderer, LightDistant> & lights)
{
    projectionInv = camera.projectionInv();

    ASSERT(mpRaycastCamera);
    cameraPos = mpRaycastCamera->position();

    if (lights.size() > 0)
    {
        lightDir = lights.front().direction();
        lightColor = lights.front().color();
    }

    mDepthBuffer->copy(*mDepthBufferBlank);

    RGB8 * pix = reinterpret_cast<RGB8*>(mFrameBuffer->buffer());
    R32F * dpix = reinterpret_cast<R32F*>(mDepthBuffer->buffer());

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

void FragmentShaderSimulator::fragShader_Blue()
{
    static u8 g = 0;
    color.r = 255;
    color.g = (u8)uniform0;
    color.b = 255;
}

void FragmentShaderSimulator::fragShader_Raycast()
{
    glm::vec4 rayScreenPos(2.0f * gl_FragCoord.x / windowSize.x - 1.0f,
                           2.0f * gl_FragCoord.y / windowSize.y - 1.0f,
                           0.0f,
                           1.0f);

    glm::vec3 rayDirProj = glm::vec3(glm::normalize(projectionInv * rayScreenPos));

    glm::vec3 rayDir = mpRaycastCamera->direction() * rayDirProj;
    glm::vec3 rayPos = cameraPos;
    
    VoxelRef voxelRef;
    glm::vec3 normal;
    VoxelFace face;
    glm::vec2 faceUv;
    u32 hit = (u32)test_ray_voxel(&voxelRef, &normal, &zDepth, &face, &faceUv, voxelWorld, rayPos, rayDir, voxelRoot, 16);

    if (hit)
    {
        // LORRTEMP
        //LOG_INFO("HIT: pRi->searchOrder[pRi->searchIndex]: %d", pRi->searchOrder[pRi->searchIndex]);
        //LOG_INFO("normal: %f, %f, %f", normal.x, normal.y, normal.z);

        f32 intensity = glm::max(glm::dot(normal, lightDir), 0.0f);

        switch (face)
        {
        case VoxelFace::Left:
            color.r = 150;
            color.g = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.b = (u8)glm::min(150.0f, 150 * faceUv.y);
            break;
        case VoxelFace::Right:
            color.r = 255;
            color.g = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.b = (u8)glm::min(255.0f, 255 * faceUv.y);
            break;
        case VoxelFace::Bottom:
            color.r = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.g = 150;
            color.b = (u8)glm::min(150.0f, 150 * faceUv.y);
            break;
        case VoxelFace::Top:
            color.r = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.g = 255;
            color.b = (u8)glm::min(255.0f, 255 * faceUv.y);
            break;
        case VoxelFace::Back:
            color.r = (u8)glm::min(150.0f, 150 * faceUv.x);
            color.g = (u8)glm::min(150.0f, 150 * faceUv.y);
            color.b = 150;
            break;
        case VoxelFace::Front:
            color.r = (u8)glm::min(255.0f, 255 * faceUv.x);
            color.g = (u8)glm::min(255.0f, 255 * faceUv.y);
            color.b = 255;
            break;
        }

        //color.r = (u8)glm::max(intensity * 255, 10.0f);
        //color.g = (u8)glm::max(intensity * 255, 10.0f);
        //color.b = (u8)glm::max(intensity * 255, 10.0f);
    }
    else
    {
        color.r = 100;
        color.g = 125;
        color.b = 255;
        zDepth = -(FLT_MAX * 0.90f);
    }

/*
    color.r = (u8)(abs(rayDir.x) * 512.0f);
    color.g = (u8)(abs(rayDir.y) * 1024.0f);
    color.b = (u8)(abs(rayDir.z) * 255.0f);
    */
//    color.r = (u8)((rayDir.x + 1.0f) * 127.5f);
//    color.g = (u8)((rayDir.y + 1.0f) * 127.5f);

//    color.r = (u8)(gl_FragCoord.x / windowSize.x * 255.0f);
//    color.g = (u8)(gl_FragCoord.y / windowSize.y * 255.0f);
}

} // namespace gaen
