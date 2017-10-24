//------------------------------------------------------------------------------
// String.h - Typedefed std::string that uses our allocator
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_CORE_STRING_H
#define GAEN_CORE_STRING_H

#include <string>

#include "core/mem.h"

namespace gaen
{

// Declare strings with the additional MemType enum parameter, E.g.:
//   String<kMT_Engine> myStr;
template <MemType memType>
using String = std::basic_string<char,
                                 std::char_traits<char>,
                                 gaen::Allocator<memType, char>>;

typedef String<kMEM_Chef> ChefString;
typedef String<kMEM_Cara> CaraString;

} // namespace gaen

#endif //#ifndef GAEN_CORE_STRING_H
