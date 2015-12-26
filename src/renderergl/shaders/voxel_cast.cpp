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
    "const uint kImageCount = 16; // max is 16 since 4 bits in VoxelRef for imageIdx\n"
    "const uint kImageSize = 8192;\n"
    "const uint kPixelSize = 8; // RG32U\n"
    "const uint kPixelsPerVoxel = 8; // 8 VoxelRefs per voxel, 8 bytes each, pixels are 8\n"
    "const uint kVoxelRefsPerPixel = 1; // 1 VoxelRef per RG32U pixel (8 bytes each)\n"
    "const uint kMaxVoxelIndex = 8388608; // 2 ** 23, we have 23 bits for index, which fits perfectly into 8192x8192x8byte image\n"
    "\n"
    "// Sub voxels\n"
    "const uint kLeftBottomBack   = 0;  // 000\n"
    "const uint kLeftBottomFront  = 1;  // 001\n"
    "const uint kLeftTopBack      = 2;  // 010\n"
    "const uint kLeftTopFront     = 3;  // 011\n"
    "const uint kRightBottomBack  = 4;  // 100\n"
    "const uint kRightBottomFront = 5;  // 101\n"
    "const uint kRightTopBack     = 6;  // 110\n"
    "const uint kRightTopFront    = 7;  // 111\n"
    "\n"
    "\n"
    "// Voxel faces\n"
    "const uint kFaceNone   = 0;\n"
    "const uint kFaceLeft   = 1;\n"
    "const uint kFaceRight  = 2;\n"
    "const uint kFaceBottom = 3;\n"
    "const uint kFaceTop    = 4;\n"
    "const uint kFaceBack   = 5;\n"
    "const uint kFaceFront  = 6;\n"
    "\n"
    "\n"
    "// Voxel types\n"
    "const uint kTerminalEmpty = 0;\n"
    "const uint kTerminalFull  = 1;\n"
    "const uint kNonTerminal   = 2;\n"
    "\n"
    "\n"
    "struct VoxelRef\n"
    "{\n"
    "    uint type; // see Voxel types consts\n"
    "    uint material;\n"
    "    uint filledNeighbors;\n"
    "    uint imageIdx;\n"
    "    uint voxelIdx;\n"
    "};\n"
    "\n"
    "\n"
    "struct VoxelRoot\n"
    "{\n"
    "    mat3 rot;\n"
    "    vec3 pos;\n"
    "    VoxelRef children;\n"
    "    float rad;\n"
    "};\n"
    "\n"
    "\n"
    "// Normals from faces in local coords\n"
    "const vec3 kNormals[7] = { vec3( 0.0,  0.0,  0.0),\n"
    "                           vec3(-1.0,  0.0,  0.0),   // left\n"
    "                           vec3( 1.0,  0.0,  0.0),   // right\n"
    "                           vec3( 0.0, -1.0,  0.0),   // bottom\n"
    "                           vec3( 0.0,  1.0,  0.0),   // top\n"
    "                           vec3( 0.0,  0.0, -1.0),   // back\n"
    "                           vec3( 0.0,  0.0,  1.0) }; // front\n"
    "\n"
    "// Based upon which voxel was hit, the order in which we should\n"
    "// search children for a hit.\n"
    "const uint kVoxelSearchOrder[6][4][8] =\n"
    "{ \n"
    "    { // left\n"
    "        { // LeftBottomBack\n"
    "            kLeftBottomBack,\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopBack,\n"
    "            kLeftTopFront,\n"
    "\n"
    "            kRightBottomBack,\n"
    "            kRightBottomFront,\n"
    "            kRightTopBack,\n"
    "            kRightTopFront\n"
    "        },\n"
    "\n"
    "        { // LeftBottomFront\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopBack,\n"
    "\n"
    "            kRightBottomFront,\n"
    "            kRightTopFront,\n"
    "            kRightBottomBack,\n"
    "            kRightTopBack\n"
    "        },\n"
    "\n"
    "        { // LeftTopFront\n"
    "            kLeftTopFront,\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomFront,\n"
    "            kLeftBottomBack,\n"
    "\n"
    "            kRightTopFront,\n"
    "            kRightTopBack,\n"
    "            kRightBottomFront,\n"
    "            kRightBottomBack\n"
    "        },\n"
    "\n"
    "        { // LeftTopBack\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomFront,\n"
    "\n"
    "            kRightTopBack,\n"
    "            kRightBottomBack,\n"
    "            kRightTopFront,\n"
    "            kRightBottomFront\n"
    "        }\n"
    "    },\n"
    "\n"
    "    { // right\n"
    "        { // RightBottomFront\n"
    "            kRightBottomFront,\n"
    "            kRightBottomBack,\n"
    "            kRightTopFront,\n"
    "            kRightTopBack,\n"
    "\n"
    "            kLeftBottomFront,\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopFront,\n"
    "            kLeftTopBack\n"
    "        },\n"
    "\n"
    "        { // RightBottomBack\n"
    "            kRightBottomBack,\n"
    "            kRightTopBack,\n"
    "            kRightBottomFront,\n"
    "            kRightTopFront,\n"
    "\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopFront\n"
    "        },\n"
    "\n"
    "        { // RightTopBack\n"
    "            kRightTopBack,\n"
    "            kRightTopFront,\n"
    "            kRightBottomBack,\n"
    "            kRightBottomFront,\n"
    "\n"
    "            kLeftTopBack,\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomBack,\n"
    "            kLeftBottomFront\n"
    "        },\n"
    "\n"
    "        { // RightTopFront\n"
    "            kRightTopFront,\n"
    "            kRightBottomFront,\n"
    "            kRightTopBack,\n"
    "            kRightBottomBack,\n"
    "\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomBack\n"
    "        }\n"
    "    },\n"
    "\n"
    "    { // bottom\n"
    "        { // LeftBottomBack\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomFront,\n"
    "\n"
    "            kLeftTopBack,\n"
    "            kRightTopBack,\n"
    "            kLeftTopFront,\n"
    "            kRightTopFront\n"
    "        },\n"
    "\n"
    "        { // RightBottomBack\n"
    "            kRightBottomBack,\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomBack,\n"
    "            kLeftBottomFront,\n"
    "\n"
    "            kRightTopBack,\n"
    "            kRightTopFront,\n"
    "            kLeftTopBack,\n"
    "            kLeftTopFront\n"
    "        },\n"
    "\n"
    "        { // RightBottomFront\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomBack,\n"
    "\n"
    "            kRightTopFront,\n"
    "            kLeftTopFront,\n"
    "            kRightTopBack,\n"
    "            kLeftTopBack\n"
    "        },\n"
    "\n"
    "        { // LeftBottomFront\n"
    "            kLeftBottomFront,\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomFront,\n"
    "            kRightBottomBack,\n"
    "\n"
    "            kLeftTopFront,\n"
    "            kLeftTopBack,\n"
    "            kRightTopFront,\n"
    "            kRightTopBack\n"
    "        }\n"
    "    },\n"
    "\n"
    "    { // top\n"
    "        { // LeftTopFront\n"
    "            kLeftTopFront,\n"
    "            kRightTopFront,\n"
    "            kLeftTopBack,\n"
    "            kRightTopBack,\n"
    "\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomBack\n"
    "        },\n"
    "\n"
    "        { // RightTopFront\n"
    "            kRightTopFront,\n"
    "            kRightTopBack,\n"
    "            kLeftTopFront,\n"
    "            kLeftTopBack,\n"
    "\n"
    "            kRightBottomFront,\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomFront,\n"
    "            kLeftBottomBack\n"
    "        },\n"
    "\n"
    "        { // RightTopBack\n"
    "            kRightTopBack,\n"
    "            kLeftTopBack,\n"
    "            kRightTopFront,\n"
    "            kLeftTopFront,\n"
    "\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomFront\n"
    "        },\n"
    "\n"
    "        { // LeftTopBack\n"
    "            kLeftTopBack,\n"
    "            kLeftTopFront,\n"
    "            kRightTopBack,\n"
    "            kRightTopFront,\n"
    "\n"
    "            kLeftBottomBack,\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomBack,\n"
    "            kRightBottomFront\n"
    "        }\n"
    "    },\n"
    "\n"
    "    { // back\n"
    "        { // RightBottomBack\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomBack,\n"
    "            kRightTopBack,\n"
    "            kLeftTopBack,\n"
    "\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomFront,\n"
    "            kRightTopFront,\n"
    "            kLeftTopFront\n"
    "        },\n"
    "\n"
    "        { // LeftBottomBack\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopBack,\n"
    "            kRightBottomBack,\n"
    "            kRightTopBack,\n"
    "\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopFront,\n"
    "            kRightBottomFront,\n"
    "            kRightTopFront\n"
    "        },\n"
    "\n"
    "        { // LeftTopBack\n"
    "            kLeftTopBack,\n"
    "            kRightTopBack,\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomBack,\n"
    "\n"
    "            kLeftTopFront,\n"
    "            kRightTopFront,\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomFront\n"
    "        },\n"
    "\n"
    "        { // RightTopBack\n"
    "            kRightTopBack,\n"
    "            kRightBottomBack,\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomBack,\n"
    "\n"
    "            kRightTopFront,\n"
    "            kRightBottomFront,\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomFront\n"
    "        }\n"
    "    },\n"
    "\n"
    "    { // front\n"
    "        { // LeftBottomFront\n"
    "            kLeftBottomFront,\n"
    "            kRightBottomFront,\n"
    "            kLeftTopFront,\n"
    "            kRightTopFront,\n"
    "\n"
    "            kLeftBottomBack,\n"
    "            kRightBottomBack,\n"
    "            kLeftTopBack,\n"
    "            kRightTopBack\n"
    "        },\n"
    "\n"
    "        { // RightBottomFront\n"
    "            kRightBottomFront,\n"
    "            kRightTopFront,\n"
    "            kLeftBottomFront,\n"
    "            kLeftTopFront,\n"
    "\n"
    "            kRightBottomBack,\n"
    "            kRightTopBack,\n"
    "            kLeftBottomBack,\n"
    "            kLeftTopBack\n"
    "        },\n"
    "\n"
    "        { // RightTopFront\n"
    "            kRightTopFront,\n"
    "            kLeftTopFront,\n"
    "            kRightBottomFront,\n"
    "            kLeftBottomFront,\n"
    "\n"
    "            kRightTopBack,\n"
    "            kLeftTopBack,\n"
    "            kRightBottomBack,\n"
    "            kLeftBottomBack\n"
    "        },\n"
    "\n"
    "        { // LeftTopFront\n"
    "            kLeftTopFront,\n"
    "            kLeftBottomFront,\n"
    "            kRightTopFront,\n"
    "            kRightBottomFront,\n"
    "\n"
    "            kLeftTopBack,\n"
    "            kLeftBottomBack,\n"
    "            kRightTopBack,\n"
    "            kRightBottomBack\n"
    "        }\n"
    "    }\n"
    "};\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "layout (local_size_x = 16, local_size_y = 16) in;\n"
    "\n"
    "// un_FrameBuffer output\n"
    "layout (rgba8, binding=0) uniform image2D un_FrameBuffer;\n"
    "\n"
    "// Voxel world data\n"
    "layout (rg32ui, binding=1) uniform uimageBuffer un_VoxelData;\n"
    "\n"
    "//layout (binding=2) uniform\n"
    "\n"
    "\n"
    "\n"
    "\n"
    "VoxelRef unpack_voxel_ref(uvec2 pix)\n"
    "{\n"
    "    VoxelRef ref;\n"
    "\n"
    "    ref.type = pix.r & 0x3; // 2 bits\n"
    "    ref.material = (pix.r & 0x3fffc) >> 2; // 16 bits\n"
    "    ref.filledNeighbors = (pix.r & 0xfc0000) >> 18; // 6 bits\n"
    "    // padding - 8 bits\n"
    "    \n"
    "    ref.imageIdx = (pix.g & 0xf); // 4 bits\n"
    "    ref.voxelIdx = (pix.g & 0x7fffff0) >> 4; // 23 bits, top order of second 8 byte pixel\n"
    "    // padding - 5 bits\n"
    "\n"
    "    return ref;\n"
    "}\n"
    "\n"
    "VoxelRoot extract_voxel_root(uint voxelRoot)\n"
    "{\n"
    "    int pixStart = int(voxelRoot * kPixelsPerVoxel);\n"
    "\n"
    "    uvec4 pix0 = imageLoad(un_VoxelData, pixStart);\n"
    "    uvec4 pix1 = imageLoad(un_VoxelData, pixStart + 1);\n"
    "    uvec4 pix2 = imageLoad(un_VoxelData, pixStart + 2);\n"
    "    uvec4 pix3 = imageLoad(un_VoxelData, pixStart + 3);\n"
    "    uvec4 pix4 = imageLoad(un_VoxelData, pixStart + 4);\n"
    "    uvec4 pix5 = imageLoad(un_VoxelData, pixStart + 5);\n"
    "    uvec4 pix6 = imageLoad(un_VoxelData, pixStart + 6);\n"
    "    uvec4 pix7 = imageLoad(un_VoxelData, pixStart + 7);\n"
    "\n"
    "    VoxelRoot root;\n"
    "\n"
    "    root.pos = vec3(uintBitsToFloat(pix0.r),\n"
    "                    uintBitsToFloat(pix0.g),\n"
    "                    uintBitsToFloat(pix1.r));\n"
    "                    \n"
    "    root.rot = mat3(uintBitsToFloat(pix1.g),\n"
    "                    uintBitsToFloat(pix2.r),\n"
    "                    uintBitsToFloat(pix2.g),\n"
    "\n"
    "                    uintBitsToFloat(pix3.r),\n"
    "                    uintBitsToFloat(pix3.g),\n"
    "                    uintBitsToFloat(pix4.r),\n"
    "                    \n"
    "                    uintBitsToFloat(pix4.g),\n"
    "                    uintBitsToFloat(pix5.r),\n"
    "                    uintBitsToFloat(pix5.g));\n"
    "\n"
    "    root.rad = uintBitsToFloat(pix6.r);\n"
    "    // pix6.g is padding in C struct\n"
    "\n"
    "    root.children = unpack_voxel_ref(pix7.rg);\n"
    "\n"
    "    return root;\n"
    "}\n"
    "\n"
    "\n"
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
    "    uvec4 voxel = imageLoad(un_VoxelData, int(gl_GlobalInvocationID.y * coord.x + gl_GlobalInvocationID.x));\n"
    "\n"
    "//    imageStore(un_FrameBuffer,\n"
    "//               ivec2(gl_GlobalInvocationID.xy),\n"
    "//               vec4(/*1.0, 0.0,*/ vec2(gl_WorkGroupID.xy) / vec2(gl_NumWorkGroups),\n"
    "//                    0.0, 0.0));\n"
    "\n"
    "\n"
    "    imageStore(un_FrameBuffer,\n"
    "               ivec2(gl_GlobalInvocationID.xy),\n"
    "               vec4(voxel.xy, 0.0, 1.0));\n"
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
    pShader->mUniforms[0].nameHash = 0x7b669d1e; /* HASH::un_FrameBuffer */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].type = GL_IMAGE_2D;

    pShader->mUniforms[1].nameHash = 0x65b189d5; /* HASH::un_VoxelData */
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
