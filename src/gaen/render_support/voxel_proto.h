//------------------------------------------------------------------------------
// voxel_proto.h - CPU side voxel support
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

#ifndef GAEN_RENDER_SUPPORT_VOXEL_PROTO_H
#define GAEN_RENDER_SUPPORT_VOXEL_PROTO_H

#include "gaen/math/vec2.h"
#include "gaen/math/vec3.h"
#include "gaen/math/vec4.h"
#include "gaen/math/mat4.h"

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"
#include "gaen/core/List.h"
#include "gaen/render_support/RaycastCamera.h"
#include "gaen/render_support/render_objects.h"
#include "gaen/render_support/voxel.h"

namespace gaen
{

class ComputeShaderSimulator
{
public:
    ~ComputeShaderSimulator();

    void init(uvec3 workGroupSize,
              uvec3 numWorkGroups);

    const u8 * frameBuffer() { return un_FrameBuffer->buffer(); }

    void render(const RaycastCamera & camera, const List<kMEM_Renderer, Light> & lights);

private:
    void compShader_Test();
    void compShader_Raycast_gpu();

    VoxelWorld mVoxelWorld;

    // uniforms
    vec3 un_CameraPos;
    quat un_CameraDir;
    mat4 un_CameraProjectionInv;

    ImageBuffer * un_FrameBuffer = nullptr;
    const ImageBuffer * un_VoxelData = nullptr;

    const ImageBuffer * un_VoxelRoots = nullptr;
    u32 un_VoxelRootCount;

    // glsl variables
    uvec3 gl_WorkGroupSize;
    uvec3 gl_NumWorkGroups;

    uvec3 gl_LocalInvocationID;
    uvec3 gl_WorkGroupID;

    uvec3 gl_GlobalInvocationID;
    u32 gl_LocalInvocationIndex;
};



class FragmentShaderSimulator
{
public:
    enum ImageIndex
    {
        kIMID_Objects = 0

//        kIMID_
    };
    
    FragmentShaderSimulator();
    ~FragmentShaderSimulator();

    void init(u32 outputImageSize, RaycastCamera * pRaycastCamera);
    const u8 * frameBuffer() { return mFrameBuffer->buffer(); }

    void render(const RaycastCamera & camera, const List<kMEM_Renderer, Light> & lights);
private:
    void fragShader_Blue();
    void fragShader_Raycast();

    bool mIsInit = false;
    ImageBuffer * mFrameBuffer;
    ImageBuffer * mDepthBuffer;
    ImageBuffer * mDepthBufferBlank;

    RaycastCamera * mpRaycastCamera;

    // GLSL "globals"
    struct ScreenCoords
    {
        u32 x;
        u32 y;
        u32 z;
    };

    RGB8 color; // frag output shader
    f32 zDepth;
    ScreenCoords gl_FragCoord;

    u32 uniform0 = 0;

    // camera stuff
    mat4 projectionInv;
    vec3 cameraPos;
    vec3 lightDir;
    vec3 lightColor;
    vec2 windowSize;
    f32 nearZ;
    f32 farZ;

    // LORRTODO - temp voxel stuff - should move to game engine proper
    VoxelRoot voxelRoot;

    VoxelWorld voxelWorld;

};

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_VOXEL_H

