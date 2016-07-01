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

// HASH: ad8ca7d1d9fcf5b2a7fee537ce90c18b
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Handle.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

#include "engine/messages/Handle.h"

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
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            set_s(entity().blockMemory().stringAlloc("initval"));
            a() = 0;
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            release_s();
            return MessageResult::Consumed;
        } // HASH::fin__
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::a:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&a())[0].cells[0] = msgAcc[0].cells[0];
                    return MessageResult::Consumed;
                }
                break;
            }
            case HASH::s:
            {
                if (_msg.blockCount < 1) break; // not enough even for BlockData header
                const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[0]);
                if (pBlockData->type != kBKTY_String) break; // incorrect BlockData type
                u32 requiredBlockCount = pBlockData->blockCount;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    Address addr = entity().blockMemory().allocCopy(pBlockData);
                    set_s(entity().blockMemory().string(addr));
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propagate; // Invalid property
        }
        return MessageResult::Propagate;
}

private:
    test__TestComp(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::test__TestComp);
        mBlockCount = 1;
    }
    test__TestComp(const test__TestComp&)              = delete;
    test__TestComp(test__TestComp&&)                   = delete;
    test__TestComp & operator=(const test__TestComp&)  = delete;
    test__TestComp & operator=(test__TestComp&&)       = delete;

    CmpString& s()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[0].cells[0]);
    }
    bool mIs_s_Assigned = false;
    void release_s()
    {
        if (mIs_s_Assigned)
        {
            entity().blockMemory().release(s());
        }
        mIs_s_Assigned = false;
    }
    void set_s(const CmpString& rhs)
    {
        release_s();
        s() = rhs;
        entity().blockMemory().addRef(s());
        mIs_s_Assigned = true;
    }

    i32& a()
    {
        return mpBlocks[0].cells[2].i;
    }


}; // class test__TestComp

} // namespace comp

void register_component__test__TestComp(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::test__TestComp, comp::test__TestComp::construct))
        PANIC("Unable to register component: test__TestComp");
}

namespace comp
{

class test__TestCompEmpty : public Component
{
public:
    static Component * construct(void * place, Entity * pEntity)
    {
        return new (place) test__TestCompEmpty(pEntity);
    }

    template <typename T>
    MessageResult message(const T & msgAcc)
    {
        const Message & _msg = msgAcc.message();
        switch(_msg.msgId)
        {
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            return MessageResult::Consumed;
        } // HASH::fin__
        }
        return MessageResult::Propagate;
}

private:
    test__TestCompEmpty(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::test__TestCompEmpty);
        mBlockCount = 0;
    }
    test__TestCompEmpty(const test__TestCompEmpty&)              = delete;
    test__TestCompEmpty(test__TestCompEmpty&&)                   = delete;
    test__TestCompEmpty & operator=(const test__TestCompEmpty&)  = delete;
    test__TestCompEmpty & operator=(test__TestCompEmpty&&)       = delete;


}; // class test__TestCompEmpty

} // namespace comp

