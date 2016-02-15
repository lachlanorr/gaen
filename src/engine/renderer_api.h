//------------------------------------------------------------------------------
// renderer_api.h - Functions thet must be implemented by the renderer
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_ENGINE_RENDERER_API_H
#define GAEN_ENGINE_RENDERER_API_H // #ifndef GAEN_ENGINE_RENDERER_API_H

#include "engine/Task.h"

namespace gaen
{

// The renderer must implement these functions.
void renderer_fin(Task & rendererTask);

void renderer_init_device(Task & rendererTask);
void renderer_init_viewport(Task & rendererTask);

void renderer_render(Task & rendererTask);
void renderer_end_frame(Task & rendererTask);

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_RENDERER_API_H
