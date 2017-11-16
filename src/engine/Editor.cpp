//------------------------------------------------------------------------------
// Editor.cpp - Game editor
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#include "engine/messages/KeyPress.h"
#include "engine/input.h"
#include "engine/InputMgr.h"

#include "engine/Editor.h"

namespace gaen
{

static const ivec4 kKeyGraveAccent = key_vec(kKEY_GraveAccent);

Editor::Editor(bool isActive)
  : mIsActive(isActive)
{
    mTask = Task::create(this, HASH::editor__);
    broadcast_insert_task(mTask.id(), active_thread_id(), mTask);
    InputMgr::register_key_press_listener(HASH::editor__, mTask.id());
}

void Editor::processKeyPress(const ivec4 & keys)
{
    if (keys == kKeyGraveAccent)
    {
        mIsActive = !mIsActive;
        broadcast_message(HASH::editor_activate__, kMessageFlag_Editor, mTask.id(), to_cell(mIsActive));
    }
}

template <typename T>
MessageResult Editor::message(const T& msgAcc)
{
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::key_press:
    {
        messages::KeyPressR<T> msgR(msgAcc);
        processKeyPress(msgR.keys());
        break;
    }
    case HASH::fin:
    {
        InputMgr::deregister_key_press_listener(HASH::editor__, mTask.id());
        break;
    }
    default:
        PANIC("Unknown Editor message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

// Template decls so we can define message func here in the .cpp
template MessageResult Editor::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Editor::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen
