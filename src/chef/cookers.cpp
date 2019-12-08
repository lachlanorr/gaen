//------------------------------------------------------------------------------
// cookers.cpp - Cookers for various asset types
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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

#include "chef/stdafx.h"

#include "core/base_defines.h"
#include "core/thread_local.h"
#include "core/hashing.h"

#include "assets/file_utils.h"
#include "assets/Gimg.h"
#include "assets/Gatl.h"
#include "assets/Gspr.h"

#include "chef/Chef.h"
#include "chef/CookerRegistry.h"
#include "chef/Tga.h"
#include "chef/cooker_utils.h"
#include "chef/cookers.h"

#include "chef/cookers/AnimImage.h"
#include "chef/cookers/Atlas.h"
#include "chef/cookers/Font.h"
#include "chef/cookers/Image.h"
#include "chef/cookers/Model.h"
#include "chef/cookers/Sprite.h"

namespace gaen
{

void register_cookers()
{
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::AnimImage)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Atlas)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Font)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Image)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Model)));
    CookerRegistry::register_cooker(UniquePtr<Cooker>(GNEW(kMEM_Chef, cookers::Sprite)));

    register_project_cookers();
}

#ifdef IS_GAEN_PROJECT
void register_project_cookers()
{
    // If we're compiling gaen as a project, no custom asset cookers.
}
#endif

} // namespace gaen
