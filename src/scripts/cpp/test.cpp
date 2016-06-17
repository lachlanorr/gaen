//------------------------------------------------------------------------------
// test.cpp - Auto-generated from test.cmp
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

// HASH: 7e958f41447ecabc43394a5c66751d95
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

class test__TestComp : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) test__TestComp(pEntity);
    }
    
    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init_data:
            foo() = entity().blockMemory().stringAlloc("/fonts/profont.gatl");
            bar() = entity().blockMemory().stringAlloc("/images/bar.tga");
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::foo:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&foo())[0] = msgAcc[0];
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
    test__TestComp(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::test__TestComp);
        mBlockCount = 2;
    }
    test__TestComp(const test__TestComp&)              = delete;
    test__TestComp(test__TestComp&&)             = delete;
    test__TestComp & operator=(const test__TestComp&)  = delete;
    test__TestComp & operator=(test__TestComp&&) = delete;

    Handle& foo()
    {
        return *reinterpret_cast<Handle*>(&mpBlocks[0].qCell);
    }

    Handle& bar()
    {
        return *reinterpret_cast<Handle*>(&mpBlocks[1].qCell);
    }


}; // class test__TestComp

} // namespace comp

void register_component__test__TestComp(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::test__TestComp, comp::test__TestComp::construct))
        PANIC("Unable to register component: test__TestComp");
}

namespace ent
{

class test__Test : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, test__Test, childCount);
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
            case HASH::prop1:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&prop1())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            case HASH::prop2:
            {
                if (_msg.blockCount < 1) break; // not enough even for BlockData header
                const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[0]);
                if (pBlockData->type != kBKTY_String) break; // incorrect BlockData type
                u32 requiredBlockCount = pBlockData->blockCount;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    Address addr = entity().blockMemory().allocCopy(pBlockData);
                    set_prop2(entity().blockMemory().string(addr));
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::msg1:
        {
            // Verify params look compatible with this message type
            u32 expectedBlockSize = 2; // BlockCount without BlockMemory params
            if (expectedBlockSize > msgAcc.available())
                return MessageResult::Propogate;

            // Check that block memory params exist in the message
            u16 blockMemCount = 0;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propogate;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propogate;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propogate;


            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringFormat("prop1 = %d", prop1()), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    test__Test(u32 childCount)
      : Entity(HASH::test__Test, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        prop1() = 20;
        set_prop2(entity().blockMemory().stringAlloc("abc"));
        mBlockCount = 1;
        mScriptTask = Task::create(this, HASH::test__Test);
    }
    test__Test(const test__Test&)              = delete;
    test__Test(test__Test&&)                   = delete;
    test__Test & operator=(const test__Test&)  = delete;
    test__Test & operator=(test__Test&&)       = delete;

    i32& prop1()
    {
        return mpBlocks[0].cells[2].i;
    }

    CmpString& prop2()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[0].cells[0]);
    }
    bool mIs_prop2_Assigned = false;
    void set_prop2(const CmpString& rhs)
    {
        if (mIs_prop2_Assigned)
        {
            entity().blockMemory().release(prop2());
        }
        else
        {
            mIs_prop2_Assigned = true;
        }
        prop2() = rhs;
        entity().blockMemory().addRef(prop2());
    }

}; // class test__Test

} // namespace ent

void register_entity__test__Test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Test, ent::test__Test::construct))
        PANIC("Unable to register entity: test__Test");
}

} // namespace gaen

