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

// HASH: c6e2ba8706d7246795507da747deb599
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
        ASSERT(sIsRegistered, "Component not registered: 0x24866170", HASH::Timer);
    }
    Timer(const Timer&)      = delete;
    Timer(const Timer&&)     = delete;
    operator=(const Timer&)  = delete;
    operator=(const Timer&&) = delete;

    static bool sIsRegistered;
}

static bool Timer::sIsRegistered = ComponentRegistry::register_constructor(HASH::Timer, Timer::construct);

} // namespace comp
 
} // namespace gaen
