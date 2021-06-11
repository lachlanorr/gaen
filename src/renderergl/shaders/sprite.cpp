//------------------------------------------------------------------------------
// sprite.cpp - Auto-generated shader from sprite.shd
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
#include "renderergl/shaders/sprite.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "layout(location = 0) in vec4 pos;\n"
    "\n"
    "layout(location = 1) uniform mat4 mvp;\n"
    "\n"
    "out vec2 oiuv;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mvp * vec4(pos.xy, 0.0, 1.0);\n"
    "    oiuv = pos.zw;\n"
    "};\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "in vec2 oiuv;\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "layout(location=0, binding=0) uniform sampler2D diffuse;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = texture(diffuse, oiuv).rgba;\n"
    "};\n"
    ; // kShaderCode_shf (END)

Shader * sprite::construct()
{
    sprite * pShader = GNEW(kMEM_Renderer, sprite);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "sprite.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "sprite.shf";
    pShader->mCodes[1].code = kShaderCode_shf;


    // Uniforms
    pShader->mUniforms[0].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mUniforms[0].index = 0;
    pShader->mUniforms[0].location = 0;
    pShader->mUniforms[0].type = GL_SAMPLER_2D;

    pShader->mUniforms[1].nameHash = 0xdd88a90e; /* HASH::mvp */
    pShader->mUniforms[1].index = 1;
    pShader->mUniforms[1].location = 1;
    pShader->mUniforms[1].type = GL_FLOAT_MAT4;


    // Attributes
    pShader->mAttributes[0].nameHash = 0x54336489; /* HASH::pos */
    pShader->mAttributes[0].index = 0;
    pShader->mAttributes[0].location = 0;
    pShader->mAttributes[0].type = GL_FLOAT_VEC4;


    // Textures
    pShader->mTextures[0].nameHash = 0x546e2a3d; /* HASH::diffuse */
    pShader->mTextures[0].index = 0;
    pShader->mTextures[0].location = 0;
    pShader->mTextures[0].type = GL_SAMPLER_2D;


    // Set base Shader members to our arrays and counts
    pShader->mCodeCount = kCodeCount;
    pShader->mpCodes = pShader->mCodes;
    pShader->mUniformCount = kUniformCount;
    pShader->mpUniforms = pShader->mUniforms;
    pShader->mAttributeCount = kAttributeCount;
    pShader->mpAttributes = pShader->mAttributes;
    pShader->mTextureCount = kTextureCount;
    pShader->mpTextures = pShader->mTextures;

    return pShader;
}

} // namespace shaders
} // namespace gaen
