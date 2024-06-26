//------------------------------------------------------------------------------
// cookers.cpp - Cookers for various asset types
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

#include "gaen/core/base_defines.h"
#include "gaen/core/thread_local.h"
#include "gaen/core/hashing.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Gimg.h"
#include "gaen/assets/Gatl.h"
#include "gaen/assets/Gspr.h"

#include "gaen/cheflib/Chef.h"
#include "gaen/cheflib/CookerRegistry.h"
#include "gaen/cheflib/cookers.h"

#include "gaen/cheflib/cookers/AnimImage.h"
#include "gaen/cheflib/cookers/Atlas.h"
#include "gaen/cheflib/cookers/Audio.h"
#include "gaen/cheflib/cookers/Font.h"
#include "gaen/cheflib/cookers/Image.h"
#include "gaen/cheflib/cookers/Model.h"
#include "gaen/cheflib/cookers/Sprite.h"
#include "gaen/cheflib/cookers/Qubicle.h"

namespace gaen
{

void register_cookers()
{
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::AnimImage)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Atlas)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Audio)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Font)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Image)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Model)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Sprite)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Qubicle)));

    register_project_cookers();
}

#ifdef IS_GAEN_PROJECT
void register_project_cookers()
{
    // If we're compiling gaen as a project, no custom asset cookers.
}
#endif

} // namespace gaen
