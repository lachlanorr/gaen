//------------------------------------------------------------------------------
// InputMgr.cpp - Maintains input state for TaskMasters
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

#include "engine/stdafx.h"

#include "engine/MessageWriter.h"
#include "engine/InputMgr.h"

namespace gaen
{

InputMgr::InputMgr()
{
    // LORRTODO - Drive settings from config file
    // These are temp settings for debugging purposes
    registerKeyToState(kKEY_W, HASH::forward);
    registerKeyToState(kKEY_A, HASH::left);
    registerKeyToState(kKEY_S, HASH::back);
    registerKeyToState(kKEY_D, HASH::right);
}

void InputMgr::processKeyInput(const KeyInput & keyInput)
{
    auto keyIt = mKeyToStateMap.find((KeyCode)keyInput.keyCode);
    if (keyIt != mKeyToStateMap.end())
    {
        for (u32 state : keyIt->second)
        {
            auto stateIt = mStateListenerMap.find(state);
            if (stateIt != mStateListenerMap.end())
            {
                for (TaskMessage tm : stateIt->second)
                {
                    MessageQueueWriter msgw(tm.message,
                                            kMessageFlag_None,
                                            kInputMgrTaskId,
                                            tm.taskId,
                                            to_cell((u32)keyInput.keyEvent),
                                            0);
                }
            }
        }
    }
}

void InputMgr::registerKeyToState(KeyCode keyCode, u32 stateHash)
{
    auto & vec = mKeyToStateMap[keyCode];
    if (std::find(vec.begin(), vec.end(), stateHash) == vec.end())
        vec.push_back(stateHash);
}

void InputMgr::registerStateListener(u32 stateHash, TaskMessage taskMessage)
{
    auto & vec = mStateListenerMap[stateHash];
    if (std::find(vec.begin(), vec.end(), taskMessage) == vec.end())
        vec.push_back(taskMessage);
}

template <typename T>
MessageResult InputMgr::message(const T& msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::keyboard_input:
        processKeyInput(KeyInput(msg.payload));
        break;
    case HASH::register_input_state_listener:
        break;
    default:
        PANIC("Unknown InputMgr message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult InputMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult InputMgr::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
