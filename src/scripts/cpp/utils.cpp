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

// HASH: ee01f7fc0da702619bac80593545afaf
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

namespace comp
{

class utils__Timer : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) utils__Timer(pEntity);
    }
    
    void update(float deltaSecs)
    {
        if ((timer_interval() > 0.00000000e+000f))
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
                last_notification() = 0.00000000e+000f;
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
            last_notification() = 0.00000000e+000f;
            timer_interval() = 0.00000000e+000f;
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
    utils__Timer(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::utils__Timer);
        mBlockCount = 1;
    }
    utils__Timer(const utils__Timer&)              = delete;
    utils__Timer(const utils__Timer&&)             = delete;
    utils__Timer & operator=(const utils__Timer&)  = delete;
    utils__Timer & operator=(const utils__Timer&&) = delete;

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


}; // class utils__Timer

} // namespace comp

void register_component__utils__Timer(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::utils__Timer, comp::utils__Timer::construct))
        PANIC("Unable to register component: utils__Timer");
}

namespace comp
{

class utils__WasdRot : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) utils__WasdRot(pEntity);
    }
    
    void update(float deltaSecs)
    {
        if (((pitching() != 0.00000000e+000f) || (yawing() != 0.00000000e+000f)))
        {
            pitch() += ((pitching() * deltaSecs) * 1.00000000e+002f);
            yaw() += ((yawing() * deltaSecs) * 1.00000000e+002f);
            Mat34 trans = system_api::transform_rotate(Vec3(system_api::radians(pitch(), entity()), system_api::radians(yaw(), entity()), 0.00000000e+000f), entity());
            { // Send Message Block
                // Compute block size, incorporating any BlockMemory parameters dynamically
                u32 blockCount = 3;

                // Prepare the queue writer
                MessageQueueWriter msgw(HASH::transform, kMessageFlag_None, entity().task().id(), entity().task().id(), to_cell(0), blockCount);

                // Write parameters to message
                *reinterpret_cast<Mat34*>(&msgw[0].cells[0]) = trans;

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
            pitch() = 0.00000000e+000f;
            pitching() = 0.00000000e+000f;
            yaw() = 0.00000000e+000f;
            yawing() = 0.00000000e+000f;
            return MessageResult::Consumed;
        case HASH::init:
        {
            // Params look compatible, message body follows
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
                pitching() = 1.00000000e+000f;
            }
            else
            {
                pitching() = 0.00000000e+000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::back:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                pitching() = -(1.00000000e+000f);
            }
            else
            {
                pitching() = 0.00000000e+000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::left:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = 1.00000000e+000f;
            }
            else
            {
                yawing() = 0.00000000e+000f;
            }
            return MessageResult::Consumed;
        }
        case HASH::right:
        {
            // Params look compatible, message body follows
            if (/*status*/msgAcc.message().payload.b)
            {
                yawing() = -(1.00000000e+000f);
            }
            else
            {
                yawing() = 0.00000000e+000f;
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    utils__WasdRot(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create_updatable(this, HASH::utils__WasdRot);
        mBlockCount = 1;
    }
    utils__WasdRot(const utils__WasdRot&)              = delete;
    utils__WasdRot(const utils__WasdRot&&)             = delete;
    utils__WasdRot & operator=(const utils__WasdRot&)  = delete;
    utils__WasdRot & operator=(const utils__WasdRot&&) = delete;

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


}; // class utils__WasdRot

} // namespace comp

void register_component__utils__WasdRot(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::utils__WasdRot, comp::utils__WasdRot::construct))
        PANIC("Unable to register component: utils__WasdRot");
}

} // namespace gaen

