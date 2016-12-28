//------------------------------------------------------------------------------
// compose_funcs.h - Built in compose scripts
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

#ifndef GAEN_ENGINE_COMPOSE_FUNCS_H
#define GAEN_ENGINE_COMPOSE_FUNCS_H

#include "math/matrices.h"

#include "engine/Handle.h"
#include "engine/Entity.h"
#include "engine/BlockMemory.h"

namespace gaen
{

// This file gets parsed by codegen.py to create system_api_meta.cpp, which
// is used by Compose to compile type safe system calls.

// All parameters must be references or const references.

namespace compose_funcs
{

inline vec3 position(const mat43 & transform, Entity * pCaller)
{
    return gaen::position(transform);
}

//void activate_entity(task_id id, Entity & caller);

} // namespace compose_funcs

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COMPOSE_FUNCS_H
