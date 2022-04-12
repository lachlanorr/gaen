//------------------------------------------------------------------------------
// Qubicle.cpp - Qubicle binary file cooker
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

#include <array>

#include "gaen/core/String.h"
#include "gaen/core/Vector.h"
#include "gaen/core/Set.h"

#include "gaen/voxel/Qbt.h"

#include "gaen/chef/chef.h"
#include "gaen/voxel/VoxObj.h"

#include "gaen/chef/cookers/Model.h"
#include "gaen/chef/cookers/Image.h"
#include "gaen/chef/cookers/Qubicle.h"

namespace gaen
{
namespace cookers
{

Qubicle::Qubicle()
{
    setVersion(1);
    addRawExt(kExtQbt);
    addCookedExt(kExtGmdl);
    addCookedExt(kExtGimg);
    addCookedExtExclusive(kExtGqbt);
}

void Qubicle::cook(CookInfo * pCookInfo) const
{
    const auto pQbTree = Qbt::load_from_file(pCookInfo->rawPath().c_str());

    const auto voxObjVec = build_voxobjs_from_qbt(pQbTree);
    //build_diffuse_front_half(pGimg.get(), *baseMatrices["Head"]);
/*
    // Create an image for our diffuse map
    u32 glyphsPerRow = (u32)(sqrt((f32)utf32Codes.size()) + 0.5f);
    u32 imgHeight = next_power_of_two(glyphsPerRow * glyphHeight);
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_R8, imgHeight, imgHeight, Image::reference_path_hash(pCookInfo));
*/
    // Make any directories needed to write transitory .png and .atl


/*--
    ChefString objTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtObj);
    ChefString pngTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtPng);

    ChefString transDir = parent_dir((const ChefString&)pngTransPath);
    make_dirs(transDir.c_str());

    if (vobj.baseMatrices.size() > 0)
        vobj.exportFiles(objTransPath, 0.0125);
--*/

    //Png::write_gimg(pngTransPath.c_str(), vobj.pGimgDiffuse.get(), false);

/*

    // Write .png transitory output file
    Png::write_gimg(pngTransPath.c_str(), pGimg.get());
    UniquePtr<CookInfo> pCiPng = pCookInfo->chef().forceCook(pngTransPath);
    pCookInfo->transferCookResult(*pCiPng, kExtGimg);

    // Write .atl transitory output file
    fontAtl.write(atlTransPath.c_str());
    UniquePtr<CookInfo> pCiAtl = pCookInfo->chef().forceCook(atlTransPath);
    pCookInfo->transferCookResult(*pCiAtl, kExtGatl);
*/
    // Write a AssetHeader only g_qb file so we can track Qubicle cooker version and force recooks.
    AssetHeader * pGqbtHeader = GNEW(kMEM_Chef, AssetHeader, FOURCC(kExtGqbt), sizeof(AssetHeader));
    pCookInfo->setCookedBuffer(pGqbtHeader);
}

} // namespace cookers
} // namespace gaen
