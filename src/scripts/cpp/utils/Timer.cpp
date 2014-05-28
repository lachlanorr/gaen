//------------------------------------------------------------------------------
// Timer.cpp - Auto-generated from Timer.cmp
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

// HASH: fc31ded2e5f5539dfe3d0dfb9e6f1ee0
#include "engine/stdafx.h"

#include "engine/Registry.h"
#include "engine/Component.h"

namespace gaen
{

namespace comp
{

class Timer : public Component
{
public:
    static Component* construct()
    {
        return GNEW(kMEM_Engine, Timer);
    }
    
private:
    Timer()
    {
        ASSERT_MSG(sIsRegistered, "Component not registered: 0x%08x", HASH::Timer);
    }
    Timer(const Timer&)      = delete;
    Timer(const Timer&&)     = delete;
    Timer & operator=(const Timer&)  = delete;
    Timer & operator=(const Timer&&) = delete;

    static bool sIsRegistered;
};

bool Timer::sIsRegistered = ComponentRegistry::register_constructor(HASH::Timer, Timer::construct);

} // namespace comp
 
} // namespace gaen

