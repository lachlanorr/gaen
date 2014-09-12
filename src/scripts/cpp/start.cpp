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

// HASH: 602a687cd280f9ba07b660cee8694aa2
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
    
    void update(float deltaSecs)
    {
        f32 f = deltaSecs;
        i32 x = 5;
        x = 6;
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        switch(msgAcc.message().msgId)
        {
        case HASH::set_property__uint:
            switch (msgAcc.message().payload.u)
            {
            case HASH::boxModelUid:
                boxModelUid() = *reinterpret_cast<const u32*>(&msgAcc[0].cells[0].u);
                return MessageResult::Consumed;
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::set_property__float:
            switch (msgAcc.message().payload.u)
            {
            case HASH::f_prop:
                f_prop() = *reinterpret_cast<const f32*>(&msgAcc[0].cells[0].u);
                return MessageResult::Consumed;
            case HASH::f_field:
                f_field() = *reinterpret_cast<const f32*>(&msgAcc[0].cells[0].u);
                return MessageResult::Consumed;
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::set_property__handle:
            switch (msgAcc.message().payload.u)
            {
            case HASH::boxModel:
                boxModel() = *reinterpret_cast<const Handle*>(&msgAcc[0].cells[0].u);
                return MessageResult::Consumed;
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            system_api::renderer_insert_model_instance(boxModelUid(), boxModel(), Mat34(1.000000f), entity());
        }
        }
        return MessageResult::Propogate;
}

private:
    start(u32 childCount)
      : Entity(HASH::start, childCount, 36, 36)
    {
        f_prop() = 1.000000f;
        f_field() = 2.000000f;
        boxModel() = system_api::create_shape_box(Vec3(1.000000f, 1.000000f, 1.000000f), Color(0, 0, 255, 255), entity());
        boxModelUid() = system_api::renderer_gen_uid(entity());

        mBlockCount = 3;
        mTask = Task::createUpdatable(this, HASH::start);

        // Component: Timer
        {
            Task & compTask = insertComponent(HASH::Timer, mComponentCount);
            // Init Property: timer_interval
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property__float, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(HASH::timer_interval));
                msgw[0].cells[0].f = 1.000000f;
                compTask.message(msgw.accessor());
            }
            // Init Property: timer_message
            {
                StackMessageBlockWriter<1> msgw(HASH::set_property__uint, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(HASH::timer_message));
                msgw[0].cells[0].u = HASH::msg;
                compTask.message(msgw.accessor());
            }
        }
    }
    start(const start&)              = delete;
    start(const start&&)             = delete;
    start & operator=(const start&)  = delete;
    start & operator=(const start&&) = delete;

    f32& f_prop()
    {
        return mpBlocks[2].cells[0].f;
    }
    f32& f_field()
    {
        return mpBlocks[2].cells[1].f;
    }
    Handle& boxModel()
    {
        return *reinterpret_cast<Handle*>(&mpBlocks[0].qCell);
    }
    u32& boxModelUid()
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

