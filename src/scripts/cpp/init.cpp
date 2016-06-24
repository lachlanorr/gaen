//------------------------------------------------------------------------------
// init.cpp - Auto-generated from init.cmp
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

// HASH: f1b7b7795c5070af799f03225108cf12
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Handle.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

// system_api declarations
#include "engine/shapes.h"
#include "engine/system_api.h"

namespace gaen
{

namespace ent
{

class init__Shape : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__Shape, childCount);
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
            system_api::print(entity().blockMemory().stringAlloc("Shape init"), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    init__Shape(u32 childCount)
      : Entity(HASH::init__Shape, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Shape);

        // Component: gaen.shapes.Sphere
        {
            Task & compTask = insertComponent(HASH::gaen__shapes__Sphere, mComponentCount);
            // Init Property: size
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::size));
                *reinterpret_cast<glm::vec3*>(&msgw[0].cells[0].u) = glm::vec3(1.00000000e+00f, 1.00000000e+00f, 1.00000000e+00f);
                compTask.message(msgw.accessor());
            }
            // Init Property: slices
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::slices));
                msgw[0].cells[0].i = 32;
                compTask.message(msgw.accessor());
            }
            // Init Property: sections
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::sections));
                msgw[0].cells[0].i = 16;
                compTask.message(msgw.accessor());
            }
            // Init Property: diffuse
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::diffuse));
                msgw[0].cells[0].color = Color(255, 255, 0, 255);
                compTask.message(msgw.accessor());
            }
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }

        // Component: gaen.utils.WasdRot
        {
            Task & compTask = insertComponent(HASH::gaen__utils__WasdRot, mComponentCount);
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }
    }
    init__Shape(const init__Shape&)              = delete;
    init__Shape(init__Shape&&)                   = delete;
    init__Shape & operator=(const init__Shape&)  = delete;
    init__Shape & operator=(init__Shape&&)       = delete;

}; // class init__Shape

} // namespace ent

void register_entity__init__Shape(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Shape, ent::init__Shape::construct))
        PANIC("Unable to register entity: init__Shape");
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

        // Component: gaen.lights.Directional
        {
            Task & compTask = insertComponent(HASH::gaen__lights__Directional, mComponentCount);
            // Init Property: dir
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::dir));
                *reinterpret_cast<glm::vec3*>(&msgw[0].cells[0].u) = glm::vec3(1.00000000e+00f, -(6.99999988e-01f), -(5.00000000e-01f));
                compTask.message(msgw.accessor());
            }
            // Init Property: col
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::col));
                msgw[0].cells[0].color = Color(255, 255, 0, 255);
                compTask.message(msgw.accessor());
            }
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }
    }
    init__Light(const init__Light&)              = delete;
    init__Light(init__Light&&)                   = delete;
    init__Light & operator=(const init__Light&)  = delete;
    init__Light & operator=(init__Light&&)       = delete;

}; // class init__Light

} // namespace ent

void register_entity__init__Light(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Light, ent::init__Light::construct))
        PANIC("Unable to register entity: init__Light");
}

namespace ent
{

class init__Camera : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__Camera, childCount);
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        return MessageResult::Propogate;
    }

private:
    init__Camera(u32 childCount)
      : Entity(HASH::init__Camera, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Camera);

        // Component: gaen.utils.WasdCamera
        {
            Task & compTask = insertComponent(HASH::gaen__utils__WasdCamera, mComponentCount);
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }
    }
    init__Camera(const init__Camera&)              = delete;
    init__Camera(init__Camera&&)                   = delete;
    init__Camera & operator=(const init__Camera&)  = delete;
    init__Camera & operator=(init__Camera&&)       = delete;

}; // class init__Camera

} // namespace ent

void register_entity__init__Camera(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Camera, ent::init__Camera::construct))
        PANIC("Unable to register entity: init__Camera");
}

namespace ent
{

class init__Start : public Entity
{
private:
    // Helper functions
    task_id entity_init__init__test_Test__90_86()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::test__Test, 8);
        // Send init_assets message
        StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInitAssets.accessor());
        // Send init message
        StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInit.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Camera__101_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Camera, 8);
        // Send init_assets message
        StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInitAssets.accessor());
        // Send init message
        StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInit.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Light__104_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Light, 8);
        // Send init_assets message
        StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInitAssets.accessor());
        // Send init message
        StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInit.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Shape__107_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Shape, 8);
        // Send init_assets message
        StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInitAssets.accessor());
        // Send init message
        StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgInit.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }


