//------------------------------------------------------------------------------
// start.cpp - Auto-generated from start.cmp
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

// HASH: acd2addef7634df573edcea19064ef05
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/MessageWriter.h"
#include "engine/Task.h"
#include "engine/Registry.h"
#include "engine/Component.h"
#include "engine/Entity.h"

namespace gaen
{

namespace ent
{

class start : public Entity
{
public:
    static Entity * construct(u32 childCount)
    {
        return GNEW(kMEM_Engine, start, childCount);
    }
    
    void update(float deltaSecs)
    {
        float f = deltaSecs;
        int x = 5;
        x = 6;
    }

    template <typename T>
    MessageResult message(const T & msgAcc) { return MessageResult::Propogate; }

private:
    start(u32 childCount)
      : Entity(HASH::start, childCount, 36, 36)
    {
        f_prop() = 1.000000f;
        f_field() = 2.000000f;

        mBlockCount = 1;
        mTask = Task::createUpdatable(this, HASH::start);
    }
    start(const start&)              = delete;
    start(const start&&)             = delete;
    start & operator=(const start&)  = delete;
    start & operator=(const start&&) = delete;

    float& f_prop()
    {
        return mpBlocks[0].cells[0].f;
    }
    float& f_field()
    {
        return mpBlocks[0].cells[1].f;
    }
}; // class start

} // namespace ent

void register_entity_start()
{
    if (!EntityRegistry::register_constructor(HASH::start, ent::start::construct))
        PANIC("Unable to register entity: start");
}

} // namespace gaen

