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

// HASH: 353dd616148b96018e4bb950df3d8dd6
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
            ASSERT(initStatus() < kIS_InitData);

            foo() = entity().blockMemory().stringAlloc("/fonts/profont.gatl");
            bar() = entity().blockMemory().stringAlloc("/images/bar.tga");

            setInitStatus(kIS_InitData);
            return MessageResult::Consumed;
        case HASH::init_assets:
            ASSERT(initStatus() < kIS_InitAssets);

            foo() = entity().blockMemory().stringAlloc("/fonts/profont.gatl");
            bar() = entity().blockMemory().stringAlloc("/images/bar.tga");

            setInitStatus(kIS_InitAssets);
            return MessageResult::Consumed;
        case HASH::set_property:
            switch (_msg.payload.u)
            {
            case HASH::foo:
            {
                ERR_IF(initStatus() >= kIS_InitAssets, "Asset property 'foo' set after asset initialization");
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&foo())[0].cells[0] = msgAcc[0].cells[0];
                    reinterpret_cast<Block*>(&foo())[0].cells[1] = msgAcc[0].cells[1];
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
        mBlockCount = 1;
    }
    test__TestComp(const test__TestComp&)              = delete;
    test__TestComp(test__TestComp&&)             = delete;
    test__TestComp & operator=(const test__TestComp&)  = delete;
    test__TestComp & operator=(test__TestComp&&) = delete;

    CmpStringAsset& foo()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[0]);
    }

    CmpStringAsset& bar()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[2]);
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
        case HASH::init_data:
            ASSERT(initStatus() < kIS_InitData);


            setInitStatus(kIS_InitData);
            return MessageResult::Consumed;
        case HASH::init_assets:
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
    test__TestCompEmpty(test__TestCompEmpty&&)             = delete;
    test__TestCompEmpty & operator=(const test__TestCompEmpty&)  = delete;
    test__TestCompEmpty & operator=(test__TestCompEmpty&&) = delete;


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
            case HASH::testFoo:
            {
                ERR_IF(initStatus() >= kIS_InitAssets, "Asset property 'testFoo' set after asset initialization");
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&testFoo())[0].cells[0] = msgAcc[0].cells[0];
                    reinterpret_cast<Block*>(&testFoo())[0].cells[1] = msgAcc[0].cells[1];
                    return MessageResult::Consumed;
                }
                break;
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            ASSERT(initStatus() < kIS_Init);

            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringAlloc("init"), entity());

            setInitStatus(kIS_Init);
            return MessageResult::Consumed;
        }
        case HASH::fin:
        {
            ASSERT(initStatus() < kIS_Fin);

            // Params look compatible, message body follows
            system_api::print(entity().blockMemory().stringAlloc("fin"), entity());

            setInitStatus(kIS_Fin);
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    test__Test(u32 childCount)
      : Entity(HASH::test__Test, childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax
    {
        testFoo() = entity().blockMemory().stringAlloc("/another/asset.foo");
        mBlockCount = 1;
        mScriptTask = Task::create(this, HASH::test__Test);

        // Component: TestComp
        {
            Task & compTask = insertComponent(HASH::test__TestComp, mComponentCount);
            // Init Property: foo
            {
                CmpString val = entity().blockMemory().stringAlloc("/some/other/path/foo");
                ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::foo), val.blockCount());
                val.writeMessage(msgw.accessor(), 0);
                compTask.message(msgw.accessor());
            }
            // Send init message
            StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, compTask.id(), compTask.id(), to_cell(0));
            compTask.message(msgBW.accessor());
        }
    }
    test__Test(const test__Test&)              = delete;
    test__Test(test__Test&&)                   = delete;
    test__Test & operator=(const test__Test&)  = delete;
    test__Test & operator=(test__Test&&)       = delete;

    CmpStringAsset& testFoo()
    {
        return *reinterpret_cast<CmpStringAsset*>(&mpBlocks[0].cells[0]);
    }

}; // class test__Test

} // namespace ent

void register_entity__test__Test(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__Test, ent::test__Test::construct))
        PANIC("Unable to register entity: test__Test");
}

} // namespace gaen

