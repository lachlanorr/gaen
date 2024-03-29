//------------------------------------------------------------------------------
// ShaderRegistry_codegen.cpp - Shader factory class
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

#include "gaen/renderergl/ShaderRegistry.h"
#include "gaen/renderergl/shaders/compute_present.h"
#include "gaen/renderergl/shaders/compute_test.h"
#include "gaen/renderergl/shaders/debug_lines.h"
#include "gaen/renderergl/shaders/faceted.h"
#include "gaen/renderergl/shaders/sprite.h"
#include "gaen/renderergl/shaders/voxchar.h"
#include "gaen/renderergl/shaders/voxel27.h"
#include "gaen/renderergl/shaders/voxel_cast.h"
#include "gaen/renderergl/shaders/voxel_cast_frag.h"
#include "gaen/renderergl/shaders/voxprop.h"
#include "gaen/renderergl/shaders/voxvertcol.h"

namespace gaen
{

void ShaderRegistry::registerAllShaderConstructors()
{
    registerShaderConstructor(0x4613be76 /* HASH::compute_present */, shaders::compute_present::construct);
    registerShaderConstructor(0x62ad79eb /* HASH::compute_test */, shaders::compute_test::construct);
    registerShaderConstructor(0x027c9dbc /* HASH::debug_lines */, shaders::debug_lines::construct);
    registerShaderConstructor(0x09352ef9 /* HASH::faceted */, shaders::faceted::construct);
    registerShaderConstructor(0x81e2581c /* HASH::sprite */, shaders::sprite::construct);
    registerShaderConstructor(0xd4208c92 /* HASH::voxchar */, shaders::voxchar::construct);
    registerShaderConstructor(0x9a83b4b8 /* HASH::voxel27 */, shaders::voxel27::construct);
    registerShaderConstructor(0xe9f55c27 /* HASH::voxel_cast */, shaders::voxel_cast::construct);
    registerShaderConstructor(0x427a5ca4 /* HASH::voxel_cast_frag */, shaders::voxel_cast_frag::construct);
    registerShaderConstructor(0xb4797a4f /* HASH::voxprop */, shaders::voxprop::construct);
    registerShaderConstructor(0x159d2745 /* HASH::voxvertcol */, shaders::voxvertcol::construct);
}


} // namespace gaen
