//------------------------------------------------------------------------------
// Vector.h - Typedefed std::vector that uses our allocator
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

#ifndef GAEN_CORE_VECTOR_H
#define GAEN_CORE_VECTOR_H

#include <vector>

#include "gaen/core/mem.h"

namespace gaen
{

// Declare vectors with the additional MemType enum parameter, E.g.:
//   Vector<int, kMT_Texture> myVector;
template <MemType memType, class T>
using Vector = std::vector<T, gaen::Allocator<memType, T>>;


} // namespace gaen


#endif //#ifndef GAEN_CORE_VECTOR_H
