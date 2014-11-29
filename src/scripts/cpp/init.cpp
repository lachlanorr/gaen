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

// HASH: 05a15e3aac6831281ecdf4ddf9f30c5d
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
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

class init__Box : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__Box, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        return MessageResult::Propogate;
    }

private:
    init__Box(u32 childCount)
      : Entity(HASH::init__Box, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Box);

        // Component: shapes.Box
        {
            Task & compTask = insertComponent(HASH::shapes__Box, mComponentCount);
            // Init Property: size
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::size));
                *reinterpret_cast<Vec3*>(&msgw[0].cells[0].u) = Vec3(3.00000000e+000f, 2.00000000e+000f, 1.00000000e+000f);
                compTask.message(msgw.accessor());
            }
            // Init Property: diffuse
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::diffuse));
                msgw[0].cells[0].color = Color(255, 255, 0, 255);
                compTask.message(msgw.accessor());
            }
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }

        // Component: utils.WasdRot
        {
            Task & compTask = insertComponent(HASH::utils__WasdRot, mComponentCount);
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    init__Box(const init__Box&)              = delete;
    init__Box(const init__Box&&)             = delete;
    init__Box & operator=(const init__Box&)  = delete;
    init__Box & operator=(const init__Box&&) = delete;

}; // class init__Box

} // namespace ent

void register_entity__init__Box(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Box, ent::init__Box::construct))
        PANIC("Unable to register entity: init__Box");
}

namespace ent
{

class init__Light : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__Light, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        return MessageResult::Propogate;
    }

private:
    init__Light(u32 childCount)
      : Entity(HASH::init__Light, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Light);

        // Component: lights.Directional
        {
            Task & compTask = insertComponent(HASH::lights__Directional, mComponentCount);
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    init__Light(const init__Light&)              = delete;
    init__Light(const init__Light&&)             = delete;
    init__Light & operator=(const init__Light&)  = delete;
    init__Light & operator=(const init__Light&&) = delete;

}; // class init__Light

} // namespace ent

void register_entity__init__Light(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Light, ent::init__Light::construct))
        PANIC("Unable to register entity: init__Light");
}

namespace ent
{

class init__start : public Entity
{
private:
    // Helper functions
    task_id entity_init__init__test_Test__51_21()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::test__Test, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Box__57_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Box, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Light__58_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Light, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

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
            // Params look compatible, message body follows
            CmpString s = entity().blockMemory().stringFormat("float: %0.2f, int: %d, and make sure we're larger than one block", 1.20000005e+000f, 10);
            task_id t = entity_init__init__test_Test__51_21();
            system_api::insert_task(t, entity());
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 1;
                CmpString bmParam0 = s;
                blockCount += bmParam0.blockCount();
                CmpString bmParam1 = entity().blockMemory().stringAlloc("a short one");
                blockCount += bmParam1.blockCount();
                CmpString bmParam2 = entity().blockMemory().stringAlloc("this is another string that is larger than one block, and then some more");
                blockCount += bmParam2.blockCount();

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::msg1, kMessageFlag_None, entity().task().id(), t, to_cell(5), blockCount);

                u32 startIndex = 1; // location in message to copy block memory items to
                // Write parameters to message
                *reinterpret_cast<Vec3*>(&msgw[0].cells[0]) = Vec3(1.00000000e+000f, 2.00000000e+000f, 3.00000000e+000f);
                *reinterpret_cast<f32*>(&msgw[0].cells[3]) = 1.20000005e+000f;
                bmParam0.writeMessage(msgw, startIndex);
                startIndex += bmParam0.blockCount();
                bmParam1.writeMessage(msgw, startIndex);
                startIndex += bmParam1.blockCount();
                bmParam2.writeMessage(msgw, startIndex);
                startIndex += bmParam2.blockCount();

                // MessageQueueWriter will send message through RAII when this scope is exited
            }
            task_id box = entity_init__init__Box__57_23();
            task_id light = entity_init__init__Light__58_25();
            system_api::insert_task(box, entity());
            system_api::insert_task(light, entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    init__start(u32 childCount)
      : Entity(HASH::init__start, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__start);
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

