//------------------------------------------------------------------------------
// InputMgr.cpp - Maintains input state for TaskMasters
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "math/common.h"

#include "assets/Config.h"
#include "assets/file_utils.h"

#include "engine/Entity.h"
#include "engine/TaskMaster.h"
#include "engine/MessageWriter.h"
#include "engine/messages/KeyPress.h"
#include "engine/messages/MouseMove.h"

#include "engine/InputMgr.h"

namespace gaen
{

static const char * kInputConf = "input.conf";

void find_input_conf(char * inputConfPath)
{
    char path[kMaxPath+1];
    char checkPath[kMaxPath+1];

    strcpy(inputConfPath, kInputConf);

    process_path(path);
    parent_dir(path);

    for (;;)
    {
        PANIC_IF(!*path, "Unable to find input.conf, make sure input.conf is located under a gaen or gaen project directory tree.");

        snprintf(checkPath, kMaxPath, "%s/%s", path, kInputConf);

        if (file_exists(checkPath))
        {
            strcpy(inputConfPath, checkPath);
            return;
        }
        parent_dir(path);
    }
}


InputMgr::InputMgr(bool isPrimary)
  : mIsPrimary(isPrimary)
{
    Config<kMEM_Engine> keyConf;

    zeroState();

    mpActiveMode = 0;

    char inputConfPath[kMaxPath+1];
    find_input_conf(inputConfPath);

    if (keyConf.read(inputConfPath))
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
                    ivec4 keys;
                    auto keyVec = keyConf.getVec(*secIt, *keyIt);
                    u32 keyCount = min(4, (i32)keyVec.size());
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
    mPressedKeys = ivec4(0);
    mMouseState.zeroState();

    for (auto & cs : mCtrlState)
    {
        cs.zeroState();
    }
}

void InputMgr::registerKeyPressListener(u32 mode, task_id target)
{
    auto & listenerVec = mKeyPressListeners[mode];
    listenerVec.push_back(target);
}

void InputMgr::deregisterKeyPressListener(u32 mode, task_id target)
{
    auto it = mKeyPressListeners.find(mode);
    if (it != mKeyPressListeners.end())
    {
        std::remove(it->second.begin(), it->second.end(), target);
    }
}

void InputMgr::deregisterKeyPressListener(task_id target)
{
    // remove from all modes
    for (auto & pair : mKeyPressListeners)
    {
        std::remove(pair.second.begin(), pair.second.end(), target);
    }
}

void InputMgr::notifyKeyPressListeners(u32 mode, const ivec4 & keys)
{
    auto it = mKeyPressListeners.find(mode);
    if (it != mKeyPressListeners.end())
    {
        for (const auto & target : it->second)
        {
            messages::KeyPressQW msgQW(HASH::key_press, kMessageFlag_None, kInputMgrTaskId, target);
            msgQW.setKeys(keys);
        }
    }
}

void InputMgr::notifyKeyPressListeners(const ivec4 & keys)
{
    if (mpActiveMode)
    {
        notifyKeyPressListeners(mpActiveMode->nameHash, keys);
    }

    // Always send to the editor task, but only if mpActiveMode is not
    // HASH::editor__.
    if (!mpActiveMode || mpActiveMode->nameHash != HASH::editor__)
    {
        notifyKeyPressListeners(HASH::editor__, keys);
    }
}

void InputMgr::register_key_press_listener(u32 mode, task_id target)
{
    broadcast_targeted_message(HASH::register_key_press_listener,
                               kMessageFlag_None,
                               target,
                               kInputMgrTaskId,
                               to_cell(mode),
                               0,
                               nullptr);
}

void InputMgr::deregister_key_press_listener(u32 mode, task_id target)
{
    broadcast_targeted_message(HASH::deregister_key_press_listener,
                               kMessageFlag_None,
                               target,
                               kInputMgrTaskId,
                               to_cell(mode),
                               0,
                               nullptr);
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

u32 InputMgr::queryState(u32 player, u32 stateHash, vec4 * pMeasure)
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

u32 InputMgr::queryState(const ivec4 & keys)
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
            }
        }
    }
    return 0;
}

void InputMgr::processKeyInput(const KeyInput & keyInput)
{
    if (keyInput.key < kKEY_NOKEY)
    {
        if (keyInput.action == kKACT_Release)
            unset_key_vec_bit(mPressedKeys, (Key)keyInput.key);
        else // kKACT_Press || kKACT_Repeat
            set_key_vec_bit(mPressedKeys, (Key)keyInput.key);

        if (mIsPrimary)
        {
            notifyKeyPressListeners(mPressedKeys);
        }
    }
    else
    {
        ERR("Unknown keyInput.key: %u", keyInput.key);
    }
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
        //LOG_INFO("Killing focus");
        mPressedKeys = ivec4(0);
        break;
    case HASH::mouse_move:
        processMouseMoveInput(MouseInput::Movement(msg.payload));
        break;
    case HASH::mouse_wheel:
        processMouseWheelInput(msg.payload.i);
        break;
    case HASH::register_key_press_listener:
        registerKeyPressListener(msg.payload.u, msg.source);
        break;
    case HASH::deregister_key_press_listener:
        deregisterKeyPressListener(msg.payload.u, msg.source);
        break;
    default:
        PANIC("Unknown InputMgr message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult InputMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult InputMgr::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

namespace system_api
{
void register_key_press_listener(i32 modeHash, Entity * pCaller)
{
    InputMgr::register_key_press_listener(modeHash, pCaller->task().id());
}

void deregister_key_press_listener(i32 modeHash, Entity * pCaller)
{
    InputMgr::deregister_key_press_listener(modeHash, pCaller->task().id());
}
} // namespace system_api


} // namespace gaen
