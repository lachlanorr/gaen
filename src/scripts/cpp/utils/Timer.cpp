//------------------------------------------------------------------------------
// Timer.cpp - Auto-generated from Timer.cmp
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

// HASH: c5440d412e57f0d857f6d4c2f5a1c57c
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

namespace gaen
{

namespace comp
{

class Timer : public Component
{
public:
    static Component * construct(void * place)
    {
        return new (place) Timer();
    }
    
    void update(float deltaSecs)
    {
        if ((interval() > 0.000000f))
        {
            last_notification() += deltaSecs;
            if ((last_notification() >= interval()))
            {
                {
                    StackMessageBlockWriter<1> msgw(HASH::timer__uint, kMessageFlag_None, mEntityTaskId, mEntityTaskId, to_cell(0));
                }
                last_notification() = 0.000000f;
            }
        }
    }

    template <typename T>
    MessageResult message(const T & msgAcc) { return MessageResult::Propogate; }

private:
    Timer()
    {
        interval() = 0.000000f;
        payload() = 0;
        last_notification() = 0.000000f;

        mTask = Task::createUpdatable(this, HASH::Timer);
        mBlockCount = 1;
    }
    Timer(const Timer&)              = delete;
    Timer(const Timer&&)             = delete;
    Timer & operator=(const Timer&)  = delete;
    Timer & operator=(const Timer&&) = delete;

    f32& interval()
    {
        return mpBlocks[0].cells[0].f;
    }
    u32& payload()
    {
        return mpBlocks[0].cells[1].u;
    }
    f32& last_notification()
    {
        return mpBlocks[0].cells[2].f;
    }

}; // class Timer

} // namespace comp

void register_component_Timer()
{
    if (!ComponentRegistry::register_constructor(HASH::Timer, comp::Timer::construct))
        PANIC("Unable to register component: Timer");
}

} // namespace gaen

