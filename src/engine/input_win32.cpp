//------------------------------------------------------------------------------
// input_win32.cpp - Converts platform key codes to Gaen keycode hashes
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "core/HashMap.h"
#include "core/logging.h"

#include "engine/input.h"

// These aren't defined in winuser.h, define them here for consistency.
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A


namespace gaen
{

static HashMap<kMEM_Engine, u32, KeyCode> sKeyMap;
static HashMap<kMEM_Engine, HANDLE, u32> sDeviceIdMap;

KeyInput convert_key_input(const void * pKeyInfo)
{
    // initialize if first time called
    if (sKeyMap.size() == 0)
    {
        sKeyMap[VK_BACK]       = kKEY_Backspace;        // 0x08 // BACKSPACE
        sKeyMap[VK_TAB]        = kKEY_Tab;              // 0x09
        sKeyMap[VK_CLEAR]      = kKEY_Clear;            // 0x0C // NUM PAD 5 with NUMLOCK off
        sKeyMap[VK_RETURN]     = kKEY_Enter;            // 0x0D
        sKeyMap[VK_PAUSE]      = kKEY_Pause;            // 0x13 
        sKeyMap[VK_CAPITAL]    = kKEY_CapsLock;         // 0x14 // CAPS LOCK
        sKeyMap[VK_ESCAPE]     = kKEY_Escape;           // 0x1B
        sKeyMap[VK_SPACE]      = kKEY_Space;            // 0x20
        sKeyMap[VK_PRIOR]      = kKEY_PageUp;           // 0x21 // PAGE UP
        sKeyMap[VK_NEXT]       = kKEY_PageDown;         // 0x22 // PAGE DOWN
        sKeyMap[VK_END]        = kKEY_End;              // 0x23
        sKeyMap[VK_HOME]       = kKEY_Home;             // 0x24
        sKeyMap[VK_LEFT]       = kKEY_Left;             // 0x25 // LEFT ARROW
        sKeyMap[VK_UP]         = kKEY_Up;               // 0x26 // UP ARROW
        sKeyMap[VK_RIGHT]      = kKEY_Right;            // 0x27 // RIGHT ARROW
        sKeyMap[VK_DOWN]       = kKEY_Down;             // 0x28 // DOWN ARROW
        sKeyMap[VK_SNAPSHOT]   = kKEY_PrintScreen;      // 0x2C // PRINT SCREEN
        sKeyMap[VK_INSERT]     = kKEY_Insert;           // 0x2D
        sKeyMap[VK_DELETE]     = kKEY_Delete;           // 0x2E
        sKeyMap[VK_0]          = kKEY_0;                // 0x30
        sKeyMap[VK_1]          = kKEY_1;                // 0x31
        sKeyMap[VK_2]          = kKEY_2;                // 0x32
        sKeyMap[VK_3]          = kKEY_3;                // 0x33
        sKeyMap[VK_4]          = kKEY_4;                // 0x34
        sKeyMap[VK_5]          = kKEY_5;                // 0x35
        sKeyMap[VK_6]          = kKEY_6;                // 0x36
        sKeyMap[VK_7]          = kKEY_7;                // 0x37
        sKeyMap[VK_8]          = kKEY_8;                // 0x38
        sKeyMap[VK_9]          = kKEY_9;                // 0x39
        sKeyMap[VK_A]          = kKEY_A;                // 0x41
        sKeyMap[VK_B]          = kKEY_B;                // 0x42
        sKeyMap[VK_C]          = kKEY_C;                // 0x43
        sKeyMap[VK_D]          = kKEY_D;                // 0x44
        sKeyMap[VK_E]          = kKEY_E;                // 0x45
        sKeyMap[VK_F]          = kKEY_F;                // 0x46
        sKeyMap[VK_G]          = kKEY_G;                // 0x47
        sKeyMap[VK_H]          = kKEY_H;                // 0x48
        sKeyMap[VK_I]          = kKEY_I;                // 0x49
        sKeyMap[VK_J]          = kKEY_J;                // 0x4A
        sKeyMap[VK_K]          = kKEY_K;                // 0x4B
        sKeyMap[VK_L]          = kKEY_L;                // 0x4C
        sKeyMap[VK_M]          = kKEY_M;                // 0x4D
        sKeyMap[VK_N]          = kKEY_N;                // 0x4E
        sKeyMap[VK_O]          = kKEY_O;                // 0x4F
        sKeyMap[VK_P]          = kKEY_P;                // 0x50
        sKeyMap[VK_Q]          = kKEY_Q;                // 0x51
        sKeyMap[VK_R]          = kKEY_R;                // 0x52
        sKeyMap[VK_S]          = kKEY_S;                // 0x53
        sKeyMap[VK_T]          = kKEY_T;                // 0x54
        sKeyMap[VK_U]          = kKEY_U;                // 0x55
        sKeyMap[VK_V]          = kKEY_V;                // 0x56
        sKeyMap[VK_W]          = kKEY_W;                // 0x57
        sKeyMap[VK_X]          = kKEY_X;                // 0x58
        sKeyMap[VK_Y]          = kKEY_Y;                // 0x59
        sKeyMap[VK_Z]          = kKEY_Z;                // 0x5A
        sKeyMap[VK_NUMPAD0]    = kKEY_Num0;             // 0x60
        sKeyMap[VK_NUMPAD1]    = kKEY_Num1;             // 0x61
        sKeyMap[VK_NUMPAD2]    = kKEY_Num2;             // 0x62
        sKeyMap[VK_NUMPAD3]    = kKEY_Num3;             // 0x63
        sKeyMap[VK_NUMPAD4]    = kKEY_Num4;             // 0x64
        sKeyMap[VK_NUMPAD5]    = kKEY_Num5;             // 0x65
        sKeyMap[VK_NUMPAD6]    = kKEY_Num6;             // 0x66
        sKeyMap[VK_NUMPAD7]    = kKEY_Num7;             // 0x67
        sKeyMap[VK_NUMPAD8]    = kKEY_Num8;             // 0x68
        sKeyMap[VK_NUMPAD9]    = kKEY_Num9;             // 0x69
        sKeyMap[VK_MULTIPLY]   = kKEY_NumMul;           // 0x6A
        sKeyMap[VK_ADD]        = kKEY_NumAdd;           // 0x6B
        sKeyMap[VK_SUBTRACT]   = kKEY_NumSub;           // 0x6D
        sKeyMap[VK_DECIMAL]    = kKEY_NumDot;           // 0x6E
        sKeyMap[VK_DIVIDE]     = kKEY_NumDiv;           // 0x6F
        sKeyMap[VK_F1]         = kKEY_F1;               // 0x70
        sKeyMap[VK_F2]         = kKEY_F2;               // 0x71
        sKeyMap[VK_F3]         = kKEY_F3;               // 0x72
        sKeyMap[VK_F4]         = kKEY_F4;               // 0x73
        sKeyMap[VK_F5]         = kKEY_F5;               // 0x74
        sKeyMap[VK_F6]         = kKEY_F6;               // 0x75
        sKeyMap[VK_F7]         = kKEY_F7;               // 0x76
        sKeyMap[VK_F8]         = kKEY_F8;               // 0x77
        sKeyMap[VK_F9]         = kKEY_F9;               // 0x78
        sKeyMap[VK_F10]        = kKEY_F10;              // 0x79
        sKeyMap[VK_F11]        = kKEY_F11;              // 0x7A
        sKeyMap[VK_F12]        = kKEY_F12;              // 0x7B
        sKeyMap[VK_F13]        = kKEY_F13;              // 0x7C
        sKeyMap[VK_F14]        = kKEY_F14;              // 0x7D
        sKeyMap[VK_F15]        = kKEY_F15;              // 0x7E
        sKeyMap[VK_F16]        = kKEY_F16;              // 0x7F
        sKeyMap[VK_F17]        = kKEY_F17;              // 0x80
        sKeyMap[VK_F18]        = kKEY_F18;              // 0x81
        sKeyMap[VK_F19]        = kKEY_F19;              // 0x82
        sKeyMap[VK_F20]        = kKEY_F20;              // 0x83
        sKeyMap[VK_F21]        = kKEY_F21;              // 0x84
        sKeyMap[VK_F22]        = kKEY_F22;              // 0x85
        sKeyMap[VK_F23]        = kKEY_F23;              // 0x86
        sKeyMap[VK_F24]        = kKEY_F24;              // 0x87
        sKeyMap[VK_NUMLOCK]    = kKEY_NumLock;          // 0x90
        sKeyMap[VK_SCROLL]     = kKEY_ScrollLock;       // 0x91   // SCROLL LOCK
        sKeyMap[VK_OEM_1]      = kKEY_Semicolon;        // 0xBA   // ';:' for US
        sKeyMap[VK_OEM_PLUS]   = kKEY_Equal;            // 0xBB   // '+' any country
        sKeyMap[VK_OEM_COMMA]  = kKEY_Comma;            // 0xBC   // ',' any country
        sKeyMap[VK_OEM_MINUS]  = kKEY_Dash;             // 0xBD   // '-' any country
        sKeyMap[VK_OEM_PERIOD] = kKEY_Period;           // 0xBE   // '.' any country
        sKeyMap[VK_OEM_2]      = kKEY_Slash;            // 0xBF   // '/?' for US
        sKeyMap[VK_OEM_3]      = kKEY_BackTick;         // 0xC0   // '`~' for US
        sKeyMap[VK_OEM_4]      = kKEY_LeftBracket;      // 0xDB  //  '[{' for US
        sKeyMap[VK_OEM_5]      = kKEY_BackSlash;        // 0xDC  //  '\|' for US
        sKeyMap[VK_OEM_6]      = kKEY_RightBracket;     // 0xDD  //  ']}' for US
        sKeyMap[VK_OEM_7]      = kKEY_Tick;             // 0xDE  //  ''"' for US
        sKeyMap[VK_OEM_102]    = kKEY_BracketBackSlash; // 0xE2 - Angle bracket or backslash on RT 102-key keyboard
    }

    const RAWINPUT * pRaw = reinterpret_cast<const RAWINPUT*>(pKeyInfo);
    ASSERT(pRaw->header.dwType == RIM_TYPEKEYBOARD);

    KeyInput keyInput;

    keyInput.keyEvent = (pRaw->data.keyboard.Flags & RI_KEY_BREAK) ? kKST_Up : kKST_Down;


    // Windows virtual keys decode is straight forward, except for shift, control and alt.
    // These virtual keys do encode left vs. right, so we have to check the flags.
    u32 vkey = pRaw->data.keyboard.VKey;
    switch (vkey)
    {
    case VK_SHIFT:
        keyInput.keyCode = (pRaw->data.keyboard.Flags & RI_KEY_E0) ? kKEY_LeftShift : kKEY_RightShift;
        break;
    case VK_CONTROL:
        keyInput.keyCode = (pRaw->data.keyboard.Flags & RI_KEY_E0) ? kKEY_LeftControl : kKEY_RightControl;
        break;
    case VK_MENU:
        keyInput.keyCode = (pRaw->data.keyboard.Flags & RI_KEY_E0) ? kKEY_LeftAlt : kKEY_RightAlt;
        break;
    default:
        // other virtual key codes are directly translatable with our map
        auto mapIt = sKeyMap.find(vkey);
        if (mapIt != sKeyMap.end())
            keyInput.keyCode = mapIt->second;
        else
            ERR("Failed to find virtual key in sKeyMap: %u", vkey);
        break;
    }

    // identify keyboard ID
    auto devIt = sDeviceIdMap.find(pRaw->header.hDevice);
    if (devIt != sDeviceIdMap.end())
        keyInput.deviceId = devIt->second;
    else
    {
        keyInput.deviceId = sDeviceIdMap.size();
        sDeviceIdMap[pRaw->header.hDevice] = keyInput.deviceId;
    }

    return keyInput;
}

MouseInput convert_mouse_input(const void * pKeyInfo)
{
    const RAWINPUT * pRaw = reinterpret_cast<const RAWINPUT*>(pKeyInfo);

    MouseInput mouseInput;

    // We use the same flags as win32. Lucky us... other platforms
    // will need to translate appropriately.
    mouseInput.buttons.buttonFlags = pRaw->data.mouse.usButtonFlags;

    if (pRaw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
        mouseInput.buttons.wheelMovement = static_cast<i16>(pRaw->data.mouse.usButtonData);
    else
        mouseInput.buttons.wheelMovement = 0;

    mouseInput.movement.xDelta = static_cast<i16>(pRaw->data.mouse.lLastX);
    mouseInput.movement.yDelta = static_cast<i16>(pRaw->data.mouse.lLastY);

    return mouseInput;
}


} // namespace gaen
