//------------------------------------------------------------------------------
// ShaderRegistry_codegen.cpp - Shader factory class
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

#include "engine/hashes.h"
#include "renderergl/ShaderRegistry.h"
#include "renderergl/shaders/compute_present.h"
#include "renderergl/shaders/compute_test.h"
#include "renderergl/shaders/faceted.h"
#include "renderergl/shaders/voxel_cast.h"

namespace gaen
{

void ShaderRegistry::registerAllShaderConstructors()
{
    registerShaderConstructor(HASH::compute_present, shaders::compute_present::construct);
    registerShaderConstructor(HASH::compute_test, shaders::compute_test::construct);
    registerShaderConstructor(HASH::faceted, shaders::faceted::construct);
    registerShaderConstructor(HASH::voxel_cast, shaders::voxel_cast::construct);
}


} // namespace gaen
