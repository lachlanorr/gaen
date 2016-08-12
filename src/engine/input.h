//------------------------------------------------------------------------------
// input.h - Low level input handling
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

#ifndef GAEN_ENGINE_KEYCODES_H
#define GAEN_ENGINE_KEYCODES_H

#include "core/base_defines.h"
#include "core/logging.h"

#include "engine/Block.h"

namespace gaen
{

enum InputDevice
{
    kIDV_Keyboard,
    kIDV_Mouse,
    kIDV_Controller
};

enum KeyCode
{
    kKEY_0                = 0,
    kKEY_1                = 1,
    kKEY_2                = 2,
    kKEY_3                = 3,
    kKEY_4                = 4,
    kKEY_5                = 5,
    kKEY_6                = 6,
    kKEY_7                = 7,
    kKEY_8                = 8,
    kKEY_9                = 9,
    kKEY_A                = 10,
    kKEY_B                = 11,
    kKEY_C                = 12,
    kKEY_D                = 13,
    kKEY_E                = 14,
    kKEY_F                = 15,
    kKEY_G                = 16,
    kKEY_H                = 17,
    kKEY_I                = 18,
    kKEY_J                = 19,
    kKEY_K                = 20,
    kKEY_L                = 21,
    kKEY_M                = 22,
    kKEY_N                = 23,
    kKEY_O                = 24,
    kKEY_P                = 25,
    kKEY_Q                = 26,
    kKEY_R                = 27,
    kKEY_S                = 28,
    kKEY_T                = 29,
    kKEY_U                = 30,
    kKEY_V                = 31,
    kKEY_W                = 32,
    kKEY_X                = 33,
    kKEY_Y                = 34,
    kKEY_Z                = 35,
    kKEY_Num0             = 36,
    kKEY_Num1             = 37,
    kKEY_Num2             = 38,
    kKEY_Num3             = 39,
    kKEY_Num4             = 40,
    kKEY_Num5             = 41,
    kKEY_Num6             = 42,
    kKEY_Num7             = 43,
    kKEY_Num8             = 44,
    kKEY_Num9             = 45,
    kKEY_NumMul           = 46,
    kKEY_NumAdd           = 47,
    kKEY_NumSub           = 48,
    kKEY_NumDot           = 49,
    kKEY_NumDiv           = 50,
    kKEY_LeftShift        = 51,
    kKEY_RightShift       = 52,
    kKEY_LeftControl      = 53,
    kKEY_RightControl     = 54,
    kKEY_LeftAlt          = 55,
    kKEY_RightAlt         = 56,
    kKEY_Backspace        = 57,
    kKEY_Tab              = 58,
    kKEY_Clear            = 59,
    kKEY_Enter            = 60,
    kKEY_Pause            = 61,
    kKEY_CapsLock         = 62,
    kKEY_Escape           = 63,
    kKEY_Space            = 64,
    kKEY_PageUp           = 65,
    kKEY_PageDown         = 66,
    kKEY_End              = 67,
    kKEY_Home             = 68,
    kKEY_Left             = 69,
    kKEY_Up               = 70,
    kKEY_Right            = 71,
    kKEY_Down             = 72,
    kKEY_PrintScreen      = 73,
    kKEY_Insert           = 74,
    kKEY_Delete           = 75,
    kKEY_NumLock          = 76,
    kKEY_ScrollLock       = 77,
    kKEY_Semicolon        = 78,
    kKEY_Equal            = 79,
    kKEY_Comma            = 80,
    kKEY_Dash             = 81,
    kKEY_Period           = 82,
    kKEY_Slash            = 83,
    kKEY_BackTick         = 84,
    kKEY_LeftBracket      = 85,
    kKEY_BackSlash        = 86,
    kKEY_BracketBackSlash = 87,
    kKEY_RightBracket     = 88,
    kKEY_Tick             = 89,
    kKEY_F1               = 90,
    kKEY_F2               = 91,
    kKEY_F3               = 92,
    kKEY_F4               = 93,
    kKEY_F5               = 94,
    kKEY_F6               = 95,
    kKEY_F7               = 96,
    kKEY_F8               = 97,
    kKEY_F9               = 98,
    kKEY_F10              = 99,
    kKEY_F11              = 100,
    kKEY_F12              = 101,
    kKEY_F13              = 102,
    kKEY_F14              = 103,
    kKEY_F15              = 104,
    kKEY_F16              = 105,
    kKEY_F17              = 106,
    kKEY_F18              = 107,
    kKEY_F19              = 108,
    kKEY_F20              = 109,
    kKEY_F21              = 110,
    kKEY_F22              = 111,
    kKEY_F23              = 112,
    kKEY_F24              = 113,

