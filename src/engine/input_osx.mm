//------------------------------------------------------------------------------
// input_osx.cpp - Converts platform key codes to Gaen keycode hashes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#import <Cocoa/Cocoa.h>

#include "core/HashMap.h"

#include "engine/input.h"

namespace gaen
{

// These are pulled out of Carbon Events.h header
enum {
  kVK_ANSI_A                    = 0x00,
  kVK_ANSI_S                    = 0x01,
  kVK_ANSI_D                    = 0x02,
  kVK_ANSI_F                    = 0x03,
  kVK_ANSI_H                    = 0x04,
  kVK_ANSI_G                    = 0x05,
  kVK_ANSI_Z                    = 0x06,
  kVK_ANSI_X                    = 0x07,
  kVK_ANSI_C                    = 0x08,
  kVK_ANSI_V                    = 0x09,
  kVK_ANSI_B                    = 0x0B,
  kVK_ANSI_Q                    = 0x0C,
  kVK_ANSI_W                    = 0x0D,
  kVK_ANSI_E                    = 0x0E,
  kVK_ANSI_R                    = 0x0F,
  kVK_ANSI_Y                    = 0x10,
  kVK_ANSI_T                    = 0x11,
  kVK_ANSI_1                    = 0x12,
  kVK_ANSI_2                    = 0x13,
  kVK_ANSI_3                    = 0x14,
  kVK_ANSI_4                    = 0x15,
  kVK_ANSI_6                    = 0x16,
  kVK_ANSI_5                    = 0x17,
  kVK_ANSI_Equal                = 0x18,
  kVK_ANSI_9                    = 0x19,
  kVK_ANSI_7                    = 0x1A,
  kVK_ANSI_Minus                = 0x1B,
  kVK_ANSI_8                    = 0x1C,
  kVK_ANSI_0                    = 0x1D,
  kVK_ANSI_RightBracket         = 0x1E,
  kVK_ANSI_O                    = 0x1F,
  kVK_ANSI_U                    = 0x20,
  kVK_ANSI_LeftBracket          = 0x21,
  kVK_ANSI_I                    = 0x22,
  kVK_ANSI_P                    = 0x23,
  kVK_ANSI_L                    = 0x25,
  kVK_ANSI_J                    = 0x26,
  kVK_ANSI_Quote                = 0x27,
  kVK_ANSI_K                    = 0x28,
  kVK_ANSI_Semicolon            = 0x29,
  kVK_ANSI_Backslash            = 0x2A,
  kVK_ANSI_Comma                = 0x2B,
  kVK_ANSI_Slash                = 0x2C,
  kVK_ANSI_N                    = 0x2D,
  kVK_ANSI_M                    = 0x2E,
  kVK_ANSI_Period               = 0x2F,
  kVK_ANSI_Grave                = 0x32,
  kVK_ANSI_KeypadDecimal        = 0x41,
  kVK_ANSI_KeypadMultiply       = 0x43,
  kVK_ANSI_KeypadPlus           = 0x45,
  kVK_ANSI_KeypadClear          = 0x47,
  kVK_ANSI_KeypadDivide         = 0x4B,
  kVK_ANSI_KeypadEnter          = 0x4C,
  kVK_ANSI_KeypadMinus          = 0x4E,
  kVK_ANSI_KeypadEquals         = 0x51,
  kVK_ANSI_Keypad0              = 0x52,
  kVK_ANSI_Keypad1              = 0x53,
  kVK_ANSI_Keypad2              = 0x54,
  kVK_ANSI_Keypad3              = 0x55,
  kVK_ANSI_Keypad4              = 0x56,
  kVK_ANSI_Keypad5              = 0x57,
  kVK_ANSI_Keypad6              = 0x58,
  kVK_ANSI_Keypad7              = 0x59,
  kVK_ANSI_Keypad8              = 0x5B,
  kVK_ANSI_Keypad9              = 0x5C
};

/* keycodes for keys that are independent of keyboard layout*/
enum {
  kVK_Return                    = 0x24,
  kVK_Tab                       = 0x30,
  kVK_Space                     = 0x31,
  kVK_Delete                    = 0x33,
  kVK_Escape                    = 0x35,
  kVK_Command                   = 0x37,
  kVK_Shift                     = 0x38,
  kVK_CapsLock                  = 0x39,
  kVK_Option                    = 0x3A,
  kVK_Control                   = 0x3B,
  kVK_RightShift                = 0x3C,
  kVK_RightOption               = 0x3D,
  kVK_RightControl              = 0x3E,
  kVK_Function                  = 0x3F,
  kVK_F17                       = 0x40,
  kVK_VolumeUp                  = 0x48,
  kVK_VolumeDown                = 0x49,
  kVK_Mute                      = 0x4A,
  kVK_F18                       = 0x4F,
  kVK_F19                       = 0x50,
  kVK_F20                       = 0x5A,
  kVK_F5                        = 0x60,
  kVK_F6                        = 0x61,
  kVK_F7                        = 0x62,
  kVK_F3                        = 0x63,
  kVK_F8                        = 0x64,
  kVK_F9                        = 0x65,
  kVK_F11                       = 0x67,
  kVK_F13                       = 0x69,
  kVK_F16                       = 0x6A,
  kVK_F14                       = 0x6B,
  kVK_F10                       = 0x6D,
  kVK_F12                       = 0x6F,
  kVK_F15                       = 0x71,
  kVK_Help                      = 0x72,
  kVK_Home                      = 0x73,
  kVK_PageUp                    = 0x74,
  kVK_ForwardDelete             = 0x75,
  kVK_F4                        = 0x76,
  kVK_End                       = 0x77,
  kVK_F2                        = 0x78,
  kVK_PageDown                  = 0x79,
  kVK_F1                        = 0x7A,
  kVK_LeftArrow                 = 0x7B,
  kVK_RightArrow                = 0x7C,
  kVK_DownArrow                 = 0x7D,
  kVK_UpArrow                   = 0x7E
};

/* ISO keyboards only*/
enum {
  kVK_ISO_Section               = 0x0A
};

/* JIS keyboards only*/
enum {
  kVK_JIS_Yen                   = 0x5D,
  kVK_JIS_Underscore            = 0x5E,
  kVK_JIS_KeypadComma           = 0x5F,
  kVK_JIS_Eisu                  = 0x66,
  kVK_JIS_Kana                  = 0x68
};



static HashMap<kMEM_Engine, u32, KeyCode> sKeyMap;

KeyInput convert_key_input(const void * pKeyInfo)
{

    // initialize if first time called
    if (sKeyMap.size() == 0)
    {
        sKeyMap[kVK_Delete]              = kKEY_Backspace;
        sKeyMap[kVK_Tab]                 = kKEY_Tab;
        sKeyMap[kVK_ANSI_KeypadClear]    = kKEY_Clear;
        sKeyMap[kVK_Return]              = kKEY_Enter;
        sKeyMap[kVK_CapsLock]            = kKEY_CapsLock;
        sKeyMap[kVK_Escape]              = kKEY_Escape;
        sKeyMap[kVK_Space]               = kKEY_Space;
        sKeyMap[kVK_LeftArrow]           = kKEY_Left;
        sKeyMap[kVK_UpArrow]             = kKEY_Up;
        sKeyMap[kVK_RightArrow]          = kKEY_Right;
        sKeyMap[kVK_DownArrow]           = kKEY_Down;
        sKeyMap[kVK_ForwardDelete]       = kKEY_Delete;
        sKeyMap[kVK_ANSI_0]              = kKEY_0;
        sKeyMap[kVK_ANSI_1]              = kKEY_1;
        sKeyMap[kVK_ANSI_2]              = kKEY_2;
        sKeyMap[kVK_ANSI_3]              = kKEY_3;
        sKeyMap[kVK_ANSI_4]              = kKEY_4;
        sKeyMap[kVK_ANSI_5]              = kKEY_5;
        sKeyMap[kVK_ANSI_6]              = kKEY_6;
        sKeyMap[kVK_ANSI_7]              = kKEY_7;
        sKeyMap[kVK_ANSI_8]              = kKEY_8;
        sKeyMap[kVK_ANSI_9]              = kKEY_9;
        sKeyMap[kVK_ANSI_A]              = kKEY_A;
        sKeyMap[kVK_ANSI_B]              = kKEY_B;
        sKeyMap[kVK_ANSI_C]              = kKEY_C;
        sKeyMap[kVK_ANSI_D]              = kKEY_D;
        sKeyMap[kVK_ANSI_E]              = kKEY_E;
        sKeyMap[kVK_ANSI_F]              = kKEY_F;
        sKeyMap[kVK_ANSI_G]              = kKEY_G;
        sKeyMap[kVK_ANSI_H]              = kKEY_H;
        sKeyMap[kVK_ANSI_I]              = kKEY_I;
        sKeyMap[kVK_ANSI_J]              = kKEY_J;
        sKeyMap[kVK_ANSI_K]              = kKEY_K;
        sKeyMap[kVK_ANSI_L]              = kKEY_L;
        sKeyMap[kVK_ANSI_M]              = kKEY_M;
        sKeyMap[kVK_ANSI_N]              = kKEY_N;
        sKeyMap[kVK_ANSI_O]              = kKEY_O;
        sKeyMap[kVK_ANSI_P]              = kKEY_P;
        sKeyMap[kVK_ANSI_Q]              = kKEY_Q;
        sKeyMap[kVK_ANSI_R]              = kKEY_R;
        sKeyMap[kVK_ANSI_S]              = kKEY_S;
        sKeyMap[kVK_ANSI_T]              = kKEY_T;
        sKeyMap[kVK_ANSI_U]              = kKEY_U;
        sKeyMap[kVK_ANSI_V]              = kKEY_V;
        sKeyMap[kVK_ANSI_W]              = kKEY_W;
        sKeyMap[kVK_ANSI_X]              = kKEY_X;
        sKeyMap[kVK_ANSI_Y]              = kKEY_Y;
        sKeyMap[kVK_ANSI_Z]              = kKEY_Z;
        sKeyMap[kVK_ANSI_Keypad0]        = kKEY_Num0;
        sKeyMap[kVK_ANSI_Keypad1]        = kKEY_Num1;
        sKeyMap[kVK_ANSI_Keypad2]        = kKEY_Num2;
        sKeyMap[kVK_ANSI_Keypad3]        = kKEY_Num3;
        sKeyMap[kVK_ANSI_Keypad4]        = kKEY_Num4;
        sKeyMap[kVK_ANSI_Keypad5]        = kKEY_Num5;
        sKeyMap[kVK_ANSI_Keypad6]        = kKEY_Num6;
        sKeyMap[kVK_ANSI_Keypad7]        = kKEY_Num7;
        sKeyMap[kVK_ANSI_Keypad8]        = kKEY_Num8;
        sKeyMap[kVK_ANSI_Keypad9]        = kKEY_Num9;
        sKeyMap[kVK_ANSI_KeypadMultiply] = kKEY_NumMul;
        sKeyMap[kVK_ANSI_KeypadPlus]     = kKEY_NumAdd;
        sKeyMap[kVK_ANSI_KeypadMinus]    = kKEY_NumSub;
        sKeyMap[kVK_ANSI_KeypadDecimal]  = kKEY_NumDot;
        sKeyMap[kVK_ANSI_KeypadDivide]   = kKEY_NumDiv;
        sKeyMap[kVK_F1]                  = kKEY_F1;
        sKeyMap[kVK_F2]                  = kKEY_F2;
        sKeyMap[kVK_F3]                  = kKEY_F3;
        sKeyMap[kVK_F4]                  = kKEY_F4;
        sKeyMap[kVK_F5]                  = kKEY_F5;
        sKeyMap[kVK_F6]                  = kKEY_F6;
        sKeyMap[kVK_F7]                  = kKEY_F7;
        sKeyMap[kVK_F8]                  = kKEY_F8;
        sKeyMap[kVK_F9]                  = kKEY_F9;
        sKeyMap[kVK_F10]                 = kKEY_F10;
        sKeyMap[kVK_F11]                 = kKEY_F11;
        sKeyMap[kVK_F12]                 = kKEY_F12;
        sKeyMap[kVK_F13]                 = kKEY_F13;
        sKeyMap[kVK_F14]                 = kKEY_F14;
        sKeyMap[kVK_F15]                 = kKEY_F15;
        sKeyMap[kVK_F16]                 = kKEY_F16;
        sKeyMap[kVK_F17]                 = kKEY_F17;
        sKeyMap[kVK_F18]                 = kKEY_F18;
        sKeyMap[kVK_F19]                 = kKEY_F19;
        sKeyMap[kVK_F20]                 = kKEY_F20;
        sKeyMap[kVK_ANSI_Semicolon]      = kKEY_Semicolon;
        sKeyMap[kVK_ANSI_Equal]          = kKEY_Equal;
        sKeyMap[kVK_ANSI_Comma]          = kKEY_Comma;
        sKeyMap[kVK_ANSI_Minus]          = kKEY_Dash;
        sKeyMap[kVK_ANSI_Period]         = kKEY_Period;
        sKeyMap[kVK_ANSI_Slash]          = kKEY_Slash;
        sKeyMap[kVK_ANSI_Grave]          = kKEY_BackTick;
        sKeyMap[kVK_ANSI_LeftBracket]    = kKEY_LeftBracket;
        sKeyMap[kVK_ANSI_Backslash]      = kKEY_BackSlash;
        sKeyMap[kVK_ANSI_RightBracket]   = kKEY_RightBracket;
        sKeyMap[kVK_ANSI_Quote]          = kKEY_Tick;

        sKeyMap[kVK_Shift]               = kKEY_LeftShift;
        sKeyMap[kVK_RightShift]          = kKEY_RightShift;
        sKeyMap[kVK_Control]             = kKEY_LeftControl;
        sKeyMap[kVK_RightControl]        = kKEY_RightControl;
        // LORRTODO - Add command and alt keys
    }

    const KeyInput * pOSXKeyInput = reinterpret_cast<const KeyInput*>(pKeyInfo);

    KeyInput keyInput;

    auto mapIt = sKeyMap.find(pOSXKeyInput->keyCode);
    if (mapIt != sKeyMap.end())
        keyInput.keyCode = mapIt->second;
    else
        PANIC("Failed to find virtual key in sKeyMap: %u", keyInput.keyCode);

    keyInput.keyEvent = pOSXKeyInput->keyEvent;
    keyInput.deviceId = pOSXKeyInput->deviceId;
    
    return keyInput;
}

} // namespace gaen
