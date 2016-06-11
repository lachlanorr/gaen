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

// HASH: 5a36532ba5874b113f318bcb0e332c1f
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
    init__Shape(const init__Shape&)              = delete;
    init__Shape(init__Shape&&)             = delete;
    init__Shape & operator=(const init__Shape&)  = delete;
    init__Shape & operator=(init__Shape&&) = delete;

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
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    init__Light(const init__Light&)              = delete;
    init__Light(init__Light&&)             = delete;
    init__Light & operator=(const init__Light&)  = delete;
    init__Light & operator=(init__Light&&) = delete;

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
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    init__Camera(const init__Camera&)              = delete;
    init__Camera(init__Camera&&)             = delete;
    init__Camera & operator=(const init__Camera&)  = delete;
    init__Camera & operator=(init__Camera&&) = delete;

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
    task_id entity_init__init__Camera__91_23()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Camera, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Light__94_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Light, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

        stageEntity(pEnt);
        return pEnt->task().id();
    }

    task_id entity_init__init__Shape__97_25()
    {
        Entity * pEnt = get_registry().constructEntity(HASH::init__Shape, 8);
        // Send init message
        StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, pEnt->task().id(), pEnt->task().id(), to_cell(0));
        pEnt->task().message(msgBW.accessor());

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
            task_id cam = entity_init__init__Camera__91_23();
            system_api::insert_entity(cam, entity());
            task_id light = entity_init__init__Light__94_25();
            system_api::insert_entity(light, entity());
            task_id shape = entity_init__init__Shape__97_25();
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
        mBlockCount = 0;
        mScriptTask = Task::create(this, HASH::init__Start);
    }
    init__Start(const init__Start&)              = delete;
    init__Start(init__Start&&)             = delete;
    init__Start & operator=(const init__Start&)  = delete;
    init__Start & operator=(init__Start&&) = delete;

}; // class init__Start

} // namespace ent

void register_entity__init__Start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::init__Start, ent::init__Start::construct))
        PANIC("Unable to register entity: init__Start");
}

} // namespace gaen

