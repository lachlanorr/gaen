//------------------------------------------------------------------------------
// input.h - Low level input handling
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#include "gaen/core/base_defines.h"
#include "gaen/core/logging.h"

#include "gaen/engine/Block.h"

namespace gaen
{

enum Key
{
    kKEY_Space          = 0,
    kKEY_Apostrophe     = 1,
    kKEY_Comma          = 2,
    kKEY_Minus          = 3,
    kKEY_Period         = 4,
    kKEY_Slash          = 5,
    kKEY_0              = 6,
    kKEY_1              = 7,
    kKEY_2              = 8,
    kKEY_3              = 9,
    kKEY_4              = 10,
    kKEY_5              = 11,
    kKEY_6              = 12,
    kKEY_7              = 13,
    kKEY_8              = 14,
    kKEY_9              = 15,
    kKEY_Semicolon      = 16,
    kKEY_Equal          = 17,
    kKEY_A              = 18,
    kKEY_B              = 19,
    kKEY_C              = 20,
    kKEY_D              = 21,
    kKEY_E              = 22,
    kKEY_F              = 23,
    kKEY_G              = 24,
    kKEY_H              = 25,
    kKEY_I              = 26,
    kKEY_J              = 27,
    kKEY_K              = 28,
    kKEY_L              = 29,
    kKEY_M              = 30,
    kKEY_N              = 31,
    kKEY_O              = 32,
    kKEY_P              = 33,
    kKEY_Q              = 34,
    kKEY_R              = 35,
    kKEY_S              = 36,
    kKEY_T              = 37,
    kKEY_U              = 38,
    kKEY_V              = 39,
    kKEY_W              = 40,
    kKEY_X              = 41,
    kKEY_Y              = 42,
    kKEY_Z              = 43,
    kKEY_LeftBracket    = 44,
    kKEY_Backslash      = 45,
    kKEY_RightBracket   = 46,
    kKEY_GraveAccent    = 47,
    kKEY_World1         = 48,
    kKEY_World2         = 49,
    kKEY_Escape         = 50,
    kKEY_Enter          = 51,
    kKEY_Tab            = 52,
    kKEY_Backspace      = 53,
    kKEY_Insert         = 54,
    kKEY_Delete         = 55,
    kKEY_Right          = 56,
    kKEY_Left           = 57,
    kKEY_Down           = 58,
    kKEY_Up             = 59,
    kKEY_PageUp         = 60,
    kKEY_PageDown       = 61,
    kKEY_Home           = 62,
    kKEY_End            = 63,
    kKEY_CapsLock       = 64,
    kKEY_ScrollLock     = 65,
    kKEY_NumLock        = 66,
    kKEY_PrintScreen    = 67,
    kKEY_Pause          = 68,
    kKEY_F1             = 69,
    kKEY_F2             = 70,
    kKEY_F3             = 71,
    kKEY_F4             = 72,
    kKEY_F5             = 73,
    kKEY_F6             = 74,
    kKEY_F7             = 75,
    kKEY_F8             = 76,
    kKEY_F9             = 77,
    kKEY_F10            = 78,
    kKEY_F11            = 79,
    kKEY_F12            = 80,
    kKEY_F13            = 81,
    kKEY_F14            = 82,
    kKEY_F15            = 83,
    kKEY_F16            = 84,
    kKEY_F17            = 85,
    kKEY_F18            = 86,
    kKEY_F19            = 87,
    kKEY_F20            = 88,
    kKEY_F21            = 89,
    kKEY_F22            = 90,
    kKEY_F23            = 91,
    kKEY_F24            = 92,
    kKEY_F25            = 93,
    kKEY_NumPad0        = 94,
    kKEY_NumPad1        = 95,
    kKEY_NumPad2        = 96,
    kKEY_NumPad3        = 97,
    kKEY_NumPad4        = 98,
    kKEY_NumPad5        = 99,
    kKEY_NumPad6        = 100,
    kKEY_NumPad7        = 101,
    kKEY_NumPad8        = 102,
    kKEY_NumPad9        = 103,
    kKEY_NumPadDecimal  = 104,
    kKEY_NumPadDivide   = 105,
    kKEY_NumPadMultiply = 106,
    kKEY_NumPadSubtract = 107,
    kKEY_NumPadAdd      = 108,
    kKEY_NumPadEnter    = 109,
    kKEY_NumPadEqual    = 110,
    kKEY_LeftShift      = 111,
    kKEY_LeftControl    = 112,
    kKEY_LeftAlt        = 113,
    kKEY_RightShift     = 114,
    kKEY_RightControl   = 115,
    kKEY_RightAlt       = 116,
    kKEY_Menu           = 117,

    kKEY_Mouse1         = 118,
    kKEY_Mouse2         = 119,
    kKEY_Mouse3         = 120,
    kKEY_Mouse4         = 121,
    kKEY_Mouse5         = 122,
    kKEY_Mouse6         = 123,
    kKEY_Mouse7         = 124,
    kKEY_Mouse8         = 125,

