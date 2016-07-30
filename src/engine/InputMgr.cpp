//------------------------------------------------------------------------------
// InputMgr.cpp - Maintains input state for TaskMasters
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

#include "engine/stdafx.h"

#include "core/logging.h"

#include "engine/MessageWriter.h"
#include "engine/messages/WatchInputState.h"
#include "engine/messages/WatchMouse.h"
#include "engine/messages/MouseMove.h"

#include "engine/InputMgr.h"

namespace gaen
{

InputMgr::InputMgr()
{
    // LORRTODO - Drive settings from config file
    // These are temp settings for debugging purposes
    registerKeyToState(kKEY_W, HASH::move_up);
    registerKeyToState(kKEY_A, HASH::move_left);
    registerKeyToState(kKEY_S, HASH::move_down);
    registerKeyToState(kKEY_D, HASH::move_right);

    registerKeyToState(kKEY_O,         HASH::shoot_up);
    registerKeyToState(kKEY_K,         HASH::shoot_left);
    registerKeyToState(kKEY_L,         HASH::shoot_down);
    registerKeyToState(kKEY_Semicolon, HASH::shoot_right);

    registerKeyToState(kKEY_Mouse2, HASH::mouse_look);

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
                for (TaskStateMessage tm : stateIt->second)
                {
                    u32 message = keyInput.keyEvent ? tm.downMessage : tm.upMessage;
                    u32 value = keyInput.keyEvent ? tm.downValue : tm.upValue;
                    MessageQueueWriter msgw(message,
                                            kMessageFlag_None,
                                            kInputMgrTaskId,
                                            tm.taskId,
                                            to_cell(value),
                                            0);
                }
            }
        }
    }
}

void InputMgr::processMouseMoveInput(const MouseInput::Movement & moveInput)
{
    if (moveInput.xDelta != 0 && moveInput.yDelta != 0)
    {
        for (TaskMessage tm : mMouseMoveListeners)
        {
            messages::MouseMoveQW msgQW(HASH::mouse_move,
                                        kMessageFlag_None,
                                        kInputMgrTaskId,
                                        tm.taskId,
                                        moveInput.xDelta);
            msgQW.setYDelta(moveInput.yDelta);
        }
    }
}

void InputMgr::processMouseWheelInput(i32 delta)
{
    if (delta != 0)
    {
        for (TaskMessage tm : mMouseWheelListeners)
        {
            MessageQueueWriter msgw(tm.message,
                                    kMessageFlag_None,
                                    kInputMgrTaskId,
                                    tm.taskId,
                                    to_cell(delta),
                                    0);
        }
    }
}

void InputMgr::registerKeyToState(KeyCode keyCode, u32 stateHash)
{
    auto & vec = mKeyToStateMap[keyCode];
    if (std::find(vec.begin(), vec.end(), stateHash) == vec.end())
        vec.push_back(stateHash);
}

void InputMgr::registerStateListener(u32 stateHash, const TaskStateMessage & taskMessage)
{
    auto & vec = mStateListenerMap[stateHash];
    if (std::find(vec.begin(), vec.end(), taskMessage) == vec.end())
        vec.push_back(taskMessage);
}

void InputMgr::registerMouseListener(const TaskMessage & moveMessage, const TaskMessage & wheelMessage)
{
    if (moveMessage.message != 0)
    {
        mMouseMoveListeners.push_back(moveMessage);
    }
    if (wheelMessage.message != 0)
    {
        mMouseWheelListeners.push_back(wheelMessage);
    }
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
    case HASH::watch_input_state:
    {
        messages::WatchInputStateR<T> msgr(msgAcc);
        registerStateListener(msgr.state(), TaskStateMessage(msg.source, msgr.downMessage(), msgr.downValue(), msgr.upMessage(), msgr.upValue()));
        break;
    }
    case HASH::watch_mouse:
    {
        messages::WatchMouseR<T> msgr(msgAcc);
        registerMouseListener(TaskMessage(msg.source, msgr.moveMessage()), TaskMessage(msg.source, msgr.wheelMessage()));
    }
    case HASH::mouse_move:
        processMouseMoveInput(MouseInput::Movement(msg.payload));
        break;
    case HASH::mouse_wheel:
        processMouseWheelInput(msg.payload.i);
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
