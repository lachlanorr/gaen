//------------------------------------------------------------------------------
// GaenPlatform.h - Gaen plaform specific app management
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

#ifndef GAEN_PLATFORM_GAENPLATFORM_H
#define GAEN_PLATFORM_GAENPLATFORM_H

#include "gaen/core/base_defines.h"
#include "gaen/engine/Task.h"

#ifndef IS_HEADLESS
#if IS_PLATFORM_WIN32
// A little crufty, but we can accommodate alternate renderers more elegantly later
#include "gaen/renderergl/RendererMesh.h"
typedef gaen::RendererMesh RendererType;
#endif
#endif // IS_HEADLESS

namespace gaen
{

class GaenPlatform
{
public:
    GaenPlatform(int argc, char ** argv);
    ~GaenPlatform();

    void start();

    void update(f32 delta);

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    void init(int argc, char ** argv);
    void fin();

#ifndef IS_HEADLESS
    RendererType mRenderer;
#endif
    void * mpContext;
};

} // namespace gaen

#endif // #ifndef GAEN_PLATFORM_GAENPLATFORM_H
