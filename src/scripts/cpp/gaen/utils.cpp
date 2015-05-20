//------------------------------------------------------------------------------
// utils.cpp - Auto-generated from utils.cmp
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

// HASH: 5cc798ac7fd352759900ee248cf9fcd5
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

namespace comp
{

class gaen__utils__Timer : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) gaen__utils__Timer(pEntity);
    }
    
    void update(float deltaSecs)
    {
        if ((timer_interval() > 0.00000000e+00f))
        {
            last_notification() += deltaSecs;
            if ((last_notification() >= timer_interval()))
            {
                { // Send Message Block
                    // Compute block size, incorporating any BlockMemory parameters dynamically
                    u32 blockCount = 0;

                    // Prepare the queue writer
                    MessageQueueWriter msgw(HASH::timer, kMessageFlag_None, entity().task().id(), entity().task().id(), to_cell(timer_message()), blockCount);

                    // Write parameters to message

                    // MessageQueueWriter will send message through RAII when this scope is exited
                }
                last_notification() = 0.00000000e+00f;
            }
        }
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            timer_interval() = 0.00000000e+00f;
            timer_message() = 0;
            last_notification() = 0.00000000e+00f;
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::timer_interval:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&timer_interval())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            case HASH::timer_message:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&timer_message())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propogate; // Invalid property
        }
        return MessageResult::Propogate;
}

private:
    gaen__utils__Timer(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::gaen__utils__Timer);
        mBlockCount = 1;
    }
    gaen__utils__Timer(const gaen__utils__Timer&)              = delete;
    gaen__utils__Timer(const gaen__utils__Timer&&)             = delete;
    gaen__utils__Timer & operator=(const gaen__utils__Timer&)  = delete;
    gaen__utils__Timer & operator=(const gaen__utils__Timer&&) = delete;

    f32& timer_interval()
    {
        return mpBlocks[0].cells[0].f;
    }

    u32& timer_message()
    {
        return mpBlocks[0].cells[1].u;
    }

    f32& last_notification()
    {
        return mpBlocks[0].cells[2].f;
    }


}; // class gaen__utils__Timer

} // namespace comp

void register_component__gaen__utils__Timer(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::gaen__utils__Timer, comp::gaen__utils__Timer::construct))
        PANIC("Unable to register component: gaen__utils__Timer");
}

