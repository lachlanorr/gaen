//------------------------------------------------------------------------------
// lights.cpp - Auto-generated from lights.cmp
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

// HASH: 202ab314dd88f99416b69b2933099e82
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

class gaen__lights__Directional : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) gaen__lights__Directional(pEntity);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            lightUid() = system_api::renderer_gen_uid(entity());
            dir() = glm::vec3(1.00000001e-01f, 1.00000001e-01f, 2.00000003e-01f);
            col() = Color(255, 255, 255, 255);
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::col:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&col())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            case HASH::dir:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&dir())[0].cells[0] = msgAcc[0].cells[0];
                    reinterpret_cast<Block*>(&dir())[0].cells[1] = msgAcc[0].cells[1];
                    reinterpret_cast<Block*>(&dir())[0].cells[2] = msgAcc[0].cells[2];
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            // Params look compatible, message body follows
            system_api::renderer_insert_light_directional(lightUid(), dir(), col(), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    gaen__lights__Directional(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::gaen__lights__Directional);
        mBlockCount = 2;
    }
    gaen__lights__Directional(const gaen__lights__Directional&)              = delete;
    gaen__lights__Directional(gaen__lights__Directional&&)             = delete;
    gaen__lights__Directional & operator=(const gaen__lights__Directional&)  = delete;
    gaen__lights__Directional & operator=(gaen__lights__Directional&&) = delete;

    u32& lightUid()
    {
        return mpBlocks[0].cells[3].u;
    }

    glm::vec3& dir()
    {
        return *reinterpret_cast<glm::vec3*>(&mpBlocks[0].qCell);
    }

    Color& col()
    {
        return mpBlocks[1].cells[0].color;
    }


}; // class gaen__lights__Directional

} // namespace comp

void register_component__gaen__lights__Directional(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::gaen__lights__Directional, comp::gaen__lights__Directional::construct))
        PANIC("Unable to register component: gaen__lights__Directional");
}

} // namespace gaen

