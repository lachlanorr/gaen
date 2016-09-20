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

#include "engine/TaskMaster.h"
#include "engine/MessageWriter.h"
#include "engine/messages/MouseMove.h"

#include "engine/InputMgr.h"

namespace gaen
{

static const char * kKeyboardConf = "input.conf";

InputMgr::InputMgr(bool isPrimary)
  : mIsPrimary(isPrimary)
{
    Config<kMEM_Engine> keyConf;

    zeroState();

    mpActiveMode = 0;
    if (keyConf.read(kKeyboardConf))
    {
        for (auto secIt = keyConf.sectionsBegin();
             secIt != keyConf.sectionsEnd();
             ++secIt)
        {
            u32 secHash = HASH::hash_func(*secIt);

            mModes[secHash].nameHash = secHash;

            for (auto keyIt = keyConf.keysBegin(*secIt);
                 keyIt != keyConf.keysEnd(*secIt);
                 ++keyIt)
            {
                u32 keyHash = HASH::hash_func(*keyIt);

                if (keyHash == HASH::none || keyHash == HASH::any)
                {
                    LOG_ERROR("'none' or 'any' input names are reserved");
                }
                else
                {
                    glm::ivec4 keys;
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
                    mModes[secHash].keyboard[keyHash] = keys;
                }
            }

            if (mpActiveMode == nullptr)
                mpActiveMode = &mModes[secHash];
        }
    }
}

void InputMgr::zeroState()
{
    mPressedKeys = glm::ivec4(0);
    mMouseState.zeroState();

    for (auto & cs : mCtrlState)
    {
        cs.zeroState();
    }
}

void InputMgr::setMode(u32 modeHash)
{
    auto it = mModes.find(modeHash);

    if (it != mModes.end() && &it->second != mpActiveMode)
    {
        mMouseState.zeroState();
        mpActiveMode = &it->second;
    }
}

u32 InputMgr::queryState(u32 player, u32 stateHash, glm::vec4 * pMeasure)
{
    if (mpActiveMode)
    {
        auto it = mpActiveMode->keyboard.find(stateHash);
        if (it != mpActiveMode->keyboard.end())
            return queryState(it->second);
    }
    return 0;
}

bool InputMgr::queryKey(Key key)
{
    if (key == kKEY_NOKEY)
        return true;
    else if (key > kKEY_NOKEY)
        return false;

    u32 idx = key / 32;
    u32 bit = key % 32;
    u32 mask = 1 << bit;

    return (mPressedKeys[idx] & mask) != 0;
}

u32 InputMgr::queryState(const glm::ivec4 & keys)
{
    u32 ret = 0;
    if (queryKey((Key)keys[0]))
    {
        ret += (Key)keys[0] != kKEY_NOKEY ? 1 : 0;
        if (queryKey((Key)keys[1]))
        {
            ret += (Key)keys[1] != kKEY_NOKEY ? 1 : 0;
            if (queryKey((Key)keys[2]))
            {
                ret += (Key)keys[1] != kKEY_NOKEY ? 1 : 0;
                if (queryKey((Key)keys[3]))
                {
                    ret += (Key)keys[3] != kKEY_NOKEY ? 1 : 0;
                    return ret;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void InputMgr::processKeyInput(const KeyInput & keyInput)
{
    ASSERT(keyInput.key < kKEY_NOKEY);
    u32 idx = keyInput.key / 32;
    u32 bit = keyInput.key % 32;
    u32 mask = 1 << bit;

    if (keyInput.action == kKACT_Release)
        mPressedKeys[idx] &= ~mask;
    else // kKACT_Press || kKACT_Repeat
        mPressedKeys[idx] |= mask;
}

void InputMgr::processMouseMoveInput(const MouseInput::Movement & moveInput)
{
    mMouseState.xPos += moveInput.xDelta;
    mMouseState.yPos += moveInput.yDelta;
}

void InputMgr::processMouseWheelInput(i32 delta)
{
    mMouseState.wheel = delta;
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
        mPressedKeys = glm::ivec4(0);
        break;
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
