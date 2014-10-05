//------------------------------------------------------------------------------
// Box.cpp - Auto-generated from Box.cmp
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

// HASH: cf33bbf2446606976326901def083635
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

class Box : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, Box, childCount);
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
            case HASH::col:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&col())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            model() = system_api::create_shape_box(size(), col(), entity());
            system_api::renderer_insert_model_instance(boxModelUid(), model(), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    Box(u32 childCount)
      : Entity(HASH::Box, childCount, 36, 36)
    {
        col() = Color(255, 0, 0, 255);
        model() = Handle::null();
        boxModelUid() = system_api::renderer_gen_uid(entity());

        mBlockCount = 4;
        mScriptTask = Task::create(this, HASH::Box);
    }
    Box(const Box&)              = delete;
    Box(const Box&&)             = delete;
    Box & operator=(const Box&)  = delete;
    Box & operator=(const Box&&) = delete;

    Vec3& size()
    {
        return *reinterpret_cast<Vec3*>(&mpBlocks[2].qCell);
    }
    Color& col()
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
}; // class Box

} // namespace ent

void register_entity_Box(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::Box, ent::Box::construct))
        PANIC("Unable to register entity: Box");
}

} // namespace gaen

