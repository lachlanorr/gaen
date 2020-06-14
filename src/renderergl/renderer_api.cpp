//------------------------------------------------------------------------------
// renderer_api.cpp - OpenGL renderer versions of renderer_api.h functions
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

#include "render_support/renderer_api.h"

#include "renderergl/Renderer.h"

namespace gaen
{

void renderer_fin(Task & rendererTask)
{
    RendererType * pRenderer = reinterpret_cast<RendererType*>(rendererTask.that());
    pRenderer->fin();
}

void renderer_init_device(Task & rendererTask)
{
    RendererType * pRenderer = reinterpret_cast<RendererType*>(rendererTask.that());
    pRenderer->initRenderDevice();
}

void renderer_init_viewport(Task & rendererTask)
{
    RendererType * pRenderer = reinterpret_cast<RendererType*>(rendererTask.that());
    pRenderer->initViewport();
}

void renderer_render(Task & rendererTask)
{
    RendererType * pRenderer = reinterpret_cast<RendererType*>(rendererTask.that());
    pRenderer->render();
}

void renderer_end_frame(Task & rendererTask)
{
    RendererType * pRenderer = reinterpret_cast<RendererType*>(rendererTask.that());
    pRenderer->endFrame();

    // Check for errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        LOG_ERROR("glGetError returned %d, 0x%x", err, err);
    }
}

} // namespace gaen

