//------------------------------------------------------------------------------
// start.cpp - Auto-generated from start.cmp
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

// HASH: 38c4f9753fc2393216012e8fabbcb175
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

namespace gaen
{

namespace ent
{

class start : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start, childCount);
    }
    
    void update(float deltaSecs)
    {
        f32 f = deltaSecs;
        i32 x = 5;
        x = 6;
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        switch(msgAcc.message().msgId)
        {
        case HASH::set_property__float:
            switch (msgAcc[0].cells[0].u)
            {
            case HASH::f_prop:
                f_prop() = *reinterpret_cast<const f32*>(&msgAcc[0].cells[1].u);
                return MessageResult::Consumed;
            case HASH::f_field:
                f_field() = *reinterpret_cast<const f32*>(&msgAcc[0].cells[1].u);
                return MessageResult::Consumed;
            }
            return MessageResult::Propogate; // Invalid property
        }
        return MessageResult::Propogate;
}

private:
    start(u32 childCount)
      : Entity(HASH::start, childCount, 36, 36)
    {
        f_prop() = 1.000000f;
        f_field() = 2.000000f;

        mBlockCount = 1;
        mTask = Task::createUpdatable(this, HASH::start);

        // Component: Timer
        {
            Task & compTask = insertComponent(HASH::Timer, mComponentCount);
            // Init Property: interval
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property__float, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(HASH::Timer));
                msgw[0].cells[0].u = HASH::interval;
                msgw[0].cells[1].f = 1.000000f;
                compTask.message(msgw.accessor());
            }
            // Init Property: message
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property__uint, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(HASH::Timer));
                msgw[0].cells[0].u = HASH::message;
                msgw[0].cells[1].u = HASH::msg;
                compTask.message(msgw.accessor());
            }
        }
    }
    start(const start&)              = delete;
    start(const start&&)             = delete;
    start & operator=(const start&)  = delete;
    start & operator=(const start&&) = delete;

    f32& f_prop()
    {
        return mpBlocks[0].cells[0].f;
    }
    f32& f_field()
    {
        return mpBlocks[0].cells[1].f;
    }
}; // class start

} // namespace ent

void register_entity_start()
{
    if (!EntityRegistry::register_constructor(HASH::start, ent::start::construct))
        PANIC("Unable to register entity: start");
}

} // namespace gaen

