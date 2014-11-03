//------------------------------------------------------------------------------
// shapes.cpp - Auto-generated from shapes.cmp
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

// HASH: 3c061b4e72c6da7ff9deee470452fba9
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

class shapes__Box : public Entity
{
private:
// Entity initializer helper functions

public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, shapes__Box, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
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
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            model() = system_api::create_shape_box(size(), diffuse(), entity());
            system_api::renderer_insert_model_instance(boxModelUid(), model(), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    shapes__Box(u32 childCount)
      : Entity(HASH::shapes__Box, childCount, 36, 36)
    {
        boxModelUid() = system_api::renderer_gen_uid(entity());
        diffuse() = Color(255, 0, 0, 255);
        model() = Handle::null();
        size() = Vec3(1.000000f, 1.000000f, 1.000000f);

        mBlockCount = 4;
        mScriptTask = Task::create(this, HASH::shapes__Box);
    }
    shapes__Box(const shapes__Box&)              = delete;
    shapes__Box(const shapes__Box&&)             = delete;
    shapes__Box & operator=(const shapes__Box&)  = delete;
    shapes__Box & operator=(const shapes__Box&&) = delete;

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
}; // class shapes__Box

} // namespace ent

void register_entity__shapes__Box(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::shapes__Box, ent::shapes__Box::construct))
        PANIC("Unable to register entity: shapes__Box");
}

} // namespace gaen

