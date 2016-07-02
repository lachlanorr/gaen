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

// HASH: a12545ad09a2d57e7c031ad1e2a491e8
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Handle.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

#include "engine/messages/Handle.h"

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
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            // Component: gaen.shapes.Sphere
            {
                Task & compTask = insertComponent(HASH::gaen__shapes__Sphere, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
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
            }
            // Component: gaen.utils.WasdRot
            {
                Task & compTask = insertComponent(HASH::gaen__utils__WasdRot, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
            }
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            return MessageResult::Consumed;
        } // HASH::fin__
        case HASH::init:
        {
            // Params look compatible, message body follows
            system_api::print(self().blockMemory().stringAlloc("Shape init"), self());
            return MessageResult::Consumed;
        } // HASH::init
        }
        return MessageResult::Propagate;
    }

private:
    init__Shape(u32 childCount)
      : Entity(HASH::init__Shape, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Shape);
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
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            // Component: gaen.lights.Directional
            {
                Task & compTask = insertComponent(HASH::gaen__lights__Directional, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
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
            }
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            return MessageResult::Consumed;
        } // HASH::fin__
        }
        return MessageResult::Propagate;
    }

private:
    init__Light(u32 childCount)
      : Entity(HASH::init__Light, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Light);
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
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            // Component: gaen.utils.WasdCamera
            {
                Task & compTask = insertComponent(HASH::gaen__utils__WasdCamera, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
            }
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            return MessageResult::Consumed;
        } // HASH::fin__
        }
        return MessageResult::Propagate;
    }

private:
    init__Camera(u32 childCount)
      : Entity(HASH::init__Camera, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Camera);
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
    task_id entity_init__init__test_Test__88_21()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::test__Test, 8);
        // Init Property: prop1
        {
            StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::prop1));
            msgw[0].cells[0].i = 500;
            pEnt->task().message(msgw.accessor());
        }
        // Init Property: prop2
        {
            CmpString val = self().blockMemory().stringAlloc("new value");
            ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::prop2), val.blockCount());
            val.writeMessage(msgw.accessor(), 0);
            pEnt->task().message(msgw.accessor());
        }

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
    }

    task_id entity_init__init__test_Test__97_50()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::test__Test, 8);
        // Init Property: prop1
        {
            StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::prop1));
            msgw[0].cells[0].i = 500;
            pEnt->task().message(msgw.accessor());
        }
        // Init Property: prop2
        {
            CmpString val = self().blockMemory().stringAlloc("new value");
            ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::prop2), val.blockCount());
            val.writeMessage(msgw.accessor(), 0);
            pEnt->task().message(msgw.accessor());
        }

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
    }

    task_id entity_init__init__Camera__104_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Camera, 8);

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
    }

    task_id entity_init__init__Light__106_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Light, 8);

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
    }

    task_id entity_init__init__Shape__108_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Shape, 8);

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
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
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            set_foo__path(self().blockMemory().stringAlloc("/fonts/profont.gatl"));
            foo() = nullptr;
            trooo() = entity_init__init__test_Test__88_21();
            // Component: gaen.utils.Timer
            {
                Task & compTask = insertComponent(HASH::gaen__utils__Timer, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
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
            }
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            self().requestAsset(mScriptTask.id(), HASH::foo, foo__path());
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            {
                messages::HandleR<T> msgr(msgAcc);
                ASSERT(msgr.taskId() == task().id());
                switch (msgr.nameHash())
                {
                case HASH::foo:
                    foo() = msgr.handle();
                default:
                    ERR("Invalid asset nameHash: %u", msgr.nameHash());
                }
            }
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            release_foo__path();
            release_foo();
            return MessageResult::Consumed;
        } // HASH::fin__
        case HASH::abc:
        {
            // Params look compatible, message body follows
            return MessageResult::Consumed;
        } // HASH::abc
        case HASH::init:
        {
            // Params look compatible, message body follows
            CmpString s = self().blockMemory().stringFormat("float: %0.2f, int: %d, and make sure we're larger than one block", 1.20000005e+00f, 10);
            task_id t = entity_init__init__test_Test__97_50();
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 1;

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::set_property, kMessageFlag_None, self().task().id(), t, to_cell(HASH::prop1), blockCount);

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
                MessageQueueWriter msgw(HASH::set_property, kMessageFlag_None, self().task().id(), t, to_cell(HASH::prop2), blockCount);

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
                CmpString bmParam1 = self().blockMemory().stringAlloc("a short one");
                blockCount += bmParam1.blockCount();
                CmpString bmParam2 = self().blockMemory().stringAlloc("this is another string that is larger than one block, and then some more");
                blockCount += bmParam2.blockCount();

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::msg1, kMessageFlag_None, self().task().id(), t, to_cell(5), blockCount);

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
            task_id cam = entity_init__init__Camera__104_23();
            task_id light = entity_init__init__Light__106_25();
            task_id shape = entity_init__init__Shape__108_25();
            return MessageResult::Consumed;
        } // HASH::init
        }
        return MessageResult::Propagate;
    }

private:
    init__Start(u32 childCount)
      : Entity(HASH::init__Start, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 2;
        mScriptTask = Task::create(this, HASH::init__Start);
    }

    init__Start(const init__Start&)              = delete;
    init__Start(init__Start&&)                   = delete;
    init__Start & operator=(const init__Start&)  = delete;
    init__Start & operator=(init__Start&&)       = delete;

    AssetHandleP& foo()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[0].cells[0]);
    }
    void release_foo()
    {
        if (foo() != nullptr)
        {
            foo()->release();
        }
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
            self().blockMemory().release(foo__path());
        }
        mIs_foo__path_Assigned = false;
    }
    void set_foo__path(const CmpStringAsset& rhs)
    {
        release_foo__path();
        foo__path() = rhs;
        self().blockMemory().addRef(foo__path());
        mIs_foo__path_Assigned = true;
    }

    task_id& trooo()
    {
        return mpBlocks[1].cells[0].u;
    }

}; // class init__Start

} // namespace ent

void register_entity__init__Start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Start, ent::init__Start::construct))
        PANIC("Unable to register entity: init__Start");
}

} // namespace gaen

