//------------------------------------------------------------------------------
// Editor.cpp - Game editor
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

#include "gaen/engine/stdafx.h"

#include "gaen/core/base_defines.h"
#if HAS(ENABLE_EDITOR)
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "gaen/core/logging.h"
#include "gaen/core/gamevars.h"

#include "gaen/engine/messages/KeyPress.h"
#include "gaen/engine/input.h"
#include "gaen/engine/InputMgr.h"

#include "gaen/engine/Editor.h"

namespace gaen
{

GAMEVAR_REF_BOOL(collision_debug);

static const ivec4 kTogglePause = key_vec(kKEY_Space);
static const ivec4 kToggleEditor = key_vec(kKEY_GraveAccent);

Editor::Editor()
  : mIsActive(false)
  , mIsPaused(false)
{
    mCollisionDebug = collision_debug;
    InputMgr::register_key_press_listener(HASH::editor__, kEditorTaskId);
}

void Editor::processKeyPress(const ivec4 & keys)
{
    if (mIsActive && keys == kTogglePause)
    {
        mIsPaused = !mIsPaused;
        broadcast_message(HASH::toggle_pause__, kMessageFlag_None, kEditorTaskId);
    }
    else if (keys == kToggleEditor)
    {
        mIsActive = !mIsActive;
        if (!mIsActive)
            unpause();
    }
}

void Editor::unpause()
{
    if (mIsPaused)
    {
        mIsPaused = false;
        broadcast_message(HASH::toggle_pause__, kMessageFlag_None, kEditorTaskId);
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

void Editor::update(const FrameTime & frameTime)
{
    static f32 fps = frameTime.fpsLast10();
    static TickCount lastFpsTime = now_ticks();
    TickCount nowFpsTime = now_ticks();
    if (ticks_to_secs(nowFpsTime - lastFpsTime) > 1.0)
    {
        fps = frameTime.fpsLast10();
        lastFpsTime = nowFpsTime;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (mIsActive)
    {
        ImGui::Begin("Editor", nullptr);
        ImGui::Text("FPS: %0.2f", fps);
        ImGui::Text("Game State: %s", mIsPaused ? "PAUSED" : "RUNNING");
        ImGui::Checkbox("Collision Debug", &mCollisionDebug);
        ImGui::End();
    }

    ImGui::Render();

    collision_debug = mCollisionDebug;
}

// Template decls so we can define message func here in the .cpp
template MessageResult Editor::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult Editor::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);

} // namespace gaen

#endif // #if HAS(ENABLE_EDITOR)
