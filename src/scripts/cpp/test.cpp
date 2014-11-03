//------------------------------------------------------------------------------
// test.cpp - Auto-generated from test.cmp
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

// HASH: 5b74e3e0d2eb8730d80480a75fa2ae6b
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

class test__test : public Entity
{
private:
// Entity initializer helper functions
    task_id entity_init__test__shapes_Box__31_25()
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
            msgw[0].cells[0].color = Color(255, 0, 0, 255);
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
        return GNEW(kMEM_Engine, test__test, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init:
        {
            task_id box_1 = entity_init__test__shapes_Box__31_25();
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    test__test(u32 childCount)
      : Entity(HASH::test__test, childCount, 36, 36)
    {

        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::test__test);
    }
    test__test(const test__test&)              = delete;
    test__test(const test__test&&)             = delete;
    test__test & operator=(const test__test&)  = delete;
    test__test & operator=(const test__test&&) = delete;

}; // class test__test

} // namespace ent

void register_entity__test__test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__test, ent::test__test::construct))
        PANIC("Unable to register entity: test__test");
}

} // namespace gaen

