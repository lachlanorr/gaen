//------------------------------------------------------------------------------
// InputMgr.h - Maintains input state for TaskMasters
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

#ifndef GAEN_ENGINE_INPUT_MGR_H
#define GAEN_ENGINE_INPUT_MGR_H

#include <glm/vec4.hpp>

#include "core/HashMap.h"
#include "core/Vector.h"

#include "engine/MessageAccessor.h"
#include "engine/input.h"

namespace gaen
{

class InputMgr
{
public:
    InputMgr(bool isPrimary);

    template <typename T>
    MessageResult message(const T& msgAcc);

    // Callable from entities since there is an InputMgr on each task master
    static void set_keyboard_config(u32 configHash);
    static bool query_keyboard(u32 stateHash);

private:
    struct TaskMessage
    {
        u32 taskId:28;
        u32 deviceId:4;
        u32 message;
        
        TaskMessage(u32 taskId, u32 message)
          : taskId(taskId)
          , deviceId(0) // LORRTODO: For now we don't use deviceId. Should probably be playerId instead. Needs more thought.
          , message(message)
        {}

        bool operator==(const TaskMessage & rhs) const
        {
            static_assert(sizeof(*this) == 8, "Tricky comparison requires 8 byte size");
            return *(const u64*)this == *(const u64*)&rhs;
        }
    };

    struct TaskStateMessage
    {
        u32 taskId:28;
        u32 deviceId:4;
        u32 downMessage;
        i32 downValue;
        u32 upMessage;
        i32 upValue;

        TaskStateMessage(u32 taskId, u32 downMessage, i32 downValue, u32 upMessage, i32 upValue)
          : taskId(taskId)
          , deviceId(0) // LORRTODO: For now we don't use deviceId. Should probably be playerId instead. Needs more thought.
          , downMessage(downMessage)
          , downValue(downValue)
          , upMessage(upMessage)
          , upValue(upValue)
        {}

        bool operator==(const TaskStateMessage & rhs) const
        {
            return (taskId == rhs.taskId &&
                    deviceId == rhs.deviceId &&
                    downMessage == rhs.downMessage &&
                    downValue == rhs.downValue &&
                    upMessage == rhs.upMessage &&
                    upValue == rhs.upValue);
        }
    };

    void setKeyboardConfig(u32 configHash);
    bool queryKeyboard(u32 stateHash);

    void setKeyFlag(const KeyInput & keyInput);
    bool queryKeyCode(KeyCode keyCode);
    bool queryKeyboard(const glm::uvec4 & keys);

    void processKeyInput(const KeyInput & keyInput);
    void processMouseMoveInput(const MouseInput::Movement & moveInput);
    void processMouseWheelInput(i32 delta);

    void registerKeyToState(KeyCode keyCode, u32 stateHash);
    void registerStateListener(u32 stateHash, const TaskStateMessage & taskMessage);

    void registerMouseListener(const TaskMessage & moveMessage, const TaskMessage & wheelMessage);

    bool mIsPrimary;

    glm::uvec4 mPressedKeys;

    u32 mActiveKeyboardConfig;
    HashMap<kMEM_Engine, u32, HashMap<kMEM_Engine, u32, glm::uvec4>> mKeyConfigs;

    // LORRTODO - Add support for removing listeners for both keys and mouse
    HashMap<kMEM_Engine, KeyCode, Vector<kMEM_Engine, u32>, std::hash<int>> mKeyToStateMap;
    HashMap<kMEM_Engine, u32, Vector<kMEM_Engine, TaskStateMessage>> mStateListenerMap;

    Vector<kMEM_Engine, TaskMessage> mMouseMoveListeners;
    Vector<kMEM_Engine, TaskMessage> mMouseWheelListeners;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_INPUT_MGR_H
