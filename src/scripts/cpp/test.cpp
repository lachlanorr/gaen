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

// HASH: 4622b2a19801627772c77767626a0835
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
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::mat01:
            {
                u32 requiredBlockCount = 3;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&mat01())[0] = msgAcc[0];
                    reinterpret_cast<Block*>(&mat01())[1] = msgAcc[1];
                    reinterpret_cast<Block*>(&mat01())[2] = msgAcc[2];
                    return MessageResult::Consumed;
                }
            }
            }
            return MessageResult::Propogate; // Invalid property
        }
        return MessageResult::Propogate;
}

private:
    test(u32 childCount)
      : Entity(HASH::test, childCount, 36, 36)
    {
        mat01() = Mat34(1.0f);

        mBlockCount = 3;
        mScriptTask = Task::create(this, HASH::test);
    }
    test(const test&)              = delete;
    test(const test&&)             = delete;
    test & operator=(const test&)  = delete;
    test & operator=(const test&&) = delete;

    Mat34& mat01()
    {
        return *reinterpret_cast<Mat34*>(&mpBlocks[0].qCell);
    }
}; // class test

} // namespace ent

void register_entity_test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test, ent::test::construct))
        PANIC("Unable to register entity: test");
}

} // namespace gaen

