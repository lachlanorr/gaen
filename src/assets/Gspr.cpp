//------------------------------------------------------------------------------
// Gspr.cpp - Sprites with animations, depend on a gatl for images
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

#include "assets/Gatl.h"
#include "assets/Gspr.h"

namespace gaen
{

bool Gspr::is_valid(const void * pBuffer, u64 size)
{
    PANIC("TODO");
    return false;
}

Gspr * Gspr::instance(void * pBuffer, u64 size)
{
    PANIC("TODO");
    return nullptr;
}

const Gspr * Gspr::instance(const void * pBuffer, u64 size)
{
    PANIC("TODO");
    return nullptr;
}

u64 Gspr::required_size()
{
    PANIC("TODO");
    return 0;
}

Gspr * Gspr::create()
{
    PANIC("TODO");
    return nullptr;
}

u64 Gspr::size() const
{
    PANIC("TODO");
    return 0;
}

u32 Gspr::glyphWidth()
{
    PANIC("TODO");
    return 0;
}

u32 Gspr::glyphHeight()
{
    PANIC("TODO");
    return 0;
}

u32 Gspr::animCount()
{
    PANIC("TODO");
    return 0;
}

u32 Gspr::frameCount(u32 animHash)
{
    PANIC("TODO");
    return 0;
}

const GlyphCoords & Gspr::getFrame(u32 animHash, u32 frameIdx)
{
    PANIC("TODO");
    return GlyphCoords();
}

const char * Gspr::atlasPath() const
{
    PANIC("TODO");
    return nullptr;
}

} // namespace gaen

