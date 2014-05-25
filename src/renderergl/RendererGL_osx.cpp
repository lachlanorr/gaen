//------------------------------------------------------------------------------
// RendererGL_osx.cpp - OpenGL Renderer
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

#include "renderergl/stdafx.h"

#include "core/base_defines.h"

#include "renderergl/gaen_opengl.h"
#include "renderergl/RendererGL.h"

namespace gaen
{

void RendererGL::initRenderDevice()
{
    ASSERT(mIsInit);

    // LORRTODO - figure out if we need this on osx
}

void RendererGL::endFrame()
{
    ASSERT(mIsInit);

    // LORRTODO - figure out if we need this on osx
}

} // namespace gaen

