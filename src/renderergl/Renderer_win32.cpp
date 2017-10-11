//------------------------------------------------------------------------------
// Renderer_win32.cpp - OpenGL Renderer
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

#include "core/base_defines.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/Renderer.h"

namespace gaen
{

void RENDERER_TYPE::initRenderDevice()
{
    ASSERT(mIsInit);

    glfwMakeContextCurrent((GLFWwindow*)mpRenderDevice);
    glfwSwapInterval(1);

    // Prepare our GL function pointers.
    // We have to wait until here to do this since if you call it too
    // early, the GL driver dll hasn't been loaded and
    // wglGetProcAddress will return NULL for all functions.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapBuffers((GLFWwindow*)mpRenderDevice);
}

void RENDERER_TYPE::endFrame()
{
    ASSERT(mIsInit);

    static bool sIsVisible = false;

    glfwSwapBuffers((GLFWwindow*)mpRenderDevice);

    // Show the window which starts as not visible.
    // Starting with a visible window gives a nasty white window on
    // screen before our renderer initializes.
    if (!sIsVisible)
    {
        glfwShowWindow((GLFWwindow*)mpRenderDevice);
        sIsVisible = true;
    }
}

} // namespace gaen

