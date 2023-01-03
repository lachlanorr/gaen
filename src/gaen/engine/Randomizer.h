//------------------------------------------------------------------------------
// Random.h - Random number generating utilities
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

#ifndef GAEN_ENGINE_RANDOM_H
#define GAEN_ENGINE_RANDOM_H

#include <random>

#include "gaen/core/base_defines.h"

namespace gaen
{

class Randomizer
{
public:
    Randomizer();
    Randomizer(u32 seed);

    u32 randU32();
    u32 randU32(u32 min, u32 max);
    i32 randI32();
    i32 randI32(i32 min, i32 max);

    f32 randF32(); // [0.0,1.0]
    f32 randF32(f32 min, f32 max);
private:
    std::mt19937 mRng;
}; // class Randomizer

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_HANDLE_H
