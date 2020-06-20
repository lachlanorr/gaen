//------------------------------------------------------------------------------
// Editor.cpp - Game editor
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

#include "core/base_defines.h"
#if HAS(ENABLE_EDITOR)
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "core/logging.h"

#include "engine/messages/KeyPress.h"
#include "engine/input.h"
#include "engine/InputMgr.h"

#include "engine/Editor.h"

namespace gaen
{

static const ivec4 kKeyGraveAccent = key_vec(kKEY_GraveAccent);

Editor::Editor()
  : mIsActive(false)
{
    InputMgr::register_key_press_listener(HASH::editor__, kEditorTaskId);
}

void Editor::processKeyPress(const ivec4 & keys)
{
    if (keys == kKeyGraveAccent)
    {
        mIsActive = !mIsActive;
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
        InputMgr::deregister_key_press_listener(HASH::editor__, kEditorTaskId);
        break;
    }
    default:
        PANIC("Unknown Editor message: %d", msg.msgId);
    }

    return MessageResult::Consumed;
}

void Editor::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (mIsActive)
    {
        bool showDemoWindow = true;
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::Render();
}

// Template decls so we can define message func here in the .cpp
template MessageResult Editor::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Editor::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

#endif // #if HAS(ENABLE_EDITOR)
