//------------------------------------------------------------------------------
// InputMgr.h - Maintains input state for TaskMasters
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include "gaen/core/HashMap.h"
#include "gaen/core/Vector.h"

#include "gaen/math/vec4.h"

#include "gaen/engine/MessageAccessor.h"
#include "gaen/engine/input.h"

namespace gaen
{

class InputMgr
{
public:
    InputMgr(bool isPrimary);

    template <typename T>
    MessageResult message(const T& msgAcc);

    void disableMode(u32 modeHash);
    void enableMode(u32 modeHash);
    bool isModeEnabled(u32 modeHash);

    u32 keyboardMode() { return mpKeyboardMode ? mpKeyboardMode->nameHash : 0; }
    void setKeyboardMode(u32 modeHash);

    static const u32 kPadInputDetected = 0xffFFffFF;
    u32 queryState(u32 player, u32 modeHash, u32 stateHash, vec4 * pMeasure);

    static const u32 kPadCountMax = 4;
    void updatePadState(u32 padId,
                        u32 codes,
                        const vec2 & lstick,
                        const vec2 & rstick,
                        f32 ltrigger,
                        f32 rtrigger);

    void pollPadInput();

    static void register_key_press_listener(u32 mode, task_id target);
    static void deregister_key_press_listener(u32 mode, task_id target);
private:
    struct MouseState
    {
        i32 xPos;
        i32 yPos;
        i32 wheel;

        void zeroState()
        {
            xPos = 0;
            yPos = 0;
            wheel = 0;
        }
    };

    struct InputMode
    {
        u32 nameHash;

        HashMap<kMEM_Engine, u32, Vector<kMEM_Engine, ivec4>> keyboard;
        HashMap<kMEM_Engine, u32, ivec4> mouseButtons;
        u32 mouseMove;

        HashMap<kMEM_Engine, u32, u32> pad;

        InputMode()
          : nameHash(0)
          , mouseMove(0)
        {}
    };

    u32 queryPadState(u32 padId, u32 codes, vec4 * pMeasure);

    bool queryKey(Key key);
    u32 queryKeyboardState(const ivec4 & keys);

    void processKeyInput(const KeyInput & keyInput);
    void processMouseMoveInput(const MouseInput::Movement & moveInput);
    void processMouseWheelInput(i32 delta);

    void zeroState();

    void registerKeyPressListener(u32 mode, task_id target);
    void deregisterKeyPressListener(u32 mode, task_id target);
    void deregisterKeyPressListener(task_id target);
    void notifyKeyPressListeners(u32 mode, const ivec4 & keys);
    void notifyKeyPressListeners(const ivec4 & keys);

    bool mIsPrimary;

    PadInput mPadState[kPadCountMax];

    ivec4 mPressedKeys;

    MouseState mMouseState;

    InputMode * mpKeyboardMode;
    Vector<kMEM_Engine, InputMode*> mActiveModes;
    HashMap<kMEM_Engine, u32, InputMode> mModes;

    HashMap<kMEM_Engine, u32, Vector<kMEM_Engine, task_id>> mKeyPressListeners;
};

// Compose API
class Entity;
namespace system_api
{
void input_enable_mode(i32 modeHash, Entity * pCaller);
void input_disable_mode(i32 modeHash, Entity * pCaller);
void input_set_keyboard_mode(i32 modeHash, Entity * pCaller);

void input_register_key_press_listener(i32 modeHash, Entity * pCaller);
void input_deregister_key_press_listener(i32 modeHash, Entity * pCaller);
} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_INPUT_MGR_H
