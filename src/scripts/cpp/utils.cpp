//------------------------------------------------------------------------------
// utils.cpp - Auto-generated from utils.cmp
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

// HASH: 5c77ba0b7cf31ccff8b3479d4e0d60be
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

namespace comp
{

class Timer : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) Timer(pEntity);
    }
    
    void update(float deltaSecs)
    {
        if ((timer_interval() > 0.000000f))
        {
            last_notification() += deltaSecs;
            if ((last_notification() >= timer_interval()))
            {
                {
                    StackMessageBlockWriter<0> msgw(HASH::timer, kMessageFlag_None, mpEntity->task().id(), mpEntity->task().id(), to_cell(timer_message()));
                    mpEntity->message(msgw.accessor());
                }
                last_notification() = 0.000000f;
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
            last_notification() = 0.000000f;
            timer_interval() = 0.000000f;
            timer_message() = 0;
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
            }
            case HASH::timer_message:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&timer_message())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
            }
            }
            return MessageResult::Propogate; // Invalid property
        }
        return MessageResult::Propogate;
}

private:
    Timer(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::Timer);
        mBlockCount = 1;
    }
    Timer(const Timer&)              = delete;
    Timer(const Timer&&)             = delete;
    Timer & operator=(const Timer&)  = delete;
    Timer & operator=(const Timer&&) = delete;

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

}; // class Timer

} // namespace comp

void register_component_Timer(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::Timer, comp::Timer::construct))
        PANIC("Unable to register component: Timer");
}

namespace comp
{

class WasdRot : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) WasdRot(pEntity);
    }
    
    void update(float deltaSecs)
    {
        pitch() += ((pitching() * deltaSecs) * 100.000000f);
        yaw() += ((yawing() * deltaSecs) * 100.000000f);
        Mat34 transform = system_api::transform_rotate(Vec3(system_api::radians(pitch(), entity()), system_api::radians(yaw(), entity()), 0.000000f), entity());
        system_api::renderer_transform_model_instance(wasdrot_modeluid(), transform, entity());
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            pitch() = 0.000000f;
            pitching() = 0.000000f;
            wasdrot_modeluid() = 0;
            yaw() = 0.000000f;
            yawing() = 0.000000f;
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::wasdrot_modeluid:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&wasdrot_modeluid())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            system_api::watch_input_state(HASH::forward, 0, HASH::forward, entity());
            system_api::watch_input_state(HASH::back, 0, HASH::back, entity());
            system_api::watch_input_state(HASH::left, 0, HASH::left, entity());
            system_api::watch_input_state(HASH::right, 0, HASH::right, entity());
            return MessageResult::Consumed;
        }
        case HASH::forward:
        {
            if (/*status*/msgAcc.message().payload.b)
            {
                pitching() = 1.000000f;
            }
            else
            {
                pitching() = 0.000000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::back:
        {
            if (/*status*/msgAcc.message().payload.b)
            {
                pitching() = -(1.000000f);
            }
            else
            {
                pitching() = 0.000000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::left:
        {
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = 1.000000f;
            }
            else
            {
                yawing() = 0.000000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::right:
        {
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = -(1.000000f);
            }
            else
            {
                yawing() = 0.000000f;
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    WasdRot(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::WasdRot);
        mBlockCount = 2;
    }
    WasdRot(const WasdRot&)              = delete;
    WasdRot(const WasdRot&&)             = delete;
    WasdRot & operator=(const WasdRot&)  = delete;
    WasdRot & operator=(const WasdRot&&) = delete;

    u32& wasdrot_modeluid()
    {
        return mpBlocks[0].cells[0].u;
    }
    f32& yawing()
    {
        return mpBlocks[0].cells[1].f;
    }
    f32& yaw()
    {
        return mpBlocks[0].cells[2].f;
    }
    f32& pitching()
    {
        return mpBlocks[0].cells[3].f;
    }
    f32& pitch()
    {
        return mpBlocks[1].cells[0].f;
    }

}; // class WasdRot

} // namespace comp

void register_component_WasdRot(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::WasdRot, comp::WasdRot::construct))
        PANIC("Unable to register component: WasdRot");
}

} // namespace gaen

