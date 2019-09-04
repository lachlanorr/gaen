//------------------------------------------------------------------------------
// render_objects.cpp - Classes shared by renderers and the engine
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

#include "render_support/stdafx.h"

#include "render_support/render_objects.h"

namespace gaen
{

std::atomic<ruid> RenderObject::sNextRuid(1);

ruid RenderObject::next_uid()
{
    return sNextRuid.fetch_add(1, std::memory_order_relaxed);
}

RenderPass pass_from_hash(i32 hash)
{
    switch (hash)
    {
    case HASH::opaque:
        return kRP_Opaque;
    case HASH::alpha:
        return kRP_Alpha;
    case HASH::engine:
        return kRP_Engine;
    case HASH::debug:
        return kRP_Debug;
    default:
        ERR("Invalid pass hash, defaulting to opaque, hash: %d", hash);
        return kRP_Opaque;
    }
}

} // namespace gaen