namespace comp
{

class gaen__utils__WasdCamera : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) gaen__utils__WasdCamera(pEntity);
    }
    
    void update(float deltaSecs)
    {
        bool hasMoved = 0;
        if ((mouseWheelDelta() != 0))
        {
            pos() += (dirForward() * ((mouseWheelDelta() * deltaSecs) * 9.99999978e-03f));
            mouseWheelDelta() = 0;
            hasMoved = 1;
        }
        if (mouseLooking())
        {
            bool hasRotated = 0;
            if ((mouseDeltaX() != 0.00000000e+00f))
            {
                f32 angle = (-(mouseDeltaX()) * rotDelta());
                Quat orientDelta = system_api::quat_from_axis_angle(dirUp(), angle, entity());
                orientation() = (orientDelta * orientation());
                hasRotated = 1;
            }
            if ((mouseDeltaY() != 0.00000000e+00f))
            {
                f32 angle = (mouseDeltaY() * rotDelta());
                Quat orientDelta = system_api::quat_from_axis_angle(dirRight(), angle, entity());
                orientation() = (orientDelta * orientation());
                hasRotated = 1;
            }
            if (hasRotated)
            {
                orientation() = system_api::quat_normalize(orientation(), entity());
                dirForward() = (orientation() * dirForwardInit());
                dirRight() = (orientation() * dirRightInit());
                dirUp() = (orientation() * dirUpInit());
                hasMoved = 1;
            }
            mouseDeltaX() = 0.00000000e+00f;
            mouseDeltaY() = 0.00000000e+00f;
        }
        if ((forwardBackward() != 0.00000000e+00f))
        {
            pos() += (dirForward() * ((moveDelta() * forwardBackward()) * deltaSecs));
            hasMoved = 1;
        }
        if ((leftRight() != 0.00000000e+00f))
        {
            pos() += (-(dirRight()) * ((moveDelta() * leftRight()) * deltaSecs));
            hasMoved = 1;
        }
        if (hasMoved)
        {
            system_api::renderer_move_camera(pos(), orientation(), entity());
        }
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            dirForwardInit() = Vec3(0.00000000e+00f, 0.00000000e+00f, -(1.00000000e+00f));
            dirRightInit() = Vec3(1.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f);
            dirUpInit() = Vec3(0.00000000e+00f, 1.00000000e+00f, 0.00000000e+00f);
            dirForward() = dirForwardInit();
            dirRight() = dirRightInit();
            dirUp() = dirUpInit();
            orientation() = Quat(0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 1.00000000e+00f);
            pos() = Vec3(0.00000000e+00f, 0.00000000e+00f, 1.00000000e+01f);
            moveDelta() = 5.00000000e+00f;
            rotDelta() = 5.00000024e-04f;
            forwardBackward() = 0.00000000e+00f;
            leftRight() = 0.00000000e+00f;
            mouseLooking() = 0;
            mouseDeltaX() = 0.00000000e+00f;
            mouseDeltaY() = 0.00000000e+00f;
            mouseWheelDelta() = 0;
            return MessageResult::Consumed;
        case HASH::init:
        {
            // Params look compatible, message body follows
            system_api::watch_input_state(HASH::mouse_look, 0, HASH::mouse_look, entity());
            system_api::watch_mouse(HASH::mouse_move, HASH::mouse_wheel, entity());
            system_api::watch_input_state(HASH::forward, 0, HASH::forward, entity());
            system_api::watch_input_state(HASH::back, 0, HASH::back, entity());
            system_api::watch_input_state(HASH::left, 0, HASH::left, entity());
            system_api::watch_input_state(HASH::right, 0, HASH::right, entity());
            return MessageResult::Consumed;
        }
        case HASH::mouse_look:
        {
            // Params look compatible, message body follows
            mouseLooking() = /*status*/msgAcc.message().payload.b;
            return MessageResult::Consumed;
        }
        case HASH::mouse_move:
        {
            // Verify params look compatible with this message type
            u32 expectedBlockSize = 1; // BlockCount without BlockMemory params
            if (expectedBlockSize > msgAcc.available())
                return MessageResult::Propogate;

            // Params look compatible, message body follows
            if (mouseLooking())
            {
                mouseDeltaX() += /*xDelta*/msgAcc.message().payload.i;
                mouseDeltaY() += /*yDelta*/msgAcc[0].cells[0].i;
            }
            return MessageResult::Consumed;
        }
        case HASH::mouse_wheel:
        {
            // Params look compatible, message body follows
            mouseWheelDelta() += /*delta*/msgAcc.message().payload.i;
            return MessageResult::Consumed;
        }
        case HASH::forward:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                forwardBackward() = 1.00000000e+00f;
            }
            else
            {
                forwardBackward() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::back:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                forwardBackward() = -(1.00000000e+00f);
            }
            else
            {
                forwardBackward() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::left:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                leftRight() = 1.00000000e+00f;
            }
            else
            {
                leftRight() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::right:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                leftRight() = -(1.00000000e+00f);
            }
            else
            {
                leftRight() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    gaen__utils__WasdCamera(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::gaen__utils__WasdCamera);
        mBlockCount = 9;
    }
    gaen__utils__WasdCamera(const gaen__utils__WasdCamera&)              = delete;
    gaen__utils__WasdCamera(const gaen__utils__WasdCamera&&)             = delete;
    gaen__utils__WasdCamera & operator=(const gaen__utils__WasdCamera&)  = delete;
    gaen__utils__WasdCamera & operator=(const gaen__utils__WasdCamera&&) = delete;

    Vec3& dirForwardInit()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[1].qCell);
    }

    Vec3& dirRightInit()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[2].qCell);
    }

    Vec3& dirUpInit()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[3].qCell);
    }

    Vec3& dirForward()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[4].qCell);
    }

    Vec3& dirRight()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[5].qCell);
    }

    Vec3& dirUp()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[6].qCell);
    }

    Quat& orientation()
    {
        return *reinterpret_cast<Quat*>(&mpBlocks[0].qCell);
    }

    Vec3& pos()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[7].qCell);
    }

    f32& moveDelta()
    {
        return mpBlocks[1].cells[3].f;
    }

    f32& rotDelta()
    {
        return mpBlocks[2].cells[3].f;
    }

    f32& forwardBackward()
    {
        return mpBlocks[3].cells[3].f;
    }

    f32& leftRight()
    {
        return mpBlocks[4].cells[3].f;
    }

    bool& mouseLooking()
    {
        return mpBlocks[5].cells[3].b;
    }

    f32& mouseDeltaX()
    {
        return mpBlocks[6].cells[3].f;
    }

    f32& mouseDeltaY()
    {
        return mpBlocks[7].cells[3].f;
    }

    i32& mouseWheelDelta()
    {
        return mpBlocks[8].cells[0].i;
    }


}; // class gaen__utils__WasdCamera

} // namespace comp

