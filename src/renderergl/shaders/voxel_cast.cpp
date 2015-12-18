//------------------------------------------------------------------------------
// voxel_cast.cpp - Auto-generated shader from voxel_cast.shd
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

#include "core/mem.h"
#include "renderergl/shaders/voxel_cast.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shc =
    "\n"
    "layout (local_size_x = 16, local_size_y = 16) in;\n"
    "\n"
    "// frameBuffer output\n"
    "layout (rgba8, binding=0) uniform image2D frameBuffer;\n"
    "\n"
    "// Voxel world data\n"
    "layout (rg32ui, binding=1) uniform uimageBuffer voxelData;\n"
    "\n"
    "//layout (binding=2) uniform\n"
    "\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    ivec2 coord = ivec2(gl_WorkGroupSize.x * gl_NumWorkGroups.x,\n"
    "                        gl_WorkGroupSize.y * gl_NumWorkGroups.y);\n"
    "\n"
    "    vec2 imageSize = vec2(coord);\n"
    "\n"
    "    vec4 color = vec4(gl_GlobalInvocationID.xy / imageSize, 0.0, 0.0);\n"
    "\n"
    "    uvec4 voxel = imageLoad(voxelData, int(gl_GlobalInvocationID.y * coord.x + gl_GlobalInvocationID.x));\n"
    "\n"
    "    imageStore(frameBuffer,\n"
    "               ivec2(gl_GlobalInvocationID.xy),\n"
    "               vec4(/*1.0, 0.0,*/ vec2(gl_WorkGroupID.xy) / vec2(gl_NumWorkGroups),\n"
    "                    0.0, 0.0));\n"
    "\n"
    "\n"
    "//    imageStore(frameBuffer,\n"
    "//               ivec2(gl_GlobalInvocationID.xy),\n"
    "//               vec4(voxel.xy, 0.0, 1.0));\n"
    "////               vec4(1.0, 1.0, 1.0, 1.0));\n"
    "    \n"
    "}\n"
    ; // kShaderCode_shc (END)

Shader * voxel_cast::construct()
{
    voxel_cast * pShader = GNEW(kMEM_Renderer, voxel_cast);

    // Program Codes
    pShader->mCodes[0].stage = GL_COMPUTE_SHADER;
    pShader->mCodes[0].filename = "voxel_cast.shc";
    pShader->mCodes[0].code = kShaderCode_shc;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0x50c55e30; /* HASH::frameBuffer */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].type = GL_IMAGE_2D;

    pShader->mUniforms[1].nameHash = 0x8b49e81f; /* HASH::voxelData */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].type = GL_UNSIGNED_INT_IMAGE_BUFFER;


    // Attributes

    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;

    return pShader;
}

} // namespace shaders
} // namespace gaen
