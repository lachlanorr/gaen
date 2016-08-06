//------------------------------------------------------------------------------
// input.cpp - Low level input handling
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

#include "core/HashMap.h"
#include "core/String.h"

#include "engine/messages/MouseMove.h"
#include "engine/MessageWriter.h"

#include "engine/input.h"


namespace gaen
{

typedef HashMap<kMEM_Engine, String<kMEM_Engine>, KeyCode> KeyCodeMap;

static KeyCodeMap sKeyCodeMap =
{
    { "0", kKEY_0},
    { "1", kKEY_1},
    { "2", kKEY_2},
    { "3", kKEY_3},
    { "4", kKEY_4},
    { "5", kKEY_5},
    { "6", kKEY_6},
    { "7", kKEY_7},
    { "8", kKEY_8},
    { "9", kKEY_9},
    { "A", kKEY_A},
    { "B", kKEY_B},
    { "C", kKEY_C},
    { "D", kKEY_D},
    { "E", kKEY_E},
    { "F", kKEY_F},
    { "G", kKEY_G},
    { "H", kKEY_H},
    { "I", kKEY_I},
    { "J", kKEY_J},
    { "K", kKEY_K},
    { "L", kKEY_L},
    { "M", kKEY_M},
    { "N", kKEY_N},
    { "O", kKEY_O},
    { "P", kKEY_P},
    { "Q", kKEY_Q},
    { "R", kKEY_R},
    { "S", kKEY_S},
    { "T", kKEY_T},
    { "U", kKEY_U},
    { "V", kKEY_V},
    { "W", kKEY_W},
    { "X", kKEY_X},
    { "Y", kKEY_Y},
    { "Z", kKEY_Z},
    { "num0", kKEY_Num0},
    { "num1", kKEY_Num1},
    { "num2", kKEY_Num2},
    { "num3", kKEY_Num3},
    { "num4", kKEY_Num4},
    { "num5", kKEY_Num5},
    { "num6", kKEY_Num6},
    { "num7", kKEY_Num7},
    { "num8", kKEY_Num8},
    { "num9", kKEY_Num9},
    { "nummul", kKEY_NumMul},
    { "numadd", kKEY_NumAdd},
    { "numsub", kKEY_NumSub},
    { "numdot", kKEY_NumDot},
    { "numdiv", kKEY_NumDiv},
    { "leftshift", kKEY_LeftShift},
    { "rightshift", kKEY_RightShift},
    { "leftcontrol", kKEY_LeftControl},
    { "rightcontrol", kKEY_RightControl},
    { "leftalt", kKEY_LeftAlt},
    { "rightalt", kKEY_RightAlt},
    { "backspace", kKEY_Backspace},
    { "tab", kKEY_Tab},
    { "clear", kKEY_Clear},
    { "enter", kKEY_Enter},
    { "pause", kKEY_Pause},
    { "capslock", kKEY_CapsLock},
    { "escape", kKEY_Escape},
    { "space", kKEY_Space},
    { "pageup", kKEY_PageUp},
    { "pagedown", kKEY_PageDown},
    { "end", kKEY_End},
    { "home", kKEY_Home},
    { "left", kKEY_Left},
    { "up", kKEY_Up},
    { "right", kKEY_Right},
    { "down", kKEY_Down},
    { "printscreen", kKEY_PrintScreen},
    { "insert", kKEY_Insert},
    { "delete", kKEY_Delete},
    { "numlock", kKEY_NumLock},
    { "scrolllock", kKEY_ScrollLock},
    { "semicolon", kKEY_Semicolon},
    { "equal", kKEY_Equal},
    { "comma", kKEY_Comma},
    { "dash", kKEY_Dash},
    { "period", kKEY_Period},
    { "slash", kKEY_Slash},
    { "backtick", kKEY_BackTick},
    { "leftbracket", kKEY_LeftBracket},
    { "backslash", kKEY_BackSlash},
    { "bracketbackslash", kKEY_BracketBackSlash},
    { "rightbracket", kKEY_RightBracket},
    { "tick", kKEY_Tick},
    { "F1", kKEY_F1},
    { "F2", kKEY_F2},
    { "F3", kKEY_F3},
    { "F4", kKEY_F4},
    { "F5", kKEY_F5},
    { "F6", kKEY_F6},
    { "F7", kKEY_F7},
    { "F8", kKEY_F8},
    { "F9", kKEY_F9},
    { "F10", kKEY_F10},
    { "F11", kKEY_F11},
    { "F12", kKEY_F12},
    { "F13", kKEY_F13},
    { "F14", kKEY_F14},
    { "F15", kKEY_F15},
    { "F16", kKEY_F16},
    { "F17", kKEY_F17},
    { "F18", kKEY_F18},
    { "F19", kKEY_F19},
    { "F20", kKEY_F20},
    { "F21", kKEY_F21},
    { "F22", kKEY_F22},
    { "F23", kKEY_F23},
    { "F24", kKEY_F24}
};

KeyCode lookup_key_code(const char * str)
{
    auto it = sKeyCodeMap.find(str);
    if (it != sKeyCodeMap.end())
        return it->second;
    return kKEY_NOKEY;
}

void process_key_input(const void * pKeyInfo)
{
    KeyInput keyInput = convert_key_input(pKeyInfo);

    broadcast_targeted_message(HASH::keyboard_input,
                               kMessageFlag_None,
                               kMainThreadTaskId,
                               kInputMgrTaskId,
                               to_cell(keyInput),
                               0,
                               nullptr);
}

void kill_focus()
{
    broadcast_targeted_message(HASH::kill_focus,
                               kMessageFlag_None,
                               kMainThreadTaskId,
                               kInputMgrTaskId,
                               to_cell(0),
                               0,
                               nullptr);
}

static const f32 kMouseUpdateInterval = 1.0f / 60.0f;
static f32 sLastUpdateTime = 0.0f;
static short sMouseDeltaX;
static short sMouseDeltaY;

void send_mouse_input()
{
    // send mouse movements if timeout expired
    if (sMouseDeltaX != 0 || sMouseDeltaY != 0)
    {
        f32 nowTime = now();
        if (nowTime - sLastUpdateTime >= kMouseUpdateInterval)
        {
            MouseInput::Movement movement;
            movement.xDelta = sMouseDeltaX;
            movement.yDelta = sMouseDeltaY;

            MessageQueueWriter msgw(HASH::mouse_move,
                                    kMessageFlag_None,
                                    kMainThreadTaskId,
                                    kInputMgrTaskId,
                                    to_cell(movement),
                                    0);

            sMouseDeltaX = 0;
            sMouseDeltaY = 0;
            sLastUpdateTime = nowTime;
        }
    }
}

void process_mouse_input(const void * pMouseInfo)
{
    MouseInput mouseInput = convert_mouse_input(pMouseInfo);

    if (mouseInput.movement.xDelta || mouseInput.movement.yDelta)
    {
        sMouseDeltaX += mouseInput.movement.xDelta;
        sMouseDeltaY += mouseInput.movement.yDelta;
    }

    if (mouseInput.buttons.buttonFlags)
    {
        KeyInput keyInput;
        keyInput.deviceId = 0;

        if (mouseInput.buttons.buttonFlags & kMBTN_1Down)
        {
            keyInput.keyCode = kKEY_Mouse1;
            keyInput.keyEvent = kKST_Down;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        else if (mouseInput.buttons.buttonFlags & kMBTN_1Up)
        {
            keyInput.keyCode = kKEY_Mouse1;
            keyInput.keyEvent = kKST_Up;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        if (mouseInput.buttons.buttonFlags & kMBTN_2Down)
        {
            keyInput.keyCode = kKEY_Mouse2;
            keyInput.keyEvent = kKST_Down;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        else if (mouseInput.buttons.buttonFlags & kMBTN_2Up)
        {
            keyInput.keyCode = kKEY_Mouse2;
            keyInput.keyEvent = kKST_Up;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        if (mouseInput.buttons.buttonFlags & kMBTN_3Down)
        {
            keyInput.keyCode = kKEY_Mouse3;
            keyInput.keyEvent = kKST_Down;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        else if (mouseInput.buttons.buttonFlags & kMBTN_3Up)
        {
            keyInput.keyCode = kKEY_Mouse3;
            keyInput.keyEvent = kKST_Up;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        if (mouseInput.buttons.buttonFlags & kMBTN_4Down)
        {
            keyInput.keyCode = kKEY_Mouse4;
            keyInput.keyEvent = kKST_Down;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        else if (mouseInput.buttons.buttonFlags & kMBTN_4Up)
        {
            keyInput.keyCode = kKEY_Mouse4;
            keyInput.keyEvent = kKST_Up;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        if (mouseInput.buttons.buttonFlags & kMBTN_5Down)
        {
            keyInput.keyCode = kKEY_Mouse5;
            keyInput.keyEvent = kKST_Down;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }
        else if (mouseInput.buttons.buttonFlags & kMBTN_5Up)
        {
            keyInput.keyCode = kKEY_Mouse5;
            keyInput.keyEvent = kKST_Up;
            MessageQueueWriter msgw(HASH::keyboard_input,
                kMessageFlag_None,
                kMainThreadTaskId,
                kInputMgrTaskId,
                to_cell(keyInput),
                0);
        }

        if (mouseInput.buttons.buttonFlags & kMBTN_Wheel)
        {
            MessageQueueWriter msgw(HASH::mouse_wheel,
                                    kMessageFlag_None,
                                    kMainThreadTaskId,
                                    kInputMgrTaskId,
                                    to_cell(mouseInput.buttons.wheelMovement),
                                    0);
        }
    }
}


} // namespace gaen
