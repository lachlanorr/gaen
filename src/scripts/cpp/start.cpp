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

// HASH: 4961ca9682e315b646fde13cf1206c96
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

class start : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init:
        {
            u32 /* entity id */ box = /* LORRTODO: Add support for kAST_EntityInit Ast Type */;
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
    start(u32 childCount)
      : Entity(HASH::start, childCount, 36, 36)
    {

        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::start);

        // Component: utils.Timer
        {
            Task & compTask = insertComponent(HASH::utils.Timer, mComponentCount);
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
    start(const start&)              = delete;
    start(const start&&)             = delete;
    start & operator=(const start&)  = delete;
    start & operator=(const start&&) = delete;

}; // class start

} // namespace ent

void register_entity_start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::start, ent::start::construct))
        PANIC("Unable to register entity: start");
}

} // namespace gaen

