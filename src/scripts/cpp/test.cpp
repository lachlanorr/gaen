//------------------------------------------------------------------------------
// test.cpp - Auto-generated from test.cmp
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

// HASH: adf8dd8ae50f14513afe1f8ea685095e
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

class test__Light : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, test__Light, childCount);
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data__:
        {
            // Component: gaen.lights.Directional
            {
                Task & compTask = insertComponent(HASH::gaen__lights__Directional, mComponentCount);
                compTask.message(msgAcc); // propagate init_data__ into component
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
        } // case HASH::init_data__
        }
        return MessageResult::Propagate;
    }

private:
    test__Light(u32 childCount)
      : Entity(HASH::test__Light, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::test__Light);
    }

    test__Light(const test__Light&)              = delete;
    test__Light(test__Light&&)                   = delete;
    test__Light & operator=(const test__Light&)  = delete;
    test__Light & operator=(test__Light&&)       = delete;

}; // class test__Light

} // namespace ent

void register_entity__test__Light(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Light, ent::test__Light::construct))
        PANIC("Unable to register entity: test__Light");
}

namespace ent
{

class test__Test : public Entity
{
private:
    // Helper functions
    task_id entity_init__test__Light__53_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::test__Light, 8);

        task_id tid = pEnt->task().id();
        pEnt->activate();
        return tid;
    }


public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, test__Test, childCount);
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data__:
        {
            return MessageResult::Consumed;
        } // case HASH::init_data__
        case HASH::init:
        {
            // Params look compatible, message body follows
            task_id light = entity_init__test__Light__53_25();
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propagate;
    }

private:
    test__Test(u32 childCount)
      : Entity(HASH::test__Test, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::test__Test);
    }

    test__Test(const test__Test&)              = delete;
    test__Test(test__Test&&)                   = delete;
    test__Test & operator=(const test__Test&)  = delete;
    test__Test & operator=(test__Test&&)       = delete;

}; // class test__Test

} // namespace ent

void register_entity__test__Test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Test, ent::test__Test::construct))
        PANIC("Unable to register entity: test__Test");
}

} // namespace gaen

