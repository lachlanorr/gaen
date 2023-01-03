//------------------------------------------------------------------------------
// Random.cpp - Random number generating utilities
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

#include <chrono>
#include <limits>

#include "gaen/engine/Randomizer.h"

namespace gaen
{

Randomizer::Randomizer()
  : Randomizer(std::chrono::steady_clock::now().time_since_epoch().count())
{}

Randomizer::Randomizer(u32 seed)
  : mRng(seed)
{}

u32 Randomizer::randU32()
{
    return mRng();
}

u32 Randomizer::randU32(u32 min, u32 max)
{
    ASSERT(max > min);
    u32 r = mRng();
    u32 diff = max - min + 1;
    r = min + r % diff;
    return r;
}

i32 Randomizer::randI32()
{
    u32 r = mRng();
    r = r - std::numeric_limits<i32>::min();
    return r;
}

i32 Randomizer::randI32(i32 min, i32 max)
{
    ASSERT(max > min);
    u32 r = mRng();
    i32 diff = max - min + 1;
    r = min + r % diff;
    return r;
}

f32 Randomizer::randF32()
{
    u32 r = mRng();
    f64 r64 = (f64)r / (f64)std::numeric_limits<u32>::max();
    return (f32)r64;
}

f32 Randomizer::randF32(f32 min, f32 max)
{
    ASSERT(max > min);
    f32 diff = max - min;
    return min + diff * randF32();
}

} // namespace gaen
