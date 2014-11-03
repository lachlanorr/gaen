//------------------------------------------------------------------------------
// init.cpp - Auto-generated from init.cmp
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

// HASH: b31dbbd1eb69a6f248178022178a9c95
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Handle.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"
#include "engine/system_api.h"

namespace gaen
{

namespace ent
{

class init__start : public Entity
{
private:
// Entity initializer helper functions
    task_id entity_init__init__shapes_Box__42_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::shapes__Box, 8);
        // Init Property: size
        {
            StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::size));
            *reinterpret_cast<Vec3*>(&msgw[0].cells[0].u) = Vec3(2.000000f, 2.000000f, 2.000000f);
            pEnt->task().message(msgw.accessor());
        }
        // Init Property: diffuse
        {
            StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::diffuse));
            msgw[0].cells[0].color = Color(255, 255, 0, 255);
            pEnt->task().message(msgw.accessor());
        }

        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }


public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__start, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init:
        {
            task_id box = entity_init__init__shapes_Box__42_23();
            system_api::insert_task(box, entity());
            return MessageResult::Consumed;
        }
        case HASH::timer:
        {
            if ((/*timer_msg*/msgAcc.message().payload.u == HASH::tick))
            {
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    init__start(u32 childCount)
      : Entity(HASH::init__start, childCount, 36, 36)
    {

        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__start);

        // Component: utils.Timer
        {
            Task & compTask = insertComponent(HASH::utils__Timer, mComponentCount);
            // Init Property: timer_interval
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_interval));
                msgw[0].cells[0].f = 0.016000f;
                compTask.message(msgw.accessor());
            }
            // Init Property: timer_message
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_message));
                msgw[0].cells[0].u = HASH::tick;
                compTask.message(msgw.accessor());
            }
        }
    }
    init__start(const init__start&)              = delete;
    init__start(const init__start&&)             = delete;
    init__start & operator=(const init__start&)  = delete;
    init__start & operator=(const init__start&&) = delete;

}; // class init__start

} // namespace ent

void register_entity__init__start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__start, ent::init__start::construct))
        PANIC("Unable to register entity: init__start");
}

} // namespace gaen

