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
#include "engine/messages/PadInput.h"

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
    Config<kMEM_Engine> inputConf;

    zeroState();

    mpActiveMode = 0;

    char inputConfPath[kMaxPath+1];
    find_input_conf(inputConfPath);

    if (inputConf.read(inputConfPath))
    {
        for (auto secIt = inputConf.sectionsBegin();
             secIt != inputConf.sectionsEnd();
             ++secIt)
        {
            u32 secHash = HASH::hash_func(*secIt);

            mModes[secHash].nameHash = secHash;

            for (auto inpIt = inputConf.keysBegin(*secIt);
                 inpIt != inputConf.keysEnd(*secIt);
                 ++inpIt)
            {
                u32 inpHash = HASH::hash_func(*inpIt);

                if (inpHash == HASH::none || inpHash == HASH::any)
                {
                    LOG_ERROR("'none' and 'any' input names are reserved");
                }
                else
                {
                    auto inpVec = inputConf.getVec(*secIt, *inpIt);

                    // Check for keys
                    ivec4 keyCodes;
                    u32 inpCount = min(4, (i32)inpVec.size());
                    for (u32 i = 0; i < inpCount; ++i)
                    {
                        keyCodes[i] = lookup_key_code(inpVec[i]);
                    }
                    for (u32 i = inpCount; i < 4; ++i)
                    {
                        keyCodes[i] = kKEY_NONE;
                    }

                    static const ivec4 kAllNoneKeys = ivec4(kKEY_NONE);
                    if (keyCodes != kAllNoneKeys)
                    {
                        mModes[secHash].keyboard[inpHash] = keyCodes;
                    }
                    else
                    {
                        // check for pad codes
                        u32 padCodes = 0;
                        u32 analogCount = 0;
                        for (u32 i = 0; i < inpCount; ++i)
                        {
                            PadCode code = lookup_pad_button(inpVec[i]);
                            if (code >= (u32)kPadCodeAnalogStart)
                                analogCount++;
                            padCodes |= code;
                        }
                        if (padCodes != kPAD_NONE)
                        {
                            if (analogCount > 2)
                                LOG_ERROR("input.conf mode has more than 2 analog pad codes");
                            else
                                mModes[secHash].pad[inpHash] = padCodes;
                        }
                    }

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

    for (u32 i = 0; i < kPadCountMax; ++i)
    {
        mPadState[i] = PadInput(i);
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
            messages::KeyPressBW msgw(HASH::key_press, kMessageFlag_None, kInputMgrTaskId, target);
            msgw.setKeys(keys);
            TaskMaster::task_master_for_active_thread().message(msgw.accessor());
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
                               to_cell(mode));
}

void InputMgr::deregister_key_press_listener(u32 mode, task_id target)
{
    broadcast_targeted_message(HASH::deregister_key_press_listener,
                               kMessageFlag_None,
                               target,
                               kInputMgrTaskId,
                               to_cell(mode));
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
        auto padit = mpActiveMode->pad.find(stateHash);
        if (padit != mpActiveMode->pad.end())
        {
            return queryPadState(player, padit->second, pMeasure);
        }

        auto keyit = mpActiveMode->keyboard.find(stateHash);
        if (keyit != mpActiveMode->keyboard.end())
            return queryKeyboardState(keyit->second);
    }
    return 0;
}

void InputMgr::updatePadState(u32 padId,
                              u32 codes,
                              const vec2 & lstick,
                              const vec2 & rstick,
                              f32 ltrigger,
                              f32 rtrigger)
{
    ASSERT(padId < kPadCountMax);
    ASSERT(mPadState[padId].padId == padId);
    PadInput & padState = mPadState[padId];
    padState.codes = codes;
    padState.lstick = lstick;
    padState.rstick = rstick;
    padState.ltrigger = ltrigger;
    padState.rtrigger = rtrigger;
}


static HashMap<kMEM_Engine, i32, PadCode> sGlfwPadMap =
{
    {GLFW_GAMEPAD_BUTTON_A,            kPAD_A},
    {GLFW_GAMEPAD_BUTTON_B,            kPAD_B},
    {GLFW_GAMEPAD_BUTTON_X,            kPAD_X},
    {GLFW_GAMEPAD_BUTTON_Y,            kPAD_Y},
    {GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,  kPAD_LBumper},
    {GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, kPAD_RBumper},
    {GLFW_GAMEPAD_BUTTON_BACK,         kPAD_Back},
    {GLFW_GAMEPAD_BUTTON_START,        kPAD_Start},
    {GLFW_GAMEPAD_BUTTON_GUIDE,        kPAD_Guide},
    {GLFW_GAMEPAD_BUTTON_LEFT_THUMB,   kPAD_LThumb},
    {GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,  kPAD_RThumb},
    {GLFW_GAMEPAD_BUTTON_DPAD_UP,      kPAD_DUp},
    {GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,   kPAD_DRight},
    {GLFW_GAMEPAD_BUTTON_DPAD_DOWN,    kPAD_DDown},
    {GLFW_GAMEPAD_BUTTON_DPAD_LEFT,    kPAD_DLeft}
};

static const f32 kStickDeadzone = 0.25f;
static const f32 kStickMultiplier = 1.0f / (1.0f - kStickDeadzone);
static const f32 kTriggerDeadzone = -0.75f;
static const f32 kTriggerMultiplier = 2.0f / (1.0f + -kTriggerDeadzone) / 2.0f;

static inline f32 correct_stick(f32 val)
{
    if (abs(val) >= kStickDeadzone)
    {
        if (val < 0)
            return (val + kStickDeadzone) * kStickMultiplier;
        else
            return (val - kStickDeadzone) * kStickMultiplier;
    }
    return 0.0f;
}

static inline f32 correct_trigger(f32 val)
{
    if (val >= kTriggerDeadzone)
        return (val + -kTriggerDeadzone) * kTriggerMultiplier;
    return 0.0f;
}

void InputMgr::pollPadInput()
{
    for (u32 i = 0; i < InputMgr::kPadCountMax; ++i)
    {
        if (glfwJoystickIsGamepad(i))
        {
            PadInput pad(i);

            GLFWgamepadstate padState;
            glfwGetGamepadState(i, &padState);
            pad.lstick.x = correct_stick(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
            pad.lstick.y = correct_stick(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
            pad.rstick.x = correct_stick(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
            pad.rstick.y = correct_stick(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
            pad.ltrigger = correct_trigger(padState.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]);
            pad.rtrigger = correct_trigger(padState.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]);
            for (const auto & kv : sGlfwPadMap)
            {
                if (padState.buttons[kv.first] == GLFW_PRESS)
                {
                    pad.codes |= kv.second;
                }
            }
            if (pad.lstick.x != 0.0f || pad.lstick.y != 0.0f)
            {
                pad.codes |= kPAD_LStick;
            }
            if (pad.rstick.x != 0.0f || pad.rstick.y != 0.0f)
            {
                pad.codes |= kPAD_RStick;
            }
            if (pad.ltrigger > 0.0f)
            {
                pad.codes |= kPAD_LTrigger;
            }
            if (pad.rtrigger > 0.0f)
            {
                pad.codes |= kPAD_RTrigger;
            }

            for (const auto & kv : sGlfwPadMap)
            {
                if (padState.buttons[kv.first] == GLFW_PRESS)
                {
                    pad.codes |= kv.second;
                }
            }

            if (mPadState[i] != pad)
            {
                mPadState[i] = pad;

                messages::PadInputBW msgw(HASH::pad_input, kMessageFlag_None, kPrimaryThreadTaskId, kInputMgrTaskId, i);
                msgw.setCodes(pad.codes);
                msgw.setLstick(pad.lstick);
                msgw.setRstick(pad.rstick);
                msgw.setLtrigger(pad.ltrigger);
                msgw.setRtrigger(pad.rtrigger);
                broadcast_targeted_message(msgw.accessor());
            }
        }
    }

}

bool InputMgr::queryKey(Key key)
{
    if (key == kKEY_NONE)
        return true;
    else if (key > kKEY_NONE)
        return false;

    u32 idx = key / 32;
    u32 bit = key % 32;
    u32 mask = 1 << bit;

    return (mPressedKeys[idx] & mask) != 0;
}

u32 InputMgr::queryPadState(u32 padId, u32 codes, vec4 * pMeasure)
{
    ASSERT(padId < kPadCountMax);
    const PadInput & pad = mPadState[padId];

    *pMeasure = vec4(0.0f);
    if ((pad.codes & codes) == codes)
    {
        if (codes & kPAD_LStick)
        {
            pMeasure->x = pad.lstick.x;
            pMeasure->y = pad.lstick.y;
            if (codes & kPAD_RStick)
            {
                pMeasure->z = pad.rstick.x;
                pMeasure->w = pad.rstick.y;
            }
            else if (codes & kPAD_LTrigger)
            {
                pMeasure->z = pad.ltrigger;
            }
            else if (codes & kPAD_RTrigger)
            {
                pMeasure->z = pad.rtrigger;
            }
        }
        else if (codes & kPAD_RStick)
        {
            pMeasure->x = pad.rstick.x;
            pMeasure->y = pad.rstick.y;
            if (codes & kPAD_LTrigger)
            {
                pMeasure->z = pad.ltrigger;
            }
            else if (codes & kPAD_RTrigger)
            {
                pMeasure->z = pad.rtrigger;
            }
        }
        else if (codes & kPAD_LTrigger)
        {
            pMeasure->x = pad.ltrigger;
            if (codes & kPAD_RTrigger)
            {
                pMeasure->y = pad.rtrigger;
            }
        }
        else if (codes & kPAD_RTrigger)
        {
            pMeasure->x = pad.rtrigger;
        }

        return kPadInputDetected;
    }

    // If codes contains a flag for analog input, always return true
    // so we can register zeroed values in input handlers.
    if (codes >= kPadCodeAnalogStart)
        return kPadInputDetected;

    return 0;
}

u32 InputMgr::queryKeyboardState(const ivec4 & keys)
{
    u32 ret = 0;
    if (queryKey((Key)keys[0]))
    {
        ret += (Key)keys[0] != kKEY_NONE ? 1 : 0;
        if (queryKey((Key)keys[1]))
        {
            ret += (Key)keys[1] != kKEY_NONE ? 1 : 0;
            if (queryKey((Key)keys[2]))
            {
                ret += (Key)keys[1] != kKEY_NONE ? 1 : 0;
                if (queryKey((Key)keys[3]))
                {
                    ret += (Key)keys[3] != kKEY_NONE ? 1 : 0;
                    return ret;
                }
            }
        }
    }
    return 0;
}

void InputMgr::processKeyInput(const KeyInput & keyInput)
{
    if (keyInput.key < kKEY_NONE)
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
    case HASH::pad_input:
    {
        messages::PadInputR<T> msgr(msgAcc);
        updatePadState(msgr.padId(), msgr.codes(), msgr.lstick(), msgr.rstick(), msgr.ltrigger(), msgr.rtrigger());
    }
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
