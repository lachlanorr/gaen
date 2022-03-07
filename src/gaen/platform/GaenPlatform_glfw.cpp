//------------------------------------------------------------------------------
// GaenPlatform_glfw.cpp - Gaen desktop glfw based app management
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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gaen/core/logging.h"
#include "gaen/core/gamevars.h"
#include "gaen/core/HashMap.h"
#include "gaen/core/String.h"
#include "gaen/core/platutils.h"
#include "gaen/engine/input.h"
#include "gaen/render_support/renderer_api.h"
#include "gaen/renderergl/Renderer.h"
#include "gaen/renderergl/gaen_opengl.h"
#include "gaen/platform/gaen.h"

#include "gaen/platform/GaenPlatform.h"

namespace gaen
{

GAMEVAR_DECL_BOOL(gldebug, false);

void glfw_error_callback(int error, const char * description)
{
    ERR("GLFW ERROR: %d - %s", error, description);
}

void glfw_focus_callback(GLFWwindow * pWindow, int focused)
{
    if (focused == GLFW_FALSE)
       gaen::kill_focus();
}

// Some assertions to make sure our constants line up with glfw's
static_assert(GLFW_RELEASE == gaen::kKACT_Release, "GLFW_RELEASE != gaen::kKACT_Release");
static_assert(GLFW_PRESS   == gaen::kKACT_Press,   "GLFW_PRESS   != gaen::kKACT_Press");
static_assert(GLFW_REPEAT  == gaen::kKACT_Repeat,  "GLFW_REPEAT  != gaen::kKACT_Repeat");

static_assert(GLFW_MOD_SHIFT   == gaen::kKMOD_Shift,   "GLFW_MOD_SHIFT   != gaen::kKMOD_Shift");
static_assert(GLFW_MOD_CONTROL == gaen::kKMOD_Control, "GLFW_MOD_CONTROL != gaen::kKMOD_Control");
static_assert(GLFW_MOD_ALT     == gaen::kKMOD_Alt,     "GLFW_MOD_ALT     != gaen::kKMOD_Alt");
static_assert(GLFW_MOD_SUPER   == gaen::kKMOD_Super,   "GLFW_MOD_SUPER   != gaen::kKMOD_Super");

void glfw_key_callback(GLFWwindow * pWindow, int key, int scancode, int action, int mods)
{
    gaen::KeyInput keyInput;
    keyInput.key = gaen::convert_glfw_key(key);
    keyInput.action = action;
    keyInput.mods = mods;
    keyInput.deviceId = 0;
    process_key_input(keyInput);
}

static int sConnectedJoysticks[GLFW_JOYSTICK_LAST];

void glfw_joystick_callback(int joy, int event)
{
    if (joy < GLFW_JOYSTICK_LAST)
        sConnectedJoysticks[joy] = event == GLFW_CONNECTED;
}

void GaenPlatform::update(f32 delta)
{
    static bool isFin = false;
    if (glfwWindowShouldClose((GLFWwindow*)mpContext))
    {
        if (!isFin)
        {
            fin_gaen();
            isFin = true;
        }
    }
    else
    {
        glfwPollEvents();
    }
}

void GaenPlatform::init(int argc, char ** argv)
{
    init_gaen(__argc, __argv);

    PANIC_IF(!glfwInit(), "glfwInit() failed");

    glfwSetErrorCallback(glfw_error_callback);

    const u32 kScreenWidth  = 1920;
    const u32 kScreenHeight = 1080;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Starting with a visible window gives a nasty white window on
    // screen before our renderer initializes. Will get shown by
    // renderer on first endFrame call.
    glfwWindowHint(GLFW_VISIBLE, 0);

    if (gldebug)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    mpContext = glfwCreateWindow(kScreenWidth, kScreenHeight, "Gaen", NULL, NULL);
    PANIC_IF(!mpContext, "glfwCreateWindow failed");

    start_gaen();

    glfwSetWindowFocusCallback((GLFWwindow*)mpContext, glfw_focus_callback);
    glfwSetKeyCallback((GLFWwindow*)mpContext, glfw_key_callback);

    glfwSetJoystickCallback(glfw_joystick_callback);

    mRenderer.init((GLFWwindow*)mpContext, kScreenWidth, kScreenHeight);
    Task rendererTask = Task::create(&mRenderer, HASH::renderer);
    set_renderer(rendererTask);

    // NOTE: From this point forward, methods on sRenderer should not
    // be called directly.  Interaction with the renderer should only
    // be made with messages sent to the primary TaskMaster.

    for (u32 i = 0; i < GLFW_JOYSTICK_LAST; ++i)
    {
        sConnectedJoysticks[i] = glfwJoystickPresent(i);
    }
}

void GaenPlatform::fin()
{
    glfwTerminate();
}


} // namespace gaen
