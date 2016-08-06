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

#include "assets/Config.h"

#include "engine/MessageWriter.h"
#include "engine/messages/WatchInputState.h"
#include "engine/messages/WatchMouse.h"
#include "engine/messages/MouseMove.h"

#include "engine/InputMgr.h"

namespace gaen
{

static const char * kKeyboardConf = "keyboard.conf";

InputMgr::InputMgr(bool isPrimary)
  : mIsPrimary(isPrimary)
{
    mPressedKeys = glm::uvec4(0);

    Config<kMEM_Engine> keyConf;

    mActiveKeyboardConfig = 0;
    if (keyConf.read(kKeyboardConf))
    {
        for (auto secIt = keyConf.sectionsBegin();
             secIt != keyConf.sectionsEnd();
             ++secIt)
        {
            u32 secHash = HASH::hash_func(*secIt);

            if (mActiveKeyboardConfig == 0)
                mActiveKeyboardConfig = secHash;

            for (auto keyIt = keyConf.keysBegin(*secIt);
                 keyIt != keyConf.keysEnd(*secIt);
                 ++keyIt)
            {
                u32 keyHash = HASH::hash_func(*keyIt);

                glm::uvec4 keys;
                auto keyVec = keyConf.getVec(*secIt, *keyIt);
                u32 keyCount = glm::min(4, (i32)keyVec.size());
                for (u32 i = 0; i < keyCount; ++i)
                {
                    keys[i] = lookup_key_code(keyVec[i]);
                }
                for (u32 i = keyCount; i < 4; ++i)
                {
                    keys[i] = kKEY_NOKEY;
                }
                mKeyConfigs[secHash][keyHash] = keys;
            }
        }
    }

    // LORRTODO - Drive mouse settings from config file
    registerKeyToState(kKEY_Mouse2, HASH::mouse_look);
}

void InputMgr::set_keyboard_config(u32 configHash)
{
    TaskMaster::task_master_for_active_thread().inputMgr().setKeyboardConfig(configHash);
}

bool InputMgr::query_keyboard(u32 stateHash)
{
    return TaskMaster::task_master_for_active_thread().inputMgr().queryKeyboard(stateHash);
}

void InputMgr::setKeyboardConfig(u32 configHash)
{
    if (mKeyConfigs.find(configHash) != mKeyConfigs.end())
        mActiveKeyboardConfig = configHash;
}

bool InputMgr::queryKeyboard(u32 stateHash)
{
    auto it = mKeyConfigs[mActiveKeyboardConfig].find(stateHash);
    if (it != mKeyConfigs[mActiveKeyboardConfig].end())
        return queryKeyboard(it->second);
    return false;
}

void InputMgr::setKeyFlag(const KeyInput & keyInput)
{
    ASSERT(keyInput.keyCode < kKEY_NOKEY);
    u32 idx = keyInput.keyCode / 32;
    u32 bit = keyInput.keyCode % 32;
    u32 mask = 1 << bit;

    if (keyInput.keyEvent == kKST_Down)
        mPressedKeys[idx] |= mask;
    else
        mPressedKeys[idx] &= ~mask;
}

bool InputMgr::queryKeyCode(KeyCode keyCode)
{
    if (keyCode == kKEY_NOKEY)
        return true;
    else if (keyCode > kKEY_NOKEY)
        return false;

    u32 idx = keyCode / 32;
    u32 bit = keyCode % 32;
    u32 mask = 1 << bit;

    return (mPressedKeys[idx] & mask) != 0;
}

bool InputMgr::queryKeyboard(const glm::uvec4 & keys)
{
    return (queryKeyCode((KeyCode)keys[0]) &&
            queryKeyCode((KeyCode)keys[1]) &&
            queryKeyCode((KeyCode)keys[2]) &&
            queryKeyCode((KeyCode)keys[3]));
}

void InputMgr::processKeyInput(const KeyInput & keyInput)
{
    setKeyFlag(keyInput);

    // Handle any keyboard listeners
    if (mIsPrimary)
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
    case HASH::kill_focus:
        LOG_INFO("Killing focus");
        mPressedKeys = glm::uvec4(0);
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
