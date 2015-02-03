//------------------------------------------------------------------------------
// start.cpp - Auto-generated from start.cmp
//------------------------------------------------------------------------------

// HASH: 7e3556ab45e1771bf92032fc16f3bb7d
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

class start__Box : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start__Box, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        return MessageResult::Propogate;
    }

private:
    start__Box(u32 childCount)
      : Entity(HASH::start__Box, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::start__Box);

        // Component: gaen.shapes.Box
        {
            Task & compTask = insertComponent(HASH::gaen__shapes__Box, mComponentCount);
            // Init Property: size
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::size));
                *reinterpret_cast<Vec3*>(&msgw[0].cells[0].u) = Vec3(3.00000000e+00f, 2.00000000e+00f, 1.00000000e+00f);
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

        // Component: gaen.utils.WasdRot
        {
            Task & compTask = insertComponent(HASH::gaen__utils__WasdRot, mComponentCount);
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    start__Box(const start__Box&)              = delete;
    start__Box(const start__Box&&)             = delete;
    start__Box & operator=(const start__Box&)  = delete;
    start__Box & operator=(const start__Box&&) = delete;

}; // class start__Box

} // namespace ent

void register_entity__start__Box(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::start__Box, ent::start__Box::construct))
        PANIC("Unable to register entity: start__Box");
}

namespace ent
{

class start__Light : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start__Light, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        return MessageResult::Propogate;
    }

private:
    start__Light(u32 childCount)
      : Entity(HASH::start__Light, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::start__Light);

        // Component: gaen.lights.Directional
        {
            Task & compTask = insertComponent(HASH::gaen__lights__Directional, mComponentCount);
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    start__Light(const start__Light&)              = delete;
    start__Light(const start__Light&&)             = delete;
    start__Light & operator=(const start__Light&)  = delete;
    start__Light & operator=(const start__Light&&) = delete;

}; // class start__Light

} // namespace ent

void register_entity__start__Light(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::start__Light, ent::start__Light::construct))
        PANIC("Unable to register entity: start__Light");
}

namespace ent
{

class start__start : public Entity
{
private:
    // Helper functions
    task_id entity_init__start__Box__24_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::start__Box, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__start__Light__25_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::start__Light, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }


public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start__start, childCount);
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
            task_id box = entity_init__start__Box__24_23();
            task_id light = entity_init__start__Light__25_25();
            system_api::insert_task(box, entity());
            system_api::insert_task(light, entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    start__start(u32 childCount)
      : Entity(HASH::start__start, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::start__start);
    }
    start__start(const start__start&)              = delete;
    start__start(const start__start&&)             = delete;
    start__start & operator=(const start__start&)  = delete;
    start__start & operator=(const start__start&&) = delete;

}; // class start__start

} // namespace ent

void register_entity__start__start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::start__start, ent::start__start::construct))
        PANIC("Unable to register entity: start__start");
}

} // namespace gaen

