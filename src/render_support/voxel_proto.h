//------------------------------------------------------------------------------
// voxel_proto.h - CPU side voxel support
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

#ifndef GAEN_RENDER_SUPPORT_VOXEL_PROTO_H
#define GAEN_RENDER_SUPPORT_VOXEL_PROTO_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/List.h"
#include "render_support/RaycastCamera.h"
#include "render_support/render_objects.h"
#include "render_support/voxel.h"

namespace gaen
{

class ComputeShaderSimulator
{
public:
    ~ComputeShaderSimulator();

    void init(glm::uvec3 workGroupSize,
              glm::uvec3 numWorkGroups);

    const u8 * frameBuffer() { return un_FrameBuffer->buffer(); }

    void render(const RaycastCamera & camera, const List<kMEM_Renderer, DirectionalLight> & lights);

private:
    void compShader_Test();
    void compShader_Raycast_gpu();

    VoxelWorld mVoxelWorld;

    // uniforms
    glm::vec3 un_CameraPos;
    glm::quat un_CameraDir;
    glm::mat4 un_CameraProjectionInv;

    ImageBuffer * un_FrameBuffer = nullptr;
    const ImageBuffer * un_VoxelData = nullptr;

    const ImageBuffer * un_VoxelRoots = nullptr;
    u32 un_VoxelRootCount;

    // glsl variables
    glm::uvec3 gl_WorkGroupSize;
    glm::uvec3 gl_NumWorkGroups;

    glm::uvec3 gl_LocalInvocationID;
    glm::uvec3 gl_WorkGroupID;

    glm::uvec3 gl_GlobalInvocationID;
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

    void render(const RaycastCamera & camera, const List<kMEM_Renderer, DirectionalLight> & lights);
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
    glm::mat4 projectionInv;
    glm::vec3 cameraPos;
    glm::vec3 lightDir;
    glm::vec4 lightColor;
    glm::vec2 windowSize;
    f32 nearZ;
    f32 farZ;

    // LORRTODO - temp voxel stuff - should move to game engine proper
    VoxelRoot voxelRoot;

    VoxelWorld voxelWorld;

};

} // namespace gaen

#endif // #ifndef GAEN_RENDER_SUPPORT_VOXEL_H

