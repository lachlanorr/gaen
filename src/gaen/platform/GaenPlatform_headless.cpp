//------------------------------------------------------------------------------
// GaenPlatform_headless.cpp - Gaen headless platform
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

#include "gaen/core/logging.h"
#include "gaen/core/gamevars.h"
#include "gaen/core/HashMap.h"
#include "gaen/core/String.h"
#include "gaen/core/platutils.h"
#include "gaen/platform/gaen.h"

#include "gaen/platform/GaenPlatform.h"

namespace gaen
{

void GaenPlatform::update(f32 delta)
{
    // no-op on headless
}

void GaenPlatform::init(int argc, char ** argv)
{
    init_gaen(__argc, __argv);
    start_gaen();
}

void GaenPlatform::fin()
{
    // no-op on headless
}


} // namespace gaen
