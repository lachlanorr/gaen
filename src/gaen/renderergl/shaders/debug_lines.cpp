//------------------------------------------------------------------------------
// debug_lines.cpp - Auto-generated shader from debug_lines.shd
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

#include "gaen/core/mem.h"
#include "gaen/renderergl/shaders/debug_lines.h"

namespace gaen
{
namespace shaders
{

static const char * kShaderCode_shv =
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec3 normal;\n"
    "layout(location = 2) in vec4 color;\n"
    "\n"
    "uniform mat4 mvp;\n"
    "\n"
    "out vec4 oicolor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    oicolor = color;\n"
    "    gl_Position = mvp * position;\n"
    "};\n"
    ; // kShaderCode_shv (END)

static const char * kShaderCode_shf =
    "in vec4 oicolor;\n"
    "out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    color = oicolor;\n"
    "};\n"
    ; // kShaderCode_shf (END)

Shader * debug_lines::construct()
{
    debug_lines * pShader = GNEW(kMEM_Renderer, debug_lines);

    // Program Codes
    pShader->mCodes[0].stage = GL_VERTEX_SHADER;
    pShader->mCodes[0].filename = "debug_lines.shv";
    pShader->mCodes[0].code = kShaderCode_shv;

    pShader->mCodes[1].stage = GL_FRAGMENT_SHADER;
    pShader->mCodes[1].filename = "debug_lines.shf";
    pShader->mCodes[1].code = kShaderCode_shf;

    // Uniforms
    //  These are computed at runtime in Shader::processProgram()

    // Attributes
    //  These are computed at runtime in Shader::processProgram()

    // Textures
    //  These are computed at runtime in Shader::processProgram()

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
