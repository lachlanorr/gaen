//------------------------------------------------------------------------------
// voxel27.cpp - Auto-generated shader from voxel27.shd
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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
#include "renderergl/shaders/voxel27.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "\n"
    "const float kLodGridSize[5] = {\n"
    "    81.0,\n"
    "    27.0,\n"
    "    9.0,\n"
    "    3.0,\n"
    "    1.0\n"
    "};\n"
    "\n"
    "const float kLevelSize[16] = {\n"
    "    1.0 / 27.0,\n"
    "    1.0 / 9.0,\n"
    "    1.0 / 3.0,\n"
    "    1.0,\n"
    "    3.0,\n"
    "    9.0,\n"
    "    27.0,\n"
    "    81.0,\n"
    "    243.0,\n"
    "    729.0,\n"
    "    2187.0,\n"
    "    6651.0,\n"
    "    19683.0,\n"
    "    59049.0,\n"
    "    177147.0,\n"
    "    531441.0\n"
    "};\n"
    "\n"
    "const float kLevelDiagonalSize[16] = {\n"
    "    kLevelSize[0]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[1]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[2]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[3]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[4]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[5]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[6]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[7]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[8]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[9]  * 4 * sqrt(3.0),\n"
    "    kLevelSize[10] * 4 * sqrt(3.0),\n"
    "    kLevelSize[11] * 4 * sqrt(3.0),\n"
    "    kLevelSize[12] * 4 * sqrt(3.0),\n"
    "    kLevelSize[13] * 4 * sqrt(3.0),\n"
    "    kLevelSize[14] * 4 * sqrt(3.0),\n"
    "    kLevelSize[15] * 4 * sqrt(3.0)\n"
    "};\n"
    "\n"
    "\n"
    "struct Voxel27PointData\n"
    "{\n"
    "    vec3 gridCoords;\n"
    "    uint voxelLevel;\n"
    "    uint material;\n"
    "};\n"
    "\n"
    "struct AABB_MinMax\n"
    "{\n"
    "    vec3 min;\n"
    "    vec3 max;\n"
    "};\n"
    "\n"
    "\n"
    "\n"
    "in uint va_VoxelPoint;\n"
    "\n"
    "uniform uint un_LodBasis; // 0 - 11 value indicating base level of LOD\n"
    "uniform vec3 un_VoxelMin; // min point of voxel aabb\n"
    "uniform mat4 un_MVP;\n"
    "\n"
    "out AABB_MinMax aabb;\n"
    "out vec3 vColor;\n"
    "\n"
    "\n"
    "Voxel27PointData extract_point_data(uint voxelPoint)\n"
    "{\n"
    "    return Voxel27PointData(vec3(float(voxelPoint & 0x7f),\n"
    "                                 float((voxelPoint & 0x3f80) >> 7),\n"
    "                                 float((voxelPoint & 0x1fc000) >> 14)),\n"
    "                            (voxelPoint & 0x1e00000) >> 21,\n"
    "                            (voxelPoint & 0xfe000000) >> 25);\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    Voxel27PointData vpd = extract_point_data(va_VoxelPoint);\n"
    "\n"
    "    uint relativeLod = vpd.voxelLevel - un_LodBasis;\n"
    "    float lodGridSize = kLodGridSize[relativeLod];\n"
    "\n"
    "    //vec4 position = vec4(un_VoxelMin + (vpd.gridCoords / lodGridSize), 1.0);\n"
    "    //vec4 position = vec4((vpd.gridCoords / lodGridSize), 1.0);\n"
    "    vec4 position = vec4((vpd.gridCoords / 81.0), 1.0);\n"
    "    vColor = vpd.gridCoords;\n"
    "\n"
    "    gl_PointSize = 4;\n"
    "    \n"
    "    gl_Position = un_MVP * position;\n"
    "};\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Globals\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "in vec3 vColor;\n"
    "out vec3 color;\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Globals (END)\n"
    "//------------------------------------------------------------------------------\n"
    "\n"
    "\n"
    "//------------------------------------------------------------------------------\n"
    "// Main Program\n"
    "//------------------------------------------------------------------------------\n"
    "void main(void)\n"
    "{\n"
    "//    color = vec3(1.0, 0.0, 1.0);\n"
    "      color = vColor / vec3(255.0);\n"
    "}\n"
    "//------------------------------------------------------------------------------\n"
    "// Main Program (END)\n"
    "//------------------------------------------------------------------------------\n"
    ; // kShaderCode_shf (END)

Shader * voxel27::construct()
{
    voxel27 * pShader = GNEW(kMEM_Renderer, voxel27);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "voxel27.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "voxel27.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0x29618670; /* HASH::un_MVP */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].location = 0;
    pShader->mUniforms[0].type = GL_FLOAT_MAT4;


    // Attributes
    pShader->mAttributes[0].nameHash = 0xc9f0e16f; /* HASH::va_VoxelPoint */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 0;
    pShader->mAttributes[0].type = GL_UNSIGNED_INT;


    // Textures

    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;
    pShader->mTextureCount = kTextureCount;
    pShader->mpTextures = nullptr;

    return pShader;
}

} // namespace shaders
} // namespace gaen