    kKEY_NONE           = 126 // Indicative of no key pressed, used when specifying no key in a 4 key combo
};

enum KeyAction
{
    kKACT_Release = 0,
    kKACT_Press   = 1,
    kKACT_Repeat  = 2
};

enum KeyMod
{
    kKMOD_Shift   = 1,
    kKMOD_Control = 2,
    kKMOD_Alt     = 4,
    kKMOD_Super   = 8
};

struct KeyInput
{
    u32 key:7;
    u32 action:2;
    u32 mods:4;
    u32 deviceId:4; // keep this to 4 so we can pack taskid + deviceid into 32 bits (LORRNOTE 2016-09-18: ???, don't remember rational for previous comment)
    u32 PADDING__:15;

    KeyInput()
    {
        *reinterpret_cast<u32*>(this) = 0;
    }

    KeyInput(cell c)
    {
        *reinterpret_cast<u32*>(this) = c.u;
    }
};
static_assert(sizeof(KeyInput) == sizeof(u32), "KeyInput unexpected size");

enum PadCode
{
    kPAD_NONE       = 0x00000,

    kPAD_A          = 0x00001,
    kPAD_B          = 0x00002,
    kPAD_X          = 0x00004,
    kPAD_Y          = 0x00008,

    kPAD_LBumper    = 0x00010,
    kPAD_RBumper    = 0x00020,

    kPAD_Back       = 0x00040,
    kPAD_Start      = 0x00080,
    kPAD_Guide      = 0x00100,

    kPAD_LThumb     = 0x00200,
    kPAD_RThumb     = 0x00400,

    kPAD_DUp        = 0x00800,
    kPAD_DRight     = 0x01000,
    kPAD_DDown      = 0x02000,
    kPAD_DLeft      = 0x04000,

    kPAD_LStick     = 0x08000,
    kPAD_RStick     = 0x10000,
    kPAD_LTrigger   = 0x20000,
    kPAD_RTrigger   = 0x40000,
};
static const u32 kPadCodeAnalogStart = kPAD_LStick;

struct PadInput
{
    u32 padId;
    u32 codes;
    vec2 lstick;
    vec2 rstick;
    f32 ltrigger;
    f32 rtrigger;

    PadInput(u32 padId = 0)
      : padId(padId)
      , codes(0)
      , lstick(0.0f)
      , rstick(0.0f)
      , ltrigger(0.0f)
      , rtrigger(0.0f)
    {}

    bool operator==(const PadInput & rhs)
    {
        return padId == rhs.padId &&
               codes == rhs.codes &&
               lstick == rhs.lstick &&
               rstick == rhs.rstick &&
               ltrigger == rhs.ltrigger &&
               rtrigger == rhs.rtrigger;
    }

    bool operator!=(const PadInput & rhs)
    {
        return !(*this == rhs);
    }
};

inline cell to_cell(KeyInput val)
{
    static_assert(sizeof(KeyInput) == 4, "KeyInput must be 4 bytes to be compatible with cell type");
    cell c;
    c.u = *reinterpret_cast<const u32*>(&val);
    return c;
}

// Convert string to KeyCode, useful when parsing config files
Key lookup_key_code(const char * str);
PadCode lookup_pad_button(const char * str);

inline u32 key_vec_idx(Key key) { return key / 32; }
inline u32 key_vec_mask(Key key) { return 1 << (key % 32); }

inline void set_key_vec_bit(ivec4 & vec, Key key) { vec[key_vec_idx(key)] |= key_vec_mask(key); }
inline void unset_key_vec_bit(ivec4 & vec, Key key) { vec[key_vec_idx(key)] &= ~key_vec_mask(key); }
inline ivec4 key_vec(Key key) { ivec4 vec(0); set_key_vec_bit(vec, key); return vec; }
inline ivec4 key_vec(Key key0, Key key1) { ivec4 vec = key_vec(key0); set_key_vec_bit(vec, key1); return vec; }
inline ivec4 key_vec(Key key0, Key key1, Key key2) { ivec4 vec = key_vec(key0, key1); set_key_vec_bit(vec, key2); return vec; }
inline ivec4 key_vec(Key key0, Key key1, Key key2, Key key3) { ivec4 vec = key_vec(key0, key1, key2); set_key_vec_bit(vec, key3); return vec; }

// KeyInput is 4 bytes to simplify passing it around as a message payload.
// We have 16 unused bits, maybe find something interesting for them
// in the future.

// void* to accomodate different platforms structs/values for key code
// information.

void process_key_input(KeyInput keyInput);
Key convert_glfw_key(int glfwKey);
void kill_focus();

PadCode convert_glfw_pad(int glfwGamepadButton);
void poll_pad_input();

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
//MouseInput convert_mouse_input(const void * pKeyInfo);

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_KEYCODES_H
