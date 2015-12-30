//------------------------------------------------------------------------------
// voxel_cast_frag.cpp - Auto-generated shader from voxel_cast_frag.shd
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
#include "renderergl/shaders/voxel_cast_frag.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "layout(location = 0) in vec4 vg_Position;\n"
    "layout(location = 1) in vec2 vg_RayScreenPos;\n"
    "\n"
    "out vec2 RayScreenPos;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vg_Position;\n"
    "    RayScreenPos = vg_RayScreenPos;\n"
    "};\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "//------------------------------------------------------------------------------\n"
    "// Constants\n"
    "//------------------------------------------------------------------------------\n"
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
    "const uint kMaxStackDepth = 32;\n"
    "//------------------------------------------------------------------------------\n"
    "// Constants (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Structs\n"
    "//------------------------------------------------------------------------------\n"
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
    "struct AABB_MinMax\n"
    "{\n"
    "    vec3 min; // min coord values along each axis\n"
    "    vec3 max; // max coord values along each axis\n"
    "};\n"
    "\n"
    "\n"
    "struct VoxelRecurseInfo\n"
    "{\n"
    "    VoxelRef voxelRef;\n"
    "    AABB_MinMax aabb;\n"
    "    uint searchOrder[8];\n"
    "    uint searchIndex;\n"
    "    bool hit;\n"
    "    uint face;\n"
    "    float entryDist;\n"
    "    float exitDist;\n"
    "    vec3 hitPosLoc;\n"
    "};\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Structs (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Static Data Structures\n"
    "//------------------------------------------------------------------------------\n"
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
    "//------------------------------------------------------------------------------\n"
    "\n"
    "// Based upon which voxel was hit, the order in which we should\n"
    "// search children for a hit.\n"
    "const uint kVoxelSearchOrder[6][4][8] =\n"
    "{\n"
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
    "//------------------------------------------------------------------------------\n"
    "// Static Data Structures (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Globals\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "in vec2 RayScreenPos;\n"
    "out vec3 color;\n"
    "\n"
    "// Voxel roots\n"
    "layout (location=0, rg32ui, binding=0) uniform uimageBuffer un_VoxelRoots;\n"
    "\n"
    "// Voxel world data\n"
    "layout (location=1, rg32ui, binding=1) uniform uimageBuffer un_VoxelData;\n"
    "\n"
    "layout (location=2) uniform uint un_VoxelRootCount;\n"
    "layout (location=3) uniform vec4 un_CameraDir;\n"
    "layout (location=4) uniform vec3 un_CameraPos;\n"
    "layout (location=5) uniform mat4 un_CameraProjectionInv;\n"
    "\n"
    "VoxelRecurseInfo stack[kMaxStackDepth];\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Globals (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Functions\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "VoxelRef unpack_voxel_ref(uvec2 pix)\n"
    "{\n"
    "    VoxelRef ref;\n"
    "\n"
    "    ref.type = pix.r & 0x3; // 2 bits\n"
    "    ref.material = (pix.r & 0xffc) >> 2; // 10 bits\n"
    "\n"
    "    ref.imageIdx = (pix.r & 0x7000) >> 12; // 3 bits\n"
    "    ref.voxelIdx = ((pix.g & 0x3f) << 17) | ((pix.r & 0xffff8000) >> 15); // 23 bits (17 in .r, 6 in .g)\n"
    "\n"
    "    ref.filledNeighbors = (pix.g & 0xffffffc0) >> 6; // 26 bits\n"
    "\n"
    "    return ref;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "VoxelRef extract_voxel_ref(uint voxelIdx, uint subVoxelIdx)\n"
    "{\n"
    "    int pixIdx = int(voxelIdx * kPixelsPerVoxel + subVoxelIdx);\n"
    "\n"
    "    uvec4 pix = imageLoad(un_VoxelData, pixIdx);\n"
    "\n"
    "    VoxelRef ref;\n"
    "\n"
    "    ref.type = pix.r & 0x3; // 2 bits\n"
    "    ref.material = (pix.r & 0xffc) >> 2; // 10 bits\n"
    "    \n"
    "    ref.imageIdx = (pix.r & 0x7000) >> 12; // 3 bits\n"
    "    ref.voxelIdx = ((pix.g & 0x3f) << 17) | ((pix.r & 0xffff8000) >> 15); // 23 bits (17 in .r, 6 in .g)\n"
    "\n"
    "    ref.filledNeighbors = (pix.g & 0xffffffc0) >> 6; // 26 bits\n"
    "\n"
    "    return ref;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "VoxelRoot extract_voxel_root(uint voxelRoot)\n"
    "{\n"
    "    int pixStart = int(voxelRoot * kPixelsPerVoxel);\n"
    "\n"
    "    uvec4 pix0 = imageLoad(un_VoxelRoots, pixStart);\n"
    "    uvec4 pix1 = imageLoad(un_VoxelRoots, pixStart + 1);\n"
    "    uvec4 pix2 = imageLoad(un_VoxelRoots, pixStart + 2);\n"
    "    uvec4 pix3 = imageLoad(un_VoxelRoots, pixStart + 3);\n"
    "    uvec4 pix4 = imageLoad(un_VoxelRoots, pixStart + 4);\n"
    "    uvec4 pix5 = imageLoad(un_VoxelRoots, pixStart + 5);\n"
    "    uvec4 pix6 = imageLoad(un_VoxelRoots, pixStart + 6);\n"
    "    uvec4 pix7 = imageLoad(un_VoxelRoots, pixStart + 7);\n"
    "\n"
    "    VoxelRoot root;\n"
    "\n"
    "    root.pos = vec3(uintBitsToFloat(pix0.r),\n"
    "                    uintBitsToFloat(pix0.g),\n"
    "                    uintBitsToFloat(pix1.r));\n"
    "\n"
    "    root.rot = mat3(uintBitsToFloat(pix1.g),\n"
    "                    uintBitsToFloat(pix2.r),\n"
    "                    uintBitsToFloat(pix2.g),\n"
    "\n"
    "                    uintBitsToFloat(pix3.r),\n"
    "                    uintBitsToFloat(pix3.g),\n"
    "                    uintBitsToFloat(pix4.r),\n"
    "\n"
    "                    uintBitsToFloat(pix4.g),\n"
    "                    uintBitsToFloat(pix5.r),\n"
    "                    uintBitsToFloat(pix5.g));\n"
    "\n"
    "    root.rad = uintBitsToFloat(pix6.r);\n"
    "    // pix6.g is padding in C struct\n"
    "\n"
    "    root.children = unpack_voxel_ref(uvec2(pix7));\n"
    "\n"
    "    return root;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "vec3 quat_multiply(vec4 qlhs, vec3 vrhs)\n"
    "{\n"
    "    float x2 = qlhs.x * 2.0;\n"
    "    float y2 = qlhs.y * 2.0;\n"
    "    float z2 = qlhs.z * 2.0;\n"
    "    float xx2 = qlhs.x * x2;\n"
    "    float yy2 = qlhs.y * y2;\n"
    "    float zz2 = qlhs.z * z2;\n"
    "    float xy2 = qlhs.x * y2;\n"
    "    float xz2 = qlhs.x * z2;\n"
    "    float yz2 = qlhs.y * z2;\n"
    "    float wx2 = qlhs.w * x2;\n"
    "    float wy2 = qlhs.w * y2;\n"
    "    float wz2 = qlhs.w * z2;\n"
    "\n"
    "    vec3 vres;\n"
    "    vres.x = (1.0 - (yy2 + zz2)) * vrhs.x + (xy2 - wz2) * vrhs.y + (xz2 + wy2) * vrhs.z;\n"
    "    vres.y = (xy2 + wz2) * vrhs.x + (1.0 - (xx2 + zz2)) * vrhs.y + (yz2 - wx2) * vrhs.z;\n"
    "    vres.z = (xz2 - wy2) * vrhs.x + (yz2 + wx2) * vrhs.y + (1.0 - (xx2 + yy2)) * vrhs.z;\n"
    "    return vres;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "vec4 uint_to_color(uint u)\n"
    "{\n"
    "    return vec4(float((u & 0xff000000) >> 24) / 255.0,\n"
    "                float((u & 0x00ff0000) >> 16) / 255.0,\n"
    "                float((u & 0x0000ff00) >>  8) / 255.0,\n"
    "                float(u & 0x000000ff) / 255.0);\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "AABB_MinMax aabb_from_rad(float rad)\n"
    "{\n"
    "    return AABB_MinMax(vec3(-rad), vec3(rad));\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "bool test_ray_box(out uint face,\n"
    "                  out float entryDist,\n"
    "                  out float exitDist,\n"
    "                  vec3 rayPos,\n"
    "                  vec3 invRayDir,\n"
    "                  AABB_MinMax aabb)\n"
    "{\n"
    "    vec3 tbot = invRayDir * (aabb.min - rayPos);\n"
    "    vec3 ttop = invRayDir * (aabb.max - rayPos);\n"
    "    vec3 tmin = min(ttop, tbot);\n"
    "    vec3 tmax = max(ttop, tbot);\n"
    "    vec2 t = max(tmin.xx, tmin.yz);\n"
    "    float t0 = max(t.x, t.y);\n"
    "    t = min(tmax.xx, tmax.yz);\n"
    "    float t1 = min(t.x, t.y);\n"
    "\n"
    "\n"
    "    // find the face that was hit, only one condition is true,\n"
    "    // and this eliminates branching.\n"
    "    // If none are true, we have no collision.\n"
    "\n"
    "    face = 0;\n"
    "    bool isHit = t0 <= t1;\n"
    "\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.x) && t0 == tbot.x) * 1);\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.x) && t0 == ttop.x) * 2);\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.y) && t0 == tbot.y) * 3);\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.y) && t0 == ttop.y) * 4);\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.z) && t0 == tbot.z) * 5);\n"
    "    face = max(face, uint(isHit && !isinf(invRayDir.z) && t0 == ttop.z) * 6);\n"
    "\n"
    "    entryDist = t0;\n"
    "    exitDist = t1;\n"
    "\n"
    "    return face != kFaceNone;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "VoxelRecurseInfo prep_stack_entry(VoxelRef voxelRef, AABB_MinMax aabb)\n"
    "{\n"
    "    VoxelRecurseInfo stackEntry;\n"
    "    stackEntry.voxelRef = voxelRef;\n"
    "    stackEntry.aabb = aabb;\n"
    "\n"
    "    stackEntry.searchOrder = kVoxelSearchOrder[0][0];\n"
    "    stackEntry.searchIndex = 0;\n"
    "    stackEntry.hit = false;\n"
    "    stackEntry.face = kFaceNone;\n"
    "    stackEntry.entryDist = 0.0;\n"
    "    stackEntry.exitDist = 0.0;\n"
    "    stackEntry.hitPosLoc = vec3(0.0);\n"
    "\n"
    "    return stackEntry;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "AABB_MinMax voxel_subspace(AABB_MinMax space, uint subIndex)\n"
    "{\n"
    "    // voxels are always cubes, so we only need one dimension's half\n"
    "    float dimHalf = 0.5 * (space.max.x - space.min.x);\n"
    "\n"
    "    // utilize the binary values of VoxelIndex to determine xyz offsets\n"
    "    vec3 offsets = vec3(((subIndex & 4) >> 2) * dimHalf,\n"
    "                        ((subIndex & 2) >> 1) * dimHalf,\n"
    "                        (subIndex & 1) * dimHalf);\n"
    "\n"
    "    AABB_MinMax ret;\n"
    "    ret.min = space.min + offsets;\n"
    "    ret.max = ret.min + vec3(dimHalf, dimHalf, dimHalf);\n"
    "    return ret;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "bool is_hit_within_voxel(uint face,\n"
    "                         vec3 hitPos,\n"
    "                         AABB_MinMax aabb)\n"
    "{\n"
    "    return (((face == kFaceLeft ||\n"
    "              face == kFaceRight) &&\n"
    "             hitPos.y > aabb.min.y &&\n"
    "             hitPos.y <= aabb.max.y &&\n"
    "             hitPos.z > aabb.min.z &&\n"
    "             hitPos.z <= aabb.max.z) ||\n"
    "\n"
    "            ((face == kFaceBottom ||\n"
    "              face == kFaceTop) &&\n"
    "             hitPos.x > aabb.min.x &&\n"
    "             hitPos.x <= aabb.max.x &&\n"
    "             hitPos.z > aabb.min.z &&\n"
    "             hitPos.z <= aabb.max.z) ||\n"
    "\n"
    "            ((face == kFaceBack ||\n"
    "              face == kFaceFront) &&\n"
    "             hitPos.x > aabb.min.x &&\n"
    "             hitPos.x <= aabb.max.x &&\n"
    "             hitPos.y > aabb.min.y &&\n"
    "             hitPos.y <= aabb.max.y));\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "void eval_voxel_hit(out uint searchOrder[8],\n"
    "                    out vec3 hitPos,\n"
    "                    uint face,\n"
    "                    float entryDist,\n"
    "                    float exitDist,\n"
    "                    vec3 rayPos,\n"
    "                    vec3 rayDir,\n"
    "                    AABB_MinMax aabb)\n"
    "{\n"
    "    hitPos = rayPos + rayDir * entryDist;\n"
    "    uint searchOrderIndex = 0;\n"
    "\n"
    "    // index into the face within kVoxelSearchOrder based on the fase we hit\n"
    "    uint searchBlock[4][8] = kVoxelSearchOrder[face - 1];\n"
    "\n"
    "    // We can skip the 0th entry, since we default to that if other 3 fail\n"
    "    AABB_MinMax subAabb0 = voxel_subspace(aabb, searchBlock[0][0]);\n"
    "    AABB_MinMax subAabb1 = voxel_subspace(aabb, searchBlock[1][0]);\n"
    "    AABB_MinMax subAabb2 = voxel_subspace(aabb, searchBlock[2][0]);\n"
    "    AABB_MinMax subAabb3 = voxel_subspace(aabb, searchBlock[3][0]);\n"
    "\n"
    "    // zero or one of following expressions are true,\n"
    "    // if zero are true, it means that subAbb0 was the hit, and we're already initialized to 0.\n"
    "    bool is_hit_within0 = is_hit_within_voxel(face, hitPos, subAabb0);\n"
    "    bool is_hit_within1 = is_hit_within_voxel(face, hitPos, subAabb1);\n"
    "    bool is_hit_within2 = is_hit_within_voxel(face, hitPos, subAabb2);\n"
    "    bool is_hit_within3 = is_hit_within_voxel(face, hitPos, subAabb3);\n"
    "\n"
    "    searchOrderIndex += uint(is_hit_within0) * 0;\n"
    "    searchOrderIndex += uint(is_hit_within1) * 1;\n"
    "    searchOrderIndex += uint(is_hit_within2) * 2;\n"
    "    searchOrderIndex += uint(is_hit_within3) * 3;\n"
    "\n"
    "    searchOrder = searchBlock[searchOrderIndex];\n"
    "}\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "vec2 calc_face_uv(uint face,\n"
    "                  vec3 hitPos,\n"
    "                  AABB_MinMax aabb)\n"
    "{\n"
    "    float aabbElems[6] = { aabb.min.x, aabb.min.y, aabb.min.z, aabb.max.x, aabb.max.y, aabb.max.z };\n"
    "\n"
    "    uint uHitElem = 0;\n"
    "    uHitElem = max(uHitElem, uint(face == kFaceLeft   || face == kFaceRight) * 2); // z\n"
    "    uHitElem = max(uHitElem, uint(face == kFaceBottom || face == kFaceTop)   * 0); // x\n"
    "    uHitElem = max(uHitElem, uint(face == kFaceBack   || face == kFaceFront) * 0); // x\n"
    "\n"
    "    uint vHitElem = 0;\n"
    "    vHitElem = max(vHitElem, uint(face == kFaceLeft   || face == kFaceRight) * 1); // y\n"
    "    vHitElem = max(vHitElem, uint(face == kFaceBottom || face == kFaceTop)   * 2); // z\n"
    "    vHitElem = max(vHitElem, uint(face == kFaceBack   || face == kFaceFront) * 1); // y\n"
    "\n"
    "\n"
    "    uint uAabbElem = 0;\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceLeft)   * 2);  // aabb.min.z\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceRight)  * 5);  // aabb.max.z\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceBottom) * 0);  // aabb.min.x\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceTop)    * 0);  // aabb.min.x\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceBack)   * 3);  // aabb.max.x\n"
    "    uAabbElem = max(uAabbElem, uint(face == kFaceFront)  * 0);  // aabb.min.x\n"
    "\n"
    "    uint vAabbElem = 0;\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceLeft)   * 1);  // aabb.min.y\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceRight)  * 1);  // aabb.min.y\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceBottom) * 2);  // aabb.min.z\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceTop)    * 5);  // aabb.max.z\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceBack)   * 1);  // aabb.min.y\n"
    "    vAabbElem = max(vAabbElem, uint(face == kFaceFront)  * 1);  // aabb.min.y\n"
    "\n"
    "    float aabbWidth = aabb.max.x - aabb.min.x;\n"
    "\n"
    "    return vec2(abs(hitPos[uHitElem] - aabbElems[uAabbElem]) / aabbWidth,\n"
    "                abs(hitPos[vHitElem] - aabbElems[vAabbElem]) / aabbWidth);\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "bool test_ray_voxel(out VoxelRef voxelRef,\n"
    "                    out vec3 normal,\n"
    "                    out float zDepth,\n"
    "                    out uint face,\n"
    "                    out vec2 faceUv,\n"
    "                    vec3 rayPos,\n"
    "                    vec3 rayDir,\n"
    "                    VoxelRoot root,\n"
    "                    uint maxDepth)\n"
    "{\n"
    "    // put ray into this voxel's space, so voxel is at 0,0,0 and 0,0,0 rotation\n"
    "    // from ray's perspective.\n"
    "\n"
    "    // Translate ray\n"
    "    vec3 rayPosLoc = rayPos - root.pos;\n"
    "\n"
    "    // Transpose of rotation matrix is the inverse\n"
    "    mat3 rotInv = transpose(root.rot);\n"
    "    vec3 rayDirLoc = rotInv * rayDir;\n"
    "    vec3 invRayDirLoc = 1.0 / rayDirLoc;\n"
    "\n"
    "    AABB_MinMax rootAabb = aabb_from_rad(root.rad);\n"
    "\n"
    "    // put voxel root on recurse stack before iteration begins\n"
    "    uint d = 0;\n"
    "    stack[d] = prep_stack_entry(root.children, rootAabb);\n"
    "\n"
    "    stack[d].hit = test_ray_box(stack[d].face,\n"
    "                                stack[d].entryDist,\n"
    "                                stack[d].exitDist,\n"
    "                                rayPosLoc,\n"
    "                                invRayDirLoc,\n"
    "                                stack[d].aabb);\n"
    "\n"
    "\n"
    "    if (!stack[d].hit || stack[d].voxelRef.type == kTerminalEmpty)\n"
    "    {\n"
    "        face = kFaceNone;\n"
    "        return false;\n"
    "    }\n"
    "\n"
    "\n"
    "    // else we hit, loop/recurse over children\n"
    "    while (true)\n"
    "    {\n"
    "        eval_voxel_hit(stack[d].searchOrder,\n"
    "                       stack[d].hitPosLoc,\n"
    "                       stack[d].face,\n"
    "                       stack[d].entryDist,\n"
    "                       stack[d].exitDist,\n"
    "                       rayPosLoc,\n"
    "                       rayDirLoc,\n"
    "                       stack[d].aabb);\n"
    "\n"
    "        if (stack[d].voxelRef.type == kTerminalFull)\n"
    "        {\n"
    "            voxelRef = stack[d].voxelRef;\n"
    "            normal = kNormals[stack[d].face];\n"
    "            zDepth = stack[d].entryDist;\n"
    "            face = stack[d].face;\n"
    "            faceUv = calc_face_uv(stack[d].face, stack[d].hitPosLoc, stack[d].aabb);\n"
    "            return true;\n"
    "        }\n"
    "        else\n"
    "        {\n"
    "            while (true)\n"
    "            {\n"
    "                if (stack[d].searchIndex >= 8)\n"
    "                {\n"
    "                    if (d > 0)\n"
    "                    {\n"
    "                        d--;\n"
    "                        break;\n"
    "                    }\n"
    "                    else\n"
    "                    {\n"
    "                        face = kFaceNone;\n"
    "                        return false;\n"
    "                    }\n"
    "                }\n"
    "                else\n"
    "                {\n"
    "                    VoxelRef childRef = extract_voxel_ref(stack[d].voxelRef.voxelIdx, stack[d].searchOrder[stack[d].searchIndex]);\n"
    "                    AABB_MinMax childAabb = voxel_subspace(stack[d].aabb,\n"
    "                                                           stack[d].searchOrder[stack[d].searchIndex]);\n"
    "                    stack[d].searchIndex++; // increment so if we recurse back here to this level we move past this one\n"
    "\n"
    "                    if (childRef.type == kTerminalEmpty)\n"
    "                    {\n"
    "                        continue;\n"
    "                    }\n"
    "\n"
    "                    VoxelRecurseInfo recInf = prep_stack_entry(childRef, childAabb);\n"
    "                    recInf.hit = test_ray_box(recInf.face,\n"
    "                                              recInf.entryDist,\n"
    "                                              recInf.exitDist,\n"
    "                                              rayPosLoc,\n"
    "                                              invRayDirLoc,\n"
    "                                              recInf.aabb);\n"
    "                    if (recInf.hit)\n"
    "                    {\n"
    "                        d++;\n"
    "\n"
    "                        stack[d] = recInf;\n"
    "                        break; // restart on parent for loop\n"
    "                    }\n"
    "                    else\n"
    "                    {\n"
    "                        continue;\n"
    "                    }\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "\n"
    "    face = kFaceNone;\n"
    "    return false;\n"
    "}\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "//------------------------------------------------------------------------------\n"
    "// Functions (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Main Program\n"
    "//------------------------------------------------------------------------------\n"
    "void main(void)\n"
    "{\n"
    "//    color = vec3((RayScreenPos + vec2(1.0, 1.0)) * vec2(0.5, 0.5), 0.0);\n"
    "//    return;\n"
    "\n"
    "    // Prepare our ray for this pixel\n"
    "    vec3 rayDirProj = normalize(un_CameraProjectionInv * vec4(RayScreenPos, 1.0, 1.0)).xyz;\n"
    "\n"
    "    vec3 rayDir = quat_multiply(un_CameraDir, rayDirProj);\n"
    "    vec3 rayPos = un_CameraPos;\n"
    "\n"
    "\n"
    "    VoxelRoot root = extract_voxel_root(0);\n"
    "\n"
    "    AABB_MinMax rootAabb = aabb_from_rad(root.rad);\n"
    "\n"
    "    VoxelRef voxelRef;\n"
    "    vec3 normal;\n"
    "    float zDepth;\n"
    "    uint face;\n"
    "    vec2 faceUv;\n"
    "\n"
    "    bool hit = test_ray_voxel(voxelRef, normal, zDepth, face, faceUv, un_CameraPos, rayDir, root, kMaxStackDepth);\n"
    "\n"
    "\n"
    "    //float entryDist;\n"
    "    //float exitDist;\n"
    "    //bool hit = test_ray_box(face, entryDist, exitDist, rayPos, 1.0 / rayDir, rootAabb);\n"
    "\n"
    "    \n"
    "    color = abs(kNormals[face]);\n"
    "}\n"
    "//------------------------------------------------------------------------------\n"
    "// Main Program (END)\n"
    "//------------------------------------------------------------------------------\n"
    ; // kShaderCode_shf (END)

Shader * voxel_cast_frag::construct()
{
    voxel_cast_frag * pShader = GNEW(kMEM_Renderer, voxel_cast_frag);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "voxel_cast_frag.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "voxel_cast_frag.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0x58bc2a47; /* HASH::un_CameraDir */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].location = 3;
    pShader->mUniforms[0].type = GL_FLOAT_VEC4;

    pShader->mUniforms[1].nameHash = 0xd7c7e9ca; /* HASH::un_CameraPos */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].location = 4;
    pShader->mUniforms[1].type = GL_FLOAT_VEC3;

    pShader->mUniforms[2].nameHash = 0x0aa42180; /* HASH::un_CameraProjectionInv */
    pShader->mUniforms[2].index = 2;
    pShader->mUniforms[2].location = 5;
    pShader->mUniforms[2].type = GL_FLOAT_MAT4;

    pShader->mUniforms[3].nameHash = 0x65b189d5; /* HASH::un_VoxelData */
    pShader->mUniforms[3].index = 3;
    pShader->mUniforms[3].location = 1;
    pShader->mUniforms[3].type = GL_UNSIGNED_INT_IMAGE_BUFFER;

    pShader->mUniforms[4].nameHash = 0xaa2d6892; /* HASH::un_VoxelRoots */
    pShader->mUniforms[4].index = 4;
    pShader->mUniforms[4].location = 0;
    pShader->mUniforms[4].type = GL_UNSIGNED_INT_IMAGE_BUFFER;


    // Attributes
    pShader->mAttributes[0].nameHash = 0xc93a0aac; /* HASH::vg_Position */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 0;
    pShader->mAttributes[0].type = GL_FLOAT_VEC4;

    pShader->mAttributes[1].nameHash = 0x27f20809; /* HASH::vg_RayScreenPos */
    pShader->mAttributes[1].index = 1;
    pShader->mAttributes[1].location = 1;
    pShader->mAttributes[1].type = GL_FLOAT_VEC2;


    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;

    return pShader;
}

} // namespace shaders
} // namespace gaen
