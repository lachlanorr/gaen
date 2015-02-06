//------------------------------------------------------------------------------
// system_api_meta.h - Metadata for C functions callable from Compose scripts
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

#ifndef GAEN_COMPOSE_SYSTEM_API_META_H
#define GAEN_COMPOSE_SYSTEM_API_META_H

#include "core/base_defines.h"
#include "engine/hashes.h"
#include "engine/Handle.h"
#include "compose/compiler.h"

namespace gaen
{

const u32 kMaxApiParams = 16;

struct ApiSignature
{
    u32 nameHash;
    DataType returnType;
    DataType paramTypes[kMaxApiParams];
};

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_SYSTEM_API_META_H