public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, init__Start, childCount);
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
            CmpString s = entity().blockMemory().stringFormat("float: %0.2f, int: %d, and make sure we're larger than one block", 1.20000005e+00f, 10);
            task_id t = entity_init__init__test_Test__90_86();
            system_api::insert_entity(t, entity());
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 1;

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::set_property, kMessageFlag_None, entity().task().id(), t, to_cell(HASH::prop1), blockCount);

                // Write parameters to message
                *reinterpret_cast<i32*>(&msgw[0].cells[0]) = 5;

                // MessageQueueWriter will send message through RAII when this scope is exited
            }
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 0;
                CmpString bmParam0 = s;
                blockCount += bmParam0.blockCount();

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::set_property, kMessageFlag_None, entity().task().id(), t, to_cell(HASH::prop2), blockCount);

                u32 startIndex = 0; // location in message to which to copy block memory items
                // Write parameters to message
                bmParam0.writeMessage(msgw, startIndex);
                startIndex += bmParam0.blockCount();

                // MessageQueueWriter will send message through RAII when this scope is exited
            }
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

                u32 startIndex = 1; // location in message to which to copy block memory items
                // Write parameters to message
                *reinterpret_cast<glm::vec3*>(&msgw[0].cells[0]) = glm::vec3(1.00000000e+00f, 2.00000000e+00f, 3.00000000e+00f);
                *reinterpret_cast<f32*>(&msgw[0].cells[3]) = 1.20000005e+00f;
                bmParam0.writeMessage(msgw, startIndex);
                startIndex += bmParam0.blockCount();
                bmParam1.writeMessage(msgw, startIndex);
                startIndex += bmParam1.blockCount();
                bmParam2.writeMessage(msgw, startIndex);
                startIndex += bmParam2.blockCount();

                // MessageQueueWriter will send message through RAII when this scope is exited
            }
            task_id cam = entity_init__init__Camera__101_23();
            system_api::insert_entity(cam, entity());
            task_id light = entity_init__init__Light__104_25();
            system_api::insert_entity(light, entity());
            task_id shape = entity_init__init__Shape__107_25();
            system_api::insert_entity(shape, entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    init__Start(u32 childCount)
      : Entity(HASH::init__Start, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        set_foo__path(entity().blockMemory().stringAlloc("/fonts/profont.gatl"));
        foo() = nullptr;
        set_bar__path(entity().blockMemory().stringAlloc("/images/bar.tga"));
        bar() = nullptr;
        mBlockCount = 2;
        mScriptTask = Task::create(this, HASH::init__Start);

        // Component: gaen.utils.Timer
        {
            Task & compTask = insertComponent(HASH::gaen__utils__Timer, mComponentCount);
            // Init Property: timer_interval
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_interval));
                msgw[0].cells[0].f = 1.00000000e+00f;
                compTask.message(msgw.accessor());
            }
            // Init Property: timer_message
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_message));
                msgw[0].cells[0].u = HASH::abc;
                compTask.message(msgw.accessor());
            }
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }
    }
    init__Start(const init__Start&)              = delete;
    init__Start(init__Start&&)                   = delete;
    init__Start & operator=(const init__Start&)  = delete;
    init__Start & operator=(init__Start&&)       = delete;

    AssetHandleP& foo()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[0].cells[0]);
    }

    CmpStringAsset& foo__path()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[2]);
    }
    bool mIs_foo__path_Assigned = false;
    void release_foo__path()
    {
        if (mIs_foo__path_Assigned)
        {
            entity().blockMemory().release(foo__path());
        }
        mIs_foo__path_Assigned = false;
    }
    void set_foo__path(const CmpStringAsset& rhs)
    {
        ERR("TODO: release block memory strings in #fin message!!!");
        release_foo__path();
        foo__path() = rhs;
        entity().blockMemory().addRef(foo__path());
        mIs_foo__path_Assigned = true;
    }

    AssetHandleP& bar()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[1].cells[0]);
    }

    CmpStringAsset& bar__path()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[1].cells[2]);
    }
    bool mIs_bar__path_Assigned = false;
    void release_bar__path()
    {
        if (mIs_bar__path_Assigned)
        {
            entity().blockMemory().release(bar__path());
        }
        mIs_bar__path_Assigned = false;
    }
    void set_bar__path(const CmpStringAsset& rhs)
    {
        ERR("TODO: release block memory strings in #fin message!!!");
        release_bar__path();
        bar__path() = rhs;
        entity().blockMemory().addRef(bar__path());
        mIs_bar__path_Assigned = true;
    }

}; // class init__Start

} // namespace ent

void register_entity__init__Start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Start, ent::init__Start::construct))
        PANIC("Unable to register entity: init__Start");
}

} // namespace gaen

