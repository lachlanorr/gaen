//------------------------------------------------------------------------------
// Map.h - Typedefed std::map that uses our allocator
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

#ifndef GAEN_CORE_MAP_H
#define GAEN_CORE_MAP_H

#include <map>

#include "gaen/core/mem.h"

namespace gaen
{

// Declare maps with the additional MemType enum parameter, E.g.:
//   Map<int, void*, kMT_Engine> myMap;
template <MemType memType,
          class Key,
          class T,
          class Compare = std::less<Key>>
using Map = std::map<Key,
                     T,
                     Compare,
                     gaen::Allocator<memType, std::pair<const Key,T>>>;


template <MemType memType,
          class Key,
          class T,
          class Compare = std::less<Key>>
using MultiMap = std::multimap<Key,
                               T,
                               Compare,
                               gaen::Allocator<memType, std::pair<const Key,T>>>;


} // namespace gaen


#endif //#ifndef GAEN_CORE_MAP_H
