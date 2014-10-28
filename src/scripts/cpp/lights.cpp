//------------------------------------------------------------------------------
// lights.cpp - Auto-generated from lights.cmp
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

// HASH: 6d6c648a4c2158c99aa5f1823db8930c
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

class DirectionalLight : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, DirectionalLight, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init:
        {
            system_api::renderer_insert_light_directional(lightUid(), Vec3(0.100000f, 0.100000f, 0.200000f), Color(255, 255, 255, 255), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    DirectionalLight(u32 childCount)
      : Entity(HASH::DirectionalLight, childCount, 36, 36)
    {
        lightUid() = system_api::renderer_gen_uid(entity());

        mBlockCount = 1;
        mScriptTask = Task::create(this, HASH::DirectionalLight);
    }
    DirectionalLight(const DirectionalLight&)              = delete;
    DirectionalLight(const DirectionalLight&&)             = delete;
    DirectionalLight & operator=(const DirectionalLight&)  = delete;
    DirectionalLight & operator=(const DirectionalLight&&) = delete;

    u32& lightUid()
    {
        return mpBlocks[0].cells[0].u;
    }
}; // class DirectionalLight

} // namespace ent

void register_entity_DirectionalLight(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::DirectionalLight, ent::DirectionalLight::construct))
        PANIC("Unable to register entity: DirectionalLight");
}

} // namespace gaen

