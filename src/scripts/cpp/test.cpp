//------------------------------------------------------------------------------
// test.cpp - Auto-generated from test.cmp
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

// HASH: 3c67c280e3f69b968804acaf1864eaf1
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

class test : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, test, childCount);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        switch(msgAcc.message().msgId)
        {
        case HASH::init:
        {
            system_api::register_input_state_listener(HASH::forward, 0, HASH::yaw_foward, entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
}

private:
    test(u32 childCount)
      : Entity(HASH::test, childCount, 36, 36)
    {

        mBlockCount = 0;
        mTask = Task::create(this, HASH::test);
    }
    test(const test&)              = delete;
    test(const test&&)             = delete;
    test & operator=(const test&)  = delete;
    test & operator=(const test&&) = delete;

}; // class test

} // namespace ent

void register_entity_test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test, ent::test::construct))
        PANIC("Unable to register entity: test");
}

} // namespace gaen

