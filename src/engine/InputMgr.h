//------------------------------------------------------------------------------
// InputMgr.h - Maintains input state for TaskMasters
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_ENGINE_INPUT_MGR_H
#define GAEN_ENGINE_INPUT_MGR_H

#include "core/HashMap.h"
#include "core/Vector.h"

#include "engine/MessageAccessor.h"
#include "engine/input.h"

namespace gaen
{

class InputMgr
{
public:
    InputMgr();

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    struct TaskMessage
    {
        u32 taskId:28;
        u32 deviceId:4;
        u32 message;
        
        TaskMessage(u32 taskId, u32 message)
          : taskId(taskId)
          , message(message)
        {}

        bool operator==(const TaskMessage & rhs) const
        {
            static_assert(sizeof(*this) == 8, "Tricky comparison requires 8 byte size");
            return *(const u64*)this == *(const u64*)&rhs;
        }
    };

    void processKeyInput(const KeyInput & keyInput);

    void registerKeyToState(KeyCode keyCode, u32 stateHash);
    void registerStateListener(u32 stateHash, TaskMessage taskMessage);

    HashMap<kMEM_Engine, KeyCode, Vector<kMEM_Engine, u32>> mKeyToStateMap;
    HashMap<kMEM_Engine, u32, Vector<kMEM_Engine, TaskMessage>> mStateListenerMap;

};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_INPUT_MGR_H
