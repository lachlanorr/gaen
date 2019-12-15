//------------------------------------------------------------------------------
// voxchar.h - Auto-generated shader from voxchar.shd
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
#ifndef GAEN_RENDERERGL_SHADERS_VOXCHAR_H
#define GAEN_RENDERERGL_SHADERS_VOXCHAR_H
#include "renderergl/shaders/Shader.h"
namespace gaen
{
namespace shaders
{

class voxchar : Shader
{
public:
    static Shader * construct();

private:
    voxchar() : Shader(0xd4208c92 /* HASH::voxchar */) {}

    static const u32 kCodeCount = 2;
    static const u32 kUniformCount = 8;
    static const u32 kAttributeCount = 4;
    static const u32 kTextureCount = 2;

    Shader::ShaderCode mCodes[kCodeCount];
    Shader::VariableInfo mUniforms[kUniformCount];
    Shader::VariableInfo mAttributes[kAttributeCount];
    Shader::VariableInfo mTextures[kTextureCount];
}; // class voxchar

} // namespace shaders
} // namespace gaen
#endif // #ifdef GAEN_RENDERERGL_SHADERS_VOXCHAR_H
