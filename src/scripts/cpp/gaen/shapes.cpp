//------------------------------------------------------------------------------
// shapes.cpp - Auto-generated from shapes.cmp
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

// HASH: ae2a807842f9a127a444b35e9d3d7360
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

class gaen__shapes__Box : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) gaen__shapes__Box(pEntity);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            boxModelUid() = system_api::renderer_gen_uid(entity());
            diffuse() = Color(255, 0, 0, 255);
            model() = Handle::null();
            size() = Vec3(1.00000000e+00f, 1.00000000e+00f, 1.00000000e+00f);
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::diffuse:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&diffuse())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            case HASH::size:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&size())[0].cells[0] = msgAcc[0].cells[0];
                    reinterpret_cast<Block*>(&size())[0].cells[1] = msgAcc[0].cells[1];
                    reinterpret_cast<Block*>(&size())[0].cells[2] = msgAcc[0].cells[2];
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            // Params look compatible, message body follows
            model() = system_api::create_shape_box(size(), diffuse(), entity());
            system_api::renderer_insert_model_instance(boxModelUid(), model(), entity());
            return MessageResult::Consumed;
        }
        case HASH::update_transform:
        {
            // Params look compatible, message body follows
            system_api::renderer_transform_model_instance(boxModelUid(), transform(), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    gaen__shapes__Box(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::gaen__shapes__Box);
        mBlockCount = 4;
    }
    gaen__shapes__Box(const gaen__shapes__Box&)              = delete;
    gaen__shapes__Box(const gaen__shapes__Box&&)             = delete;
    gaen__shapes__Box & operator=(const gaen__shapes__Box&)  = delete;
    gaen__shapes__Box & operator=(const gaen__shapes__Box&&) = delete;

    Vec3& size()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[2].qCell);
    }

    Color& diffuse()
    {
        return mpBlocks[2].cells[3].color;
    }

    Handle& model()
    {
        return *reinterpret_cast<Handle*>(&mpBlocks[0].qCell);
    }

    u32& boxModelUid()
    {
        return mpBlocks[3].cells[0].u;
    }


}; // class gaen__shapes__Box

} // namespace comp

void register_component__gaen__shapes__Box(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::gaen__shapes__Box, comp::gaen__shapes__Box::construct))
        PANIC("Unable to register component: gaen__shapes__Box");
}

} // namespace gaen

