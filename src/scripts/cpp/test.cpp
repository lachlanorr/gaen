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

// HASH: 3a534e68d3866031bfce5e1cf732977a
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
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

class test__start : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, test__start, childCount);
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
            case HASH::sProp:
            {
                u32 requiredBlockCount = 1;
                if (_msg.blockCount >= requiredBlockCount)
                {
                    reinterpret_cast<Block*>(&sProp())[0].cells[0] = msgAcc[0].cells[0];
                    reinterpret_cast<Block*>(&sProp())[0].cells[1] = msgAcc[0].cells[1];
                    return MessageResult::Consumed;
                }
            }
            }
            return MessageResult::Propogate; // Invalid property
        case HASH::init:
        {
            set_sField(entity().blockMemory().stringAlloc("def"));
            return MessageResult::Consumed;
        }
        }
        return MessageResult::Propogate;
    }

private:
    test__start(u32 childCount)
      : Entity(HASH::test__start, childCount, 36, 36)
    {
        set_sField(entity().blockMemory().stringAlloc(""));
        set_sProp(entity().blockMemory().stringAlloc("prop val as string"));

        mBlockCount = 1;
        mScriptTask = Task::create(this, HASH::test__start);
    }
    test__start(const test__start&)              = delete;
    test__start(const test__start&&)             = delete;
    test__start & operator=(const test__start&)  = delete;
    test__start & operator=(const test__start&&) = delete;

    CmpString& sProp()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[0].cells[0]);
    }
    bool mIs_sProp_Assigned = false;
    void set_sProp(CmpString& rhs)
    {
        if (mIs_sProp_Assigned)
        {
            entity().blockMemory().release(sProp());
        }
        else
        {
            mIs_sProp_Assigned = true;
        }
        sProp() = rhs;
        entity().blockMemory().addRef(sProp());
    }

    CmpString& sField()
    {
        return *reinterpret_cast<CmpString*>(&mpBlocks[0].cells[2]);
    }
    bool mIs_sField_Assigned = false;
    void set_sField(CmpString& rhs)
    {
        if (mIs_sField_Assigned)
        {
            entity().blockMemory().release(sField());
        }
        else
        {
            mIs_sField_Assigned = true;
        }
        sField() = rhs;
        entity().blockMemory().addRef(sField());
    }

}; // class test__start

} // namespace ent

void register_entity__test__start(Registry & registry)
{
    if (!registry.registerEntityConstructor(HASH::test__start, ent::test__start::construct))
        PANIC("Unable to register entity: test__start");
}

} // namespace gaen

