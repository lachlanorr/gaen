//------------------------------------------------------------------------------
// RendererGLAdapter.h - Adapts RendererGL into a task that handles messages
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

#ifndef GAEN_ENGINE_RENDERERGL_ADAPTER_H
#define GAEN_ENGINE_RENDERERGL_ADAPTER_H

#include "core/base_defines.h"
#include "renderergl/RendererGL.h"

#include "engine/MessageQueue.h"


namespace gaen
{

// In order to decouple renderer details from the engine, this class
// wraps a RendererGL instance and acts as a Task from the perspective
// of the engine.
// To replace the renderer with a custom renderer, you'll need to
// create a class such as this that offers an 'update' method (which
// typically calls through to a 'render' method on the renderer), and
// a 'message' method that translates the standard renderer messages
// into method calls on the renderer.
class RendererGLAdapter
{
public:
    RendererGLAdapter(RendererGL * pRendererGL)
        : mpRendererGL(pRendererGL) {}

    void update(f32 deltaSecs)
    {
        ASSERT(mpRendererGL);
        mpRendererGL->render();
    }

    MessageResult message(const MessageQueue::MessageAccessor& msgAcc);

private:
    RendererGL * mpRendererGL;

};


} // namespace gaen


#endif // #ifndef GAEN_ENGINE_RENDERERGL_ADAPTER_H
