//------------------------------------------------------------------------------
// start.cpp - Auto-generated from start.cmp
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

// HASH: e48e60f55c244fba4a5a6241e3267ba1
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

class start : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init:
        {
            system_api::renderer_insert_model_instance(boxModelUid(), boxModel(), Mat34(1.000000f), entity());
            system_api::renderer_insert_light_directional(lightUid(), Vec3(0.100000f, 0.100000f, 0.200000f), Color(255, 255, 255, 255), entity());
            return MessageResult::Consumed;
        }
        case HASH::timer:
        {
            if ((/*timer_msg*/msgAcc.message().payload.u == HASH::tick))
            {
                angle() += 2.000000f;
            }
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    start(u32 childCount)
      : Entity(HASH::start, childCount, 36, 36)
    {
        angle() = 0.000000f;
        boxModel() = system_api::create_shape_box(Vec3(1.000000f, 1.000000f, 1.000000f), Color(0, 0, 255, 255), entity());
        boxModelUid() = system_api::renderer_gen_uid(entity());
        lightUid() = system_api::renderer_gen_uid(entity());

        mBlockCount = 3;
        mScriptTask = Task::create(this, HASH::start);

        // Component: Timer
        {
            Task & compTask = insertComponent(HASH::Timer, mComponentCount);
            // Init Property: timer_interval
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_interval));
                msgw[0].cells[0].f = 0.016000f;
                compTask.message(msgw.accessor());
            }
            // Init Property: timer_message
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::timer_message));
                msgw[0].cells[0].u = HASH::tick;
                compTask.message(msgw.accessor());
            }
        }

        // Component: WasdRot
        {
            Task & compTask = insertComponent(HASH::WasdRot, mComponentCount);
            // Init Property: wasdrot_modeluid
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::wasdrot_modeluid));
                msgw[0].cells[0].u = boxModelUid();
                compTask.message(msgw.accessor());
            }
        }
    }
    start(const start&)              = delete;
    start(const start&&)             = delete;
    start & operator=(const start&)  = delete;
    start & operator=(const start&&) = delete;

    f32& angle()
    {
        return mpBlocks[2].cells[0].f;
    }
    Handle& boxModel()
    {
        return *reinterpret_cast<Handle*>(&mpBlocks[0].qCell);
    }
    u32& boxModelUid()
    {
        return mpBlocks[2].cells[1].u;
    }
    u32& lightUid()
    {
        return mpBlocks[2].cells[2].u;
    }
}; // class start

} // namespace ent

void register_entity_start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::start, ent::start::construct))
        PANIC("Unable to register entity: start");
}

} // namespace gaen

