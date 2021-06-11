//------------------------------------------------------------------------------
// ShaderRegistry.h - Shader factory class
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

#ifndef GAEN_RENDERERGL_SHADER_REGISTRY_H
#define GAEN_RENDERERGL_SHADER_REGISTRY_H

#include "core/base_defines.h"
#include "core/HashMap.h"
#include "renderergl/shaders/Shader.h"

namespace gaen
{

class ShaderRegistry
{
public:
    typedef shaders::Shader * (*ShaderConstructor)();

    ShaderRegistry();

    shaders::Shader * constructShader(u32 nameHash);

private:
    void registerShaderConstructor(u32 nameHash, ShaderConstructor constructor);
    void registerAllShaderConstructors();

    HashMap<kMEM_Renderer, u32, ShaderConstructor> mShaderConstructors;
};


}

#endif // #ifndef GAEN_RENDERERGL_SHADER_REGISTRY_H