    kKEY_Mouse1           = 121,
    kKEY_Mouse2           = 122,
    kKEY_Mouse3           = 123,
    kKEY_Mouse4           = 124,
    kKEY_Mouse5           = 125,

    kKEY_NOKEY            = 126 // Indicative of no key pressed, used when specifying no key in a 4 key combo
};

enum KeyState
{
    kKST_Up   = 0,
    kKST_Down = 1
};

struct KeyInput
{
    u32 keyCode:7;
    u32 keyEvent:1;
    u32 deviceId:4; // keep this to 4 so we can pack taskid + deviceid into 32 bits
    u32 PADDING__:20;

    KeyInput()
    {}

    KeyInput(cell c)
    {
        *this = *reinterpret_cast<KeyInput*>(&c);
    }
};

enum ControllerButton
{
    kCTRL_DpadUp     = 0,
    kCTRL_DpadDown   = 1,
    kCTRL_DpadLeft   = 2,
    kCTRL_DpadRight  = 3,

    kCTRL_BtnA       = 4,
    kCTRL_BtnB       = 5,
    kCTRL_BtnX       = 6,
    kCTRL_BtnY       = 7,

    kCTRL_R1         = 8,
    kCTRL_R2         = 9,
    kCTRL_R3         = 10,

    kCTRL_L1         = 11,
    kCTRL_L2         = 12,
    kCTRL_L3         = 13,

    kCTRL_Opt0       = 14,
    kCTRL_Opt1       = 15,
    kCTRL_Opt2       = 16,
    kCTRL_Opt3       = 17
};

inline cell to_cell(KeyInput val)
{
    static_assert(sizeof(KeyInput) == 4, "KeyInput must be 4 bytes to be compatible with cell type");
    cell c;
    c.u = *reinterpret_cast<const u32*>(&val);
    return c;
}

// Convert string to KeyCode, useful when parsing config files
KeyCode lookup_key_code(const char * str);

// KeyInput is 4 bytes to simplify passing it around as a message payload.
// We have 16 unused bits, maybe find something interesting for them
// in the future.

// void* to accomodate different platforms structs/values for key code
// information.

void process_key_input(const void * pKeyInfo);
KeyInput convert_key_input(const void * pKeyInfo);
void kill_focus();


// Mouse stuff

enum MouseEvent
{
    kMEVT_1Down = 0x0001,
    kMEVT_1Up   = 0x0002,

    kMEVT_2Down = 0x0004,
    kMEVT_2Up   = 0x0008,

    kMEVT_3Down = 0x0010,
    kMEVT_3Up   = 0x0020,

    kMEVT_4Down = 0x0040,
    kMEVT_4Up   = 0x0080,

    kMEVT_5Down = 0x0100,
    kMEVT_5Up   = 0x0200,

    kMEVT_Wheel = 0x0400
};

struct MouseInput
{
    struct Buttons
    {
        u16 buttonFlags;
        i16 wheelMovement;
    } buttons;

    struct Movement
    {
        i16 xDelta;
        i16 yDelta;

        Movement() {}

        Movement(cell c)
        {
            *this = *reinterpret_cast<Movement*>(&c);
        }
    } movement;
};

static_assert(sizeof(MouseInput) == 8, "MouseInput not 8 bytes as expected");

inline cell to_cell(MouseInput::Movement val)
{
    static_assert(sizeof(MouseInput::Movement) == 4, "MouseInput::Movement must be 4 bytes to be compatible with cell type");
    cell c;
    c.u = *reinterpret_cast<u32*>(&val);
    return c;
}

void send_mouse_input();
void process_mouse_input(const void * pMouseInfo);
MouseInput convert_mouse_input(const void * pKeyInfo);

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_KEYCODES_H
