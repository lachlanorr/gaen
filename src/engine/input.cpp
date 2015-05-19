//------------------------------------------------------------------------------
// input.cpp - Low level input handling
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#include "engine/messages/MouseMove.h"
#include "engine/MessageWriter.h"

#include "engine/input.h"


namespace gaen
{

void process_key_input(const void * pKeyInfo)
{
    KeyInput keyInput = convert_key_input(pKeyInfo);

    MessageQueueWriter msgw(HASH::keyboard_input,
                            kMessageFlag_None,
                            kMainThreadTaskId,
                            kInputMgrTaskId,
                            to_cell(keyInput),
                            0);
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
            keyInput.keyEvent = kKEVT_Down;
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
            keyInput.keyEvent = kKEVT_Up;
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
            keyInput.keyEvent = kKEVT_Down;
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
            keyInput.keyEvent = kKEVT_Up;
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
            keyInput.keyEvent = kKEVT_Down;
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
            keyInput.keyEvent = kKEVT_Up;
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
            keyInput.keyEvent = kKEVT_Down;
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
            keyInput.keyEvent = kKEVT_Up;
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
            keyInput.keyEvent = kKEVT_Down;
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
            keyInput.keyEvent = kKEVT_Up;
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