void register_component__test__TestCompEmpty(Registry & registry)
{
    if (!registry.registerComponentConstructor(HASH::test__TestCompEmpty, comp::test__TestCompEmpty::construct))
        PANIC("Unable to register component: test__TestCompEmpty");
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
        case HASH::init__:
        {
            // Initialize properties and fields to default values
            set_testFoo__path(entity().blockMemory().stringAlloc("/fonts/profont.gimg"));
            testFoo() = nullptr;
            prop1() = 20;
            set_prop2(entity().blockMemory().stringAlloc("abc"));
            // Component: TestComp
            {
                Task & compTask = insertComponent(HASH::test__TestComp, mComponentCount);
                compTask.message(msgAcc); // propagate init__ into component
                // Init Property: s
                {
                    CmpString val = entity().blockMemory().stringAlloc("Changedval");
                    ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::s), val.blockCount());
                    val.writeMessage(msgw.accessor(), 0);
                    compTask.message(msgw.accessor());
                }
                // Init Property: a
                {
                    StackMessageBlockWriter<1> msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::a));
                    msgw[0].cells[0].i = 5;
                    compTask.message(msgw.accessor());
                }
            }
            return MessageResult::Consumed;
        } // HASH::init__
        case HASH::request_assets__:
        {
            // Request any assets we require
            entity().requestAsset(mScriptTask.id(), HASH::testFoo, testFoo__path());
            return MessageResult::Consumed;
        } // HASH::request_assets__
        case HASH::asset_ready__:
        {
            // Asset is loaded and a handle to it has been sent to us
            {
                messages::HandleR<T> msgr(msgAcc);
                ASSERT(msgr.taskId() == task().id());
                switch (msgr.nameHash())
                {
                case HASH::testFoo:
                    testFoo() = msgr.handle();
                default:
                    ERR("Invalid asset nameHash: %u", msgr.nameHash());
                }
            }
            return MessageResult::Consumed;
        } // HASH::assets_ready__
        case HASH::fin__:
        {
            // Release any block data or handle fields and properties
            release_testFoo__path();
            release_testFoo();
            release_prop2();
            return MessageResult::Consumed;
        } // HASH::fin__
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
            case HASH::testFoo__path:
            {
                if (_msg.blockCount < 1) break; // not enough even for BlockData header
                const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[0]);
                if (pBlockData->type != kBKTY_String) break; // incorrect BlockData type
                u32 requiredBlockCount = pBlockData->blockCount;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    Address addr = entity().blockMemory().allocCopy(pBlockData);
                    set_testFoo__path(entity().blockMemory().string(addr));
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propagate; // Invalid property
        case HASH::msg1:
        {
            // Verify params look compatible with this message type
            u32 expectedBlockSize = 2; // BlockCount without BlockMemory params
            if (expectedBlockSize > msgAcc.available())
                return MessageResult::Propagate;

            // Check that block memory params exist in the message
            u16 blockMemCount = 0;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propagate;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propagate;

            blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], kBKTY_String);
            expectedBlockSize += blockMemCount;
            if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())
                return MessageResult::Propagate;


            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringFormat("prop1 = %d", prop1()), entity());
            return MessageResult::Consumed;
        } // HASH::msg1
        case HASH::init:
        {
            // Params look compatible, message body follows
            glm::vec3 v0;
            v0.x = 5.00000000e+00f;
            system_api::print(entity().blockMemory().stringFormat("x = %f", v0.x), entity());
            system_api::print(entity().blockMemory().stringFormat("prop1 = %d, prop2 = \"%s\"", prop1(), prop2().c_str()), entity());
            system_api::print(entity().blockMemory().stringAlloc("init"), entity());
            system_api::print_asset_info(testFoo(), entity());
            return MessageResult::Consumed;
        } // HASH::init
        case HASH::fin:
        {
            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringAlloc("fin"), entity());
            return MessageResult::Consumed;
        } // HASH::fin
        }
        return MessageResult::Propagate;
    }

private:
    test__Test(u32 childCount)
      : Entity(HASH::test__Test, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        mBlockCount = 2;
        mScriptTask = Task::create(this, HASH::test__Test);
    }

    test__Test(const test__Test&)              = delete;
    test__Test(test__Test&&)                   = delete;
    test__Test & operator=(const test__Test&)  = delete;
    test__Test & operator=(test__Test&&)       = delete;

    AssetHandleP& testFoo()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[0].cells[0]);
    }
    void release_testFoo()
    {
        if (testFoo() != nullptr)
        {
            testFoo()->release();
        }
    }

    CmpStringAsset& testFoo__path()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[2]);
    }
    bool mIs_testFoo__path_Assigned = false;
    void release_testFoo__path()
    {
        if (mIs_testFoo__path_Assigned)
        {
            entity().blockMemory().release(testFoo__path());
        }
        mIs_testFoo__path_Assigned = false;
    }
    void set_testFoo__path(const CmpStringAsset& rhs)
    {
        release_testFoo__path();
        testFoo__path() = rhs;
        entity().blockMemory().addRef(testFoo__path());
        mIs_testFoo__path_Assigned = true;
    }

    i32& prop1()
    {
        return mpBlocks[1].cells[2].i;
    }

    CmpString& prop2()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[1].cells[0]);
    }
    bool mIs_prop2_Assigned = false;
    void release_prop2()
    {
        if (mIs_prop2_Assigned)
        {
            entity().blockMemory().release(prop2());
        }
        mIs_prop2_Assigned = false;
    }
    void set_prop2(const CmpString& rhs)
    {
        release_prop2();
        prop2() = rhs;
        entity().blockMemory().addRef(prop2());
        mIs_prop2_Assigned = true;
    }

}; // class test__Test

} // namespace ent

void register_entity__test__Test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Test, ent::test__Test::construct))
        PANIC("Unable to register entity: test__Test");
}

} // namespace gaen

