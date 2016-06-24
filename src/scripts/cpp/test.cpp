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

// HASH: 9ad1dfdfa88937a918e285012229384f
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
        case HASH::init_data__:
            ASSERT(initStatus() < kIS_InitData);

            set_foo__path(entity().blockMemory().stringAlloc("/fonts/profont.gatl"));
            foo() = nullptr;
            set_bar__path(entity().blockMemory().stringAlloc("/images/bar.tga"));
            bar() = nullptr;
            set_s(entity().blockMemory().stringAlloc("initval"));
            a() = 0;

            setInitStatus(kIS_InitData);
            return MessageResult::Consumed;
        case HASH::init_assets__:
            ASSERT(initStatus() < kIS_InitAssets);

            entity().requestAsset(mScriptTask.id(), HASH::foo, foo__path());
            entity().requestAsset(mScriptTask.id(), HASH::bar, bar__path());

            setInitStatus(kIS_InitAssets);
            return MessageResult::Consumed;
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
            case HASH::foo__path:
            {
                if (_msg.blockCount < 1) break; // not enough even for BlockData header
                const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[0]);
                if (pBlockData->type != kBKTY_String) break; // incorrect BlockData type
                u32 requiredBlockCount = pBlockData->blockCount;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    Address addr = entity().blockMemory().allocCopy(pBlockData);
                    set_foo__path(entity().blockMemory().string(addr));
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
            return MessageResult::Propogate; // Invalid property
        }
        return MessageResult::Propogate;
}

private:
    test__TestComp(Entity * pEntity)
      : Component(pEntity)
    {
        mScriptTask = Task::create(this, HASH::test__TestComp);
        mBlockCount = 3;
    }
    test__TestComp(const test__TestComp&)              = delete;
    test__TestComp(test__TestComp&&)                   = delete;
    test__TestComp & operator=(const test__TestComp&)  = delete;
    test__TestComp & operator=(test__TestComp&&)       = delete;

    AssetHandleP& foo()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[0].cells[0]);
    }

    CmpStringAsset& foo__path()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[2]);
    }
    bool mIs_foo__path_Assigned = false;
    void release_foo__path()
    {
        if (mIs_foo__path_Assigned)
        {
            entity().blockMemory().release(foo__path());
        }
        mIs_foo__path_Assigned = false;
    }
    void set_foo__path(const CmpStringAsset& rhs)
    {
        ERR("TODO: release block memory strings in #fin message!!!");
        release_foo__path();
        foo__path() = rhs;
        entity().blockMemory().addRef(foo__path());
        mIs_foo__path_Assigned = true;
    }

    AssetHandleP& bar()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[1].cells[0]);
    }

    CmpStringAsset& bar__path()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[1].cells[2]);
    }
    bool mIs_bar__path_Assigned = false;
    void release_bar__path()
    {
        if (mIs_bar__path_Assigned)
        {
            entity().blockMemory().release(bar__path());
        }
        mIs_bar__path_Assigned = false;
    }
    void set_bar__path(const CmpStringAsset& rhs)
    {
        ERR("TODO: release block memory strings in #fin message!!!");
        release_bar__path();
        bar__path() = rhs;
        entity().blockMemory().addRef(bar__path());
        mIs_bar__path_Assigned = true;
    }

    CmpString& s()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[2].cells[0]);
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
        ERR("TODO: release block memory strings in #fin message!!!");
        release_s();
        s() = rhs;
        entity().blockMemory().addRef(s());
        mIs_s_Assigned = true;
    }

    i32& a()
    {
        return mpBlocks[2].cells[2].i;
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
        case HASH::init_data__:
            ASSERT(initStatus() < kIS_InitData);


            setInitStatus(kIS_InitData);
            return MessageResult::Consumed;
        case HASH::init_assets__:
            ASSERT(initStatus() < kIS_InitAssets);


            setInitStatus(kIS_InitAssets);
            return MessageResult::Consumed;
        }
        return MessageResult::Propogate;
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
        case HASH::set_property:
            switch (_msg.payload.u)
            {
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
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringAlloc("init"), entity());
            system_api::print_asset_info(testFoo(), entity());
            return MessageResult::Consumed;
        }
        case HASH::fin:
        {
            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringAlloc("fin"), entity());
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    test__Test(u32 childCount)
      : Entity(HASH::test__Test, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        set_testFoo__path(entity().blockMemory().stringAlloc("/another/asset.foo"));
        testFoo() = nullptr;
        mBlockCount = 1;
        mScriptTask = Task::create(this, HASH::test__Test);

        // Component: TestComp
        {
            Task & compTask = insertComponent(HASH::test__TestComp, mComponentCount);
            // Init Property: foo
            {
                CmpString val = entity().blockMemory().stringAlloc("/some/other/path/foo");
                ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::foo__path), val.blockCount());
                val.writeMessage(msgw.accessor(), 0);
                compTask.message(msgw.accessor());
            }
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
            // Send init_assets message
            StackMessageBlockWriter<0> msgInitAssets(HASH::init_assets__, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInitAssets.accessor());
            // Send init message
            StackMessageBlockWriter<0> msgInit(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgInit.accessor());
        }
    }
    test__Test(const test__Test&)              = delete;
    test__Test(test__Test&&)                   = delete;
    test__Test & operator=(const test__Test&)  = delete;
    test__Test & operator=(test__Test&&)       = delete;

    AssetHandleP& testFoo()
    {
        return *reinterpret_cast<AssetHandleP*>(&mpBlocks[0].cells[0]);
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
        ERR("TODO: release block memory strings in #fin message!!!");
        release_testFoo__path();
        testFoo__path() = rhs;
        entity().blockMemory().addRef(testFoo__path());
        mIs_testFoo__path_Assigned = true;
    }

}; // class test__Test

} // namespace ent

void register_entity__test__Test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Test, ent::test__Test::construct))
        PANIC("Unable to register entity: test__Test");
}

} // namespace gaen

