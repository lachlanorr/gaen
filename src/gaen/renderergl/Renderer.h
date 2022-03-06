//------------------------------------------------------------------------------
// Renderer.h - Generic renderer include that defines RendererType
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

#ifndef GAEN_RENDERERGL_RENDERER_H
#define GAEN_RENDERERGL_RENDERER_H

#include "gaen/core/base_defines.h"

#define USE_RENDERER_MESH   HAS_X
#define USE_RENDERER_PROTO  HAS__

#if HAS(USE_RENDERER_MESH)
#include "gaen/renderergl/RendererMesh.h"
#define RENDERER_TYPE RendererMesh
namespace gaen
{
typedef RendererMesh RendererType;
}

#elif HAS(USE_RENDERER_PROTO) // #if HAS(USE_RENDERER_MESH)
#include "gaen/renderergl/RendererProto.h"
#define RENDERER_TYPE RendererProto
namespace gaen
{
typedef RendererProto RendererType;
}

#else
#error No renderer defined
#endif // #if HAS(USE_PROTOTYPE_RENDERER)

#endif // #ifndef GAEN_RENDERERGL_RENDERER_H