void register_component__gaen__utils__WasdCamera(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::gaen__utils__WasdCamera, comp::gaen__utils__WasdCamera::construct))
        PANIC("Unable to register component: gaen__utils__WasdCamera");
}

namespace comp
{

class gaen__utils__WasdRot : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) gaen__utils__WasdRot(pEntity);
    }
    
    void update(float deltaSecs)
    {
        if (((pitching() != 0.00000000e+00f) || (yawing() != 0.00000000e+00f)))
        {
            pitch() += ((pitching() * deltaSecs) * 1.00000000e+02f);
            yaw() += ((yawing() * deltaSecs) * 1.00000000e+02f);
            Mat34 trans = system_api::mat34_rotation(Vec3(system_api::radians(pitch(), entity()), system_api::radians(yaw(), entity()), 0.00000000e+00f), entity());
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 3;

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::transform, kMessageFlag_None, entity().task().id(), entity().task().id(), to_cell(0), blockCount);

                // Write parameters to message
                msgw.insertBlocks(0, trans);

                // MessageQueueWriter will send message through RAII when this scope is exited
            }
        }
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            yawing() = 0.00000000e+00f;
            yaw() = 0.00000000e+00f;
            pitching() = 0.00000000e+00f;
            pitch() = 0.00000000e+00f;
            return MessageResult::Consumed;
        case HASH::init:
        {
            // Params look compatible, message body follows
            system_api::watch_input_state(HASH::mouse_look, 0, HASH::mouse_look, entity());
            system_api::watch_mouse(HASH::mouse_move, HASH::mouse_wheel, entity());
            system_api::watch_input_state(HASH::forward, 0, HASH::forward, entity());
            system_api::watch_input_state(HASH::back, 0, HASH::back, entity());
            system_api::watch_input_state(HASH::left, 0, HASH::left, entity());
            system_api::watch_input_state(HASH::right, 0, HASH::right, entity());
            return MessageResult::Consumed;
        }
        case HASH::forward:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                pitching() = 1.00000000e+00f;
            }
            else
            {
                pitching() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::back:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                pitching() = -(1.00000000e+00f);
            }
            else
            {
                pitching() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::left:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = 1.00000000e+00f;
            }
            else
            {
                yawing() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        case HASH::right:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = -(1.00000000e+00f);
            }
            else
            {
                yawing() = 0.00000000e+00f;
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    gaen__utils__WasdRot(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::gaen__utils__WasdRot);
        mBlockCount = 1;
    }
    gaen__utils__WasdRot(const gaen__utils__WasdRot&)              = delete;
    gaen__utils__WasdRot(const gaen__utils__WasdRot&&)             = delete;
    gaen__utils__WasdRot & operator=(const gaen__utils__WasdRot&)  = delete;
    gaen__utils__WasdRot & operator=(const gaen__utils__WasdRot&&) = delete;

    f32& yawing()
    {
        return mpBlocks[0].cells[0].f;
    }

    f32& yaw()
    {
        return mpBlocks[0].cells[1].f;
    }

    f32& pitching()
    {
        return mpBlocks[0].cells[2].f;
    }

    f32& pitch()
    {
        return mpBlocks[0].cells[3].f;
    }


}; // class gaen__utils__WasdRot

} // namespace comp

void register_component__gaen__utils__WasdRot(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::gaen__utils__WasdRot, comp::gaen__utils__WasdRot::construct))
        PANIC("Unable to register component: gaen__utils__WasdRot");
}

} // namespace gaen

