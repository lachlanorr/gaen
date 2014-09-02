//------------------------------------------------------------------------------
// system_api.h - C functionas available to Compose scripts
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_COMPOSE_SYSTEM_API_H
#define GAEN_COMPOSE_SYSTEM_API_H

#include "engine/Handle.h"
#include "engine/Model.h"
#include "engine/Entity.h"
#include "engine/system_api_meta.h"

namespace gaen
{

const ApiSignature * find_api(const char * name);

// This file gets parsed by codegen.py.
// Always use a single line per definition, as the parser is quite simple.

//void print(

namespace system_api
{

Handle create_model_box(const Vec3 & size, const Color & color, const Entity & caller);
void renderer_insert_model_instance(const Handle & modelHandle, const u32 & instanceId, const Mat34 & transform, const Entity & caller);

}

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_SYSTEM_API_H
