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

#include "gaen/core/String.h"
#include "gaen/core/Vector.h"

#include "gaen/chef/chef.h"
#include "gaen/chef/Qbt.h"
#include "gaen/chef/Png.h"

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

typedef HashMap<kMEM_Chef, ChefString, const QbtNode*> NodesMap;

static void get_matrices(NodesMap & nodes, const QbtNode & node)
{
    if (node.typeId == kQBNT_Matrix)
    {
        nodes[node.name] = &node;
    }
    else
    {
        for (u32 i = 0; i < node.children.size(); ++i)
        {
            get_matrices(nodes, *node.children[i]);
        }
    }
}

static NodesMap get_compound_matrices(const Qbt & qbt, const char * topLevelCompound)
{
    NodesMap nodes;

    if (qbt.pRoot)
    {
        for (u32 i = 0; i < qbt.pRoot->children.size(); ++i)
        {
            if (qbt.pRoot->children[i]->name == topLevelCompound && qbt.pRoot->children[i]->typeId == kQBNT_Compound)
            {
                get_matrices(nodes, *qbt.pRoot->children[i]);
            }
        }
    }

    return nodes;
}

static void build_diffuse(Gimg* pGimg, const QbtNode & matrix)
{
    PANIC_IF(matrix.typeId != kQBNT_Matrix, "Not a matrix: '%s', %d", matrix.name.c_str(), matrix.typeId);

    pGimg->clear(Color(0,0,0,0));

    u32 centerXR = matrix.size.x / 2;
    u32 centerXL = centerXR - 1;

    u32 backHalf = matrix.size.z / 2;

    bool yShouldAdvance = true;
    Color *pScanLine, *pScanLeft, *pScanRight;
    u32 currLine = 0;
    for (i32 ymat = matrix.size.y-1; ymat >= 0; --ymat)
    {
        if (yShouldAdvance) // if the last time through we found a non-null voxel
        {
            pScanLine = (Color*)pGimg->scanline(currLine++);
            pScanLeft = pScanLine + pGimg->width() / 2;
            pScanRight = pScanLeft + 1;
            yShouldAdvance = false;
        }

        for (i32 zmat = matrix.size.z-1; zmat > backHalf; --zmat)
        {
            // do left side
            for (i32 xmat = centerXL; xmat >= 0; --xmat)
            {
                const Color & col = matrix.voxel(xmat, ymat, zmat);
                if (col.a() > 1) // a == 1 means a core voxel
                {
                    yShouldAdvance = true;
                    *pScanLeft = col;
                    pScanLeft->seta(255);
                    pScanLeft--;
                }
            }

            // do right side
            for (i32 xmat = centerXR; xmat < matrix.size.x; ++xmat)
            {
                const Color & col = matrix.voxel(xmat, ymat, zmat);
                if (col.a() > 1) // a == 1 means a core voxel
                {
                    yShouldAdvance = true;
                    *pScanRight = col;
                    pScanRight->seta(255);
                    pScanRight++;
                }
            }
        }
    }
}

// NOTE:
// Texture map for center of pixel
//u = x / width + 0.5 / width;
//v = y / height + 0.5 / height;

void Qubicle::cook(CookInfo * pCookInfo) const
{
    QbtUP pQbTree = Qbt::load_from_file(pCookInfo->rawPath().c_str());

    NodesMap baseMatrices = get_compound_matrices(*pQbTree, "Base");

    static const u32 kImgDim = 256;
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_RGBA8, kImgDim, kImgDim, Image::reference_path_hash(pCookInfo));

    build_diffuse(pGimg.get(), *baseMatrices["Chest"]);
/*
    // Create an image for our diffuse map
    u32 glyphsPerRow = (u32)(sqrt((f32)utf32Codes.size()) + 0.5f);
    u32 imgHeight = next_power_of_two(glyphsPerRow * glyphHeight);
    Scoped_GFREE<Gimg> pGimg = Gimg::create(kPXL_R8, imgHeight, imgHeight, Image::reference_path_hash(pCookInfo));
*/
    // Make any directories needed to write transitory .png and .atl
    ChefString objTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtObj);
    ChefString pngTransPath = pCookInfo->chef().getRawTransPath(pCookInfo->rawPath(), kExtPng);

    ChefString transDir = parent_dir((const ChefString&)pngTransPath);
    make_dirs(transDir.c_str());

    Png::write_gimg(pngTransPath.c_str(), pGimg.get(), false);

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
    // Write a AssetHeader only g_qb file so we can track Font cooker version and force recooks.
    AssetHeader * pGqbtHeader = GNEW(kMEM_Chef, AssetHeader, FOURCC(kExtGqbt), sizeof(AssetHeader));
    pCookInfo->setCookedBuffer(pGqbtHeader);
}

} // namespace cookers
} // namespace gaen
