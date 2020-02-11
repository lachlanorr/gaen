//------------------------------------------------------------------------------
// input.cpp - Low level input handling
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

#include "core/HashMap.h"
#include "core/String.h"

#include "engine/messages/MouseMove.h"
#include "engine/messages/PadInput.h"
#include "engine/MessageWriter.h"

#include "engine/InputMgr.h"
#include "engine/input.h"

namespace gaen
{

typedef HashMap<kMEM_Engine, String<kMEM_Engine>, Key> KeyCodeMap;
static KeyCodeMap sKeyCodeMap =
{
    { "space",            kKEY_Space},
    { "apostrophe",       kKEY_Apostrophe},
    { "comma",            kKEY_Comma},
    { "minus",            kKEY_Minus},
    { "period",           kKEY_Period},
    { "slash",            kKEY_Slash},
    { "0",                kKEY_0},
    { "1",                kKEY_1},
    { "2",                kKEY_2},
    { "3",                kKEY_3},
    { "4",                kKEY_4},
    { "5",                kKEY_5},
    { "6",                kKEY_6},
    { "7",                kKEY_7},
    { "8",                kKEY_8},
    { "9",                kKEY_9},
    { "semicolon",        kKEY_Semicolon},
    { "equal",            kKEY_Equal},
    { "A",                kKEY_A},
    { "B",                kKEY_B},
    { "C",                kKEY_C},
    { "D",                kKEY_D},
    { "E",                kKEY_E},
    { "F",                kKEY_F},
    { "G",                kKEY_G},
    { "H",                kKEY_H},
    { "I",                kKEY_I},
    { "J",                kKEY_J},
    { "K",                kKEY_K},
    { "L",                kKEY_L},
    { "M",                kKEY_M},
    { "N",                kKEY_N},
    { "O",                kKEY_O},
    { "P",                kKEY_P},
    { "Q",                kKEY_Q},
    { "R",                kKEY_R},
    { "S",                kKEY_S},
    { "T",                kKEY_T},
    { "U",                kKEY_U},
    { "V",                kKEY_V},
    { "W",                kKEY_W},
    { "X",                kKEY_X},
    { "Y",                kKEY_Y},
    { "Z",                kKEY_Z},
    { "left_bracket",     kKEY_LeftBracket},
    { "backslash",        kKEY_Backslash},
    { "right_bracket",    kKEY_RightBracket},
    { "grave_accent",     kKEY_GraveAccent},
    { "world_1",          kKEY_World1},
    { "world_2",          kKEY_World2},
    { "escape",           kKEY_Escape},
    { "enter",            kKEY_Enter},
    { "tab",              kKEY_Tab},
    { "backspace",        kKEY_Backspace},
    { "insert",           kKEY_Insert},
    { "delete",           kKEY_Delete},
    { "right",            kKEY_Right},
    { "left",             kKEY_Left},
    { "down",             kKEY_Down},
    { "up",               kKEY_Up},
    { "page_up",          kKEY_PageUp},
    { "page_down",        kKEY_PageDown},
    { "home",             kKEY_Home},
    { "end",              kKEY_End},
    { "caps_lock",        kKEY_CapsLock},
    { "scroll_lock",      kKEY_ScrollLock},
    { "num_lock",         kKEY_NumLock},
    { "print_screen",     kKEY_PrintScreen},
    { "pause",            kKEY_Pause},
    { "F1",               kKEY_F1},
    { "F2",               kKEY_F2},
    { "F3",               kKEY_F3},
    { "F4",               kKEY_F4},
    { "F5",               kKEY_F5},
    { "F6",               kKEY_F6},
    { "F7",               kKEY_F7},
    { "F8",               kKEY_F8},
    { "F9",               kKEY_F9},
    { "F10",              kKEY_F10},
    { "F11",              kKEY_F11},
    { "F12",              kKEY_F12},
    { "F13",              kKEY_F13},
    { "F14",              kKEY_F14},
    { "F15",              kKEY_F15},
    { "F16",              kKEY_F16},
    { "F17",              kKEY_F17},
    { "F18",              kKEY_F18},
    { "F19",              kKEY_F19},
    { "F20",              kKEY_F20},
    { "F21",              kKEY_F21},
    { "F22",              kKEY_F22},
    { "F23",              kKEY_F23},
    { "F24",              kKEY_F24},
    { "F25",              kKEY_F25},
    { "num_pad_0",        kKEY_NumPad0},
    { "num_pad_1",        kKEY_NumPad1},
    { "num_pad_2",        kKEY_NumPad2},
    { "num_pad_3",        kKEY_NumPad3},
    { "num_pad_4",        kKEY_NumPad4},
    { "num_pad_5",        kKEY_NumPad5},
    { "num_pad_6",        kKEY_NumPad6},
    { "num_pad_7",        kKEY_NumPad7},
    { "num_pad_8",        kKEY_NumPad8},
    { "num_pad_9",        kKEY_NumPad9},
    { "num_pad_decimal",  kKEY_NumPadDecimal},
    { "num_pad_divide",   kKEY_NumPadDivide},
    { "num_pad_multiply", kKEY_NumPadMultiply},
    { "num_pad_subtract", kKEY_NumPadSubtract},
    { "num_pad_add",      kKEY_NumPadAdd},
    { "num_pad_enter",    kKEY_NumPadEnter},
    { "num_pad_equal",    kKEY_NumPadEqual},
    { "left_shift",       kKEY_LeftShift},
    { "left_control",     kKEY_LeftControl},
    { "left_alt",         kKEY_LeftAlt},
    { "right_shift",      kKEY_RightShift},
    { "right_control",    kKEY_RightControl},
    { "right_alt",        kKEY_RightAlt},
    { "menu",             kKEY_Menu},
    { "mouse_1",          kKEY_Mouse1},
    { "mouse_2",          kKEY_Mouse2},
    { "mouse_3",          kKEY_Mouse3},
    { "mouse_4",          kKEY_Mouse4},
    { "mouse_5",          kKEY_Mouse5},
    { "mouse_6",          kKEY_Mouse6},
    { "mouse_7",          kKEY_Mouse7},
    { "mouse_8",          kKEY_Mouse8}
};
Key lookup_key_code(const char * str)
{
    auto it = sKeyCodeMap.find(str);
    if (it != sKeyCodeMap.end())
        return it->second;
    return kKEY_NONE;
}


typedef HashMap<kMEM_Engine, String<kMEM_Engine>, PadCode> PadCodeMap;
static PadCodeMap sPadCodeMap =
{
    { "pad_A",        kPAD_A},
    { "pad_B",        kPAD_B},
    { "pad_X",        kPAD_X},
    { "pad_Y",        kPAD_Y},

    { "pad_LBumper",  kPAD_LBumper},
    { "pad_RBumper",  kPAD_RBumper},

    { "pad_Back",     kPAD_Back},
    { "pad_Start",    kPAD_Start},
    { "pad_Guide",    kPAD_Guide},

    { "pad_LThumb",   kPAD_LThumb},
    { "pad_RThumb",   kPAD_RThumb},

    { "pad_DUp",      kPAD_DUp},
    { "pad_DRight",   kPAD_DRight},
    { "pad_DDown",    kPAD_DDown},
    { "pad_DLeft",    kPAD_DLeft},

    { "pad_LStick",   kPAD_LStick},
    { "pad_RStick",   kPAD_RStick},
    { "pad_LTrigger", kPAD_LTrigger},
    { "pad_RTrigger", kPAD_RTrigger}
};
PadCode lookup_pad_button(const char * str)
{
    auto it = sPadCodeMap.find(str);
    if (it != sPadCodeMap.end())
        return it->second;
    return kPAD_NONE;
}

void process_key_input(KeyInput keyInput)
{
    broadcast_targeted_message(HASH::keyboard_input,
                               kMessageFlag_None,
                               kPrimaryThreadTaskId,
                               kInputMgrTaskId,
                               to_cell(keyInput),
                               0,
                               nullptr);
}

static HashMap<kMEM_Engine, i32, Key> sGlfwKeyMap =
{
    {GLFW_KEY_SPACE,         kKEY_Space},
    {GLFW_KEY_APOSTROPHE,    kKEY_Apostrophe},
    {GLFW_KEY_COMMA,         kKEY_Comma},
    {GLFW_KEY_MINUS,         kKEY_Minus},
    {GLFW_KEY_PERIOD,        kKEY_Period},
    {GLFW_KEY_SLASH,         kKEY_Slash},
    {GLFW_KEY_0,             kKEY_0},
    {GLFW_KEY_1,             kKEY_1},
    {GLFW_KEY_2,             kKEY_2},
    {GLFW_KEY_3,             kKEY_3},
    {GLFW_KEY_4,             kKEY_4},
    {GLFW_KEY_5,             kKEY_5},
    {GLFW_KEY_6,             kKEY_6},
    {GLFW_KEY_7,             kKEY_7},
    {GLFW_KEY_8,             kKEY_8},
    {GLFW_KEY_9,             kKEY_9},
    {GLFW_KEY_SEMICOLON,     kKEY_Semicolon},
    {GLFW_KEY_EQUAL,         kKEY_Equal},
    {GLFW_KEY_A,             kKEY_A},
    {GLFW_KEY_B,             kKEY_B},
    {GLFW_KEY_C,             kKEY_C},
    {GLFW_KEY_D,             kKEY_D},
    {GLFW_KEY_E,             kKEY_E},
    {GLFW_KEY_F,             kKEY_F},
    {GLFW_KEY_G,             kKEY_G},
    {GLFW_KEY_H,             kKEY_H},
    {GLFW_KEY_I,             kKEY_I},
    {GLFW_KEY_J,             kKEY_J},
    {GLFW_KEY_K,             kKEY_K},
    {GLFW_KEY_L,             kKEY_L},
    {GLFW_KEY_M,             kKEY_M},
    {GLFW_KEY_N,             kKEY_N},
    {GLFW_KEY_O,             kKEY_O},
    {GLFW_KEY_P,             kKEY_P},
    {GLFW_KEY_Q,             kKEY_Q},
    {GLFW_KEY_R,             kKEY_R},
    {GLFW_KEY_S,             kKEY_S},
    {GLFW_KEY_T,             kKEY_T},
    {GLFW_KEY_U,             kKEY_U},
    {GLFW_KEY_V,             kKEY_V},
    {GLFW_KEY_W,             kKEY_W},
    {GLFW_KEY_X,             kKEY_X},
    {GLFW_KEY_Y,             kKEY_Y},
    {GLFW_KEY_Z,             kKEY_Z},
    {GLFW_KEY_LEFT_BRACKET,  kKEY_LeftBracket},
    {GLFW_KEY_BACKSLASH,     kKEY_Backslash},
    {GLFW_KEY_RIGHT_BRACKET, kKEY_RightBracket},
    {GLFW_KEY_GRAVE_ACCENT,  kKEY_GraveAccent},
    {GLFW_KEY_WORLD_1,       kKEY_World1},
    {GLFW_KEY_WORLD_2,       kKEY_World2},
    {GLFW_KEY_ESCAPE,        kKEY_Escape},
    {GLFW_KEY_ENTER,         kKEY_Enter},
    {GLFW_KEY_TAB,           kKEY_Tab},
    {GLFW_KEY_BACKSPACE,     kKEY_Backspace},
    {GLFW_KEY_INSERT,        kKEY_Insert},
    {GLFW_KEY_DELETE,        kKEY_Delete},
    {GLFW_KEY_RIGHT,         kKEY_Right},
    {GLFW_KEY_LEFT,          kKEY_Left},
    {GLFW_KEY_DOWN,          kKEY_Down},
    {GLFW_KEY_UP,            kKEY_Up},
    {GLFW_KEY_PAGE_UP,       kKEY_PageUp},
    {GLFW_KEY_PAGE_DOWN,     kKEY_PageDown},
    {GLFW_KEY_HOME,          kKEY_Home},
    {GLFW_KEY_END,           kKEY_End},
    {GLFW_KEY_CAPS_LOCK,     kKEY_CapsLock},
    {GLFW_KEY_SCROLL_LOCK,   kKEY_ScrollLock},
    {GLFW_KEY_NUM_LOCK,      kKEY_NumLock},
    {GLFW_KEY_PRINT_SCREEN,  kKEY_PrintScreen},
    {GLFW_KEY_PAUSE,         kKEY_Pause},
    {GLFW_KEY_F1,            kKEY_F1},
    {GLFW_KEY_F2,            kKEY_F2},
    {GLFW_KEY_F3,            kKEY_F3},
    {GLFW_KEY_F4,            kKEY_F4},
    {GLFW_KEY_F5,            kKEY_F5},
    {GLFW_KEY_F6,            kKEY_F6},
    {GLFW_KEY_F7,            kKEY_F7},
    {GLFW_KEY_F8,            kKEY_F8},
    {GLFW_KEY_F9,            kKEY_F9},
    {GLFW_KEY_F10,           kKEY_F10},
    {GLFW_KEY_F11,           kKEY_F11},
    {GLFW_KEY_F12,           kKEY_F12},
    {GLFW_KEY_F13,           kKEY_F13},
    {GLFW_KEY_F14,           kKEY_F14},
    {GLFW_KEY_F15,           kKEY_F15},
    {GLFW_KEY_F16,           kKEY_F16},
    {GLFW_KEY_F17,           kKEY_F17},
    {GLFW_KEY_F18,           kKEY_F18},
    {GLFW_KEY_F19,           kKEY_F19},
    {GLFW_KEY_F20,           kKEY_F20},
    {GLFW_KEY_F21,           kKEY_F21},
    {GLFW_KEY_F22,           kKEY_F22},
    {GLFW_KEY_F23,           kKEY_F23},
    {GLFW_KEY_F24,           kKEY_F24},
    {GLFW_KEY_F25,           kKEY_F25},
    {GLFW_KEY_KP_0,          kKEY_NumPad0},
    {GLFW_KEY_KP_1,          kKEY_NumPad1},
    {GLFW_KEY_KP_2,          kKEY_NumPad2},
    {GLFW_KEY_KP_3,          kKEY_NumPad3},
    {GLFW_KEY_KP_4,          kKEY_NumPad4},
    {GLFW_KEY_KP_5,          kKEY_NumPad5},
    {GLFW_KEY_KP_6,          kKEY_NumPad6},
    {GLFW_KEY_KP_7,          kKEY_NumPad7},
    {GLFW_KEY_KP_8,          kKEY_NumPad8},
    {GLFW_KEY_KP_9,          kKEY_NumPad9},
    {GLFW_KEY_KP_DECIMAL,    kKEY_NumPadDecimal},
    {GLFW_KEY_KP_DIVIDE,     kKEY_NumPadDivide},
    {GLFW_KEY_KP_MULTIPLY,   kKEY_NumPadMultiply},
    {GLFW_KEY_KP_SUBTRACT,   kKEY_NumPadSubtract},
    {GLFW_KEY_KP_ADD,        kKEY_NumPadAdd},
    {GLFW_KEY_KP_ENTER,      kKEY_NumPadEnter},
    {GLFW_KEY_KP_EQUAL,      kKEY_NumPadEqual},
    {GLFW_KEY_LEFT_SHIFT,    kKEY_LeftShift},
    {GLFW_KEY_LEFT_CONTROL,  kKEY_LeftControl},
    {GLFW_KEY_LEFT_ALT,      kKEY_LeftAlt},
    {GLFW_KEY_RIGHT_SHIFT,   kKEY_RightShift},
    {GLFW_KEY_RIGHT_CONTROL, kKEY_RightControl},
    {GLFW_KEY_RIGHT_ALT,     kKEY_RightAlt},
    {GLFW_KEY_MENU,          kKEY_Menu},

    {GLFW_KEY_UNKNOWN,       kKEY_NONE}
};

Key convert_glfw_key(int glfwKey)
{
    auto it = sGlfwKeyMap.find(glfwKey);
    if (it != sGlfwKeyMap.end())
        return it->second;
    else
        return kKEY_NONE;
}

void kill_focus()
{
    broadcast_targeted_message(HASH::kill_focus,
                               kMessageFlag_None,
                               kPrimaryThreadTaskId,
                               kInputMgrTaskId,
                               to_cell(0),
                               0,
                               nullptr);
}

void poll_pad_input()
{
    InputMgr & inputMgr = TaskMaster::task_master_for_active_thread().inputMgr();
    inputMgr.pollPadInput();
}

static const f64 kMouseUpdateInterval = 1.0f / 60.0f;
static f64 sLastUpdateTime = 0.0f;
static i32 sMouseDeltaX;
static i32 sMouseDeltaY;

void send_mouse_input()
{
    // send mouse movements if timeout expired
    if (sMouseDeltaX != 0 || sMouseDeltaY != 0)
    {
        f64 nowTime = now();
        if (nowTime - sLastUpdateTime >= kMouseUpdateInterval)
        {
            MouseInput::Movement movement;
            movement.xDelta = sMouseDeltaX;
            movement.yDelta = sMouseDeltaY;

            broadcast_targeted_message(HASH::mouse_move,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(movement),
                                       0,
                                       nullptr);

            sMouseDeltaX = 0;
            sMouseDeltaY = 0;
            sLastUpdateTime = nowTime;
        }
    }
}

void process_mouse_input(const void * pMouseInfo)
{
/*
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
        if (mouseInput.buttons.buttonFlags & kMEVT_1Down)
        {
            keyInput.key = kKEY_Mouse1;
            keyInput.action = kKST_Press;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        else if (mouseInput.buttons.buttonFlags & kMEVT_1Up)
        {
            keyInput.key = kKEY_Mouse1;
            keyInput.action = kKST_Release;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        if (mouseInput.buttons.buttonFlags & kMEVT_2Down)
        {
            keyInput.key = kKEY_Mouse2;
            keyInput.action = kKST_Press;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        else if (mouseInput.buttons.buttonFlags & kMEVT_2Up)
        {
            keyInput.key = kKEY_Mouse2;
            keyInput.action = kKST_Release;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        if (mouseInput.buttons.buttonFlags & kMEVT_3Down)
        {
            keyInput.key = kKEY_Mouse3;
            keyInput.action = kKST_Press;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        else if (mouseInput.buttons.buttonFlags & kMEVT_3Up)
        {
            keyInput.key = kKEY_Mouse3;
            keyInput.action = kKST_Release;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        if (mouseInput.buttons.buttonFlags & kMEVT_4Down)
        {
            keyInput.key = kKEY_Mouse4;
            keyInput.action = kKST_Press;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        else if (mouseInput.buttons.buttonFlags & kMEVT_4Up)
        {
            keyInput.key = kKEY_Mouse4;
            keyInput.action = kKST_Release;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        if (mouseInput.buttons.buttonFlags & kMEVT_5Down)
        {
            keyInput.key = kKEY_Mouse5;
            keyInput.action = kKST_Press;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }
        else if (mouseInput.buttons.buttonFlags & kMEVT_5Up)
        {
            keyInput.key = kKEY_Mouse5;
            keyInput.action = kKST_Release;
            broadcast_targeted_message(HASH::keyboard_input,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(keyInput),
                                       0,
                                       nullptr);
        }

        if (mouseInput.buttons.buttonFlags & kMEVT_Wheel)
        {
            broadcast_targeted_message(HASH::mouse_wheel,
                                       kMessageFlag_None,
                                       kPrimaryThreadTaskId,
                                       kInputMgrTaskId,
                                       to_cell(mouseInput.buttons.wheelMovement),
                                       0,
                                       nullptr);
        }
    }
*/
}


} // namespace gaen
