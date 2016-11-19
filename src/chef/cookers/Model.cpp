//------------------------------------------------------------------------------
// Model.cpp - Model cooker
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

#include "chef/stdafx.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gmdl.h"

#include "chef/cooker_utils.h"
#include "chef/CookInfo.h"
#include "chef/cookers/Model.h"

namespace gaen
{
namespace cookers
{

Model::Model()
{
    setVersion(1);
    addRawExt(kExtObj);
    addCookedExtExclusive(kExtGmdl);
}

void Model::cook(CookInfo * pCookInfo) const
{
    const char * ext = get_ext(pCookInfo->rawPath().c_str());

    if (0 == strcmp(ext, "png"))
    {
        cookPng(pCookInfo);
    }
    else if (0 == strcmp(ext, "tga"))
    {
        cookTga(pCookInfo);
    }
    else
    {
        PANIC("Unable to cook model: %s", pCookInfo->rawPath().c_str());
    }
}

void Model::cookObj(CookInfo * pCookInfo) const
{
//    UniquePtr<Png> pPng = Png::read(pCookInfo->rawPath().c_str());
//    PANIC_IF(!pPng, "Failed to read png: %s", pCookInfo->rawPath().c_str());

    // Convert to a Gimg with same-ish pixel format
    //Gmdl * pGmdlObj;


//    ASSERT(pGimg);
//    pCookInfo->setCookedBuffer(pGimg);
}

}
} // namespace gaen


