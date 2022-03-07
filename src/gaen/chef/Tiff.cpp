//------------------------------------------------------------------------------
// Tiff.cpp - Tiff image processing
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

#include "gaen/chef/stdafx.h"

#include <tiffio.h>

#include "gaen/assets/Gimg.h"

#include "gaen/chef/Tiff.h"

namespace gaen
{

static const char * kPhotoshopHeader = "Adobe Photoshop Document Data Block";

#pragma pack(push, 1)
struct PsTiffLayerInfo
{
    u32 sig8BIM;
    u32 secType;
    u32 length;
    u16 layerCount;
};

struct PsTiffLayerRec
{
    u32 top;
    u32 left;
    u32 bottom;
    u32 right;
    u16 numChannels;
};

struct PsTiffLayerBlend
{
    u32 sig8BIM;
    u32 blendMode;
    u8 opacity;
    u8 clipping;
    u8 flags;
    u8 filler;
    u32 extraDataLen;
};

#pragma pack(pop)

Tiff::Tiff()
  : mpTif(nullptr)
  , mOrigin(kORIG_TopLeft)
  , mWidth(0)
  , mHeight(0)
{}

Tiff::~Tiff()
{
    if (mpTif)
    {
        TIFFClose(mpTif);
        mpTif = nullptr;
    }
}

u8 * Tiff::scanline(u32 idx)
{
    return reinterpret_cast<u8*>(mpPixels.get() + (idx * mWidth));
}

static const u32 k8BIM = FOURCCSW("8BIM");
static const u32 kLayr = FOURCCSW("Layr");
static const u32 knorm = FOURCCSW("norm");

static u8 * proc_layer_record(Tiff::LayerInfo * pLayerInfo, u8 * pData)
{
    PsTiffLayerRec * pRec = reinterpret_cast<PsTiffLayerRec*>(pData);
    pLayerInfo->top = pRec->top;
    pLayerInfo->left = pRec->left;
    pLayerInfo->bottom = pRec->bottom;
    pLayerInfo->right = pRec->right;

    pData += sizeof(PsTiffLayerRec);

    // advance 6 * num channels
    pData += 6 * pRec->numChannels;

    PsTiffLayerBlend * pBlend = reinterpret_cast<PsTiffLayerBlend*>(pData);
    PANIC_IF(pBlend->sig8BIM != k8BIM, "Tiff::proc_layer_record 8BIM signature not present");
    PANIC_IF(pBlend->blendMode != knorm, "Tiff::proc_layer_record norm signature not present");

    pData += sizeof(PsTiffLayerBlend);

    u8 * pRet = pData + pBlend->extraDataLen;

    u32 * pMaskSize = reinterpret_cast<u32*>(pData);

    pData += *pMaskSize + 4;

    u32 * pBlendingRangesSize = reinterpret_cast<u32*>(pData);
    pData += *pBlendingRangesSize + 4;

    pLayerInfo->name = reinterpret_cast<char*>(pData+1); // not sure why the +1 is required based on Adobe spec... but it is

    return pRet;
}

const Vector<kMEM_Chef, Tiff::LayerInfo> & Tiff::layers()
{
    PANIC_IF(mpTif == nullptr, "Tiff::layers called before initialized");

    if (mLayers.size() == 0)
    {
        u32 size;
        u8 * pData;
        int ret = TIFFGetField(mpTif, 37724, &size, &pData);

        if (ret == 1 || !pData)
        {
            const u32 kHeaderLen = (u32)strlen(kPhotoshopHeader)+1;
            // Verify block starts with expected value
            PANIC_IF(size < kHeaderLen, "Tiff::layers photoshop header not present: %s", mPath.c_str());
            PANIC_IF(0 != memcmp(pData, kPhotoshopHeader, kHeaderLen), "Tiff::layers photoshop header not present: %s", mPath.c_str());

            pData += kHeaderLen;

            PsTiffLayerInfo *pLayerInfo = reinterpret_cast<PsTiffLayerInfo*>(pData);


            PANIC_IF(pLayerInfo->sig8BIM != k8BIM, "Tiff::layers 8BIM signature not present");
            PANIC_IF(pLayerInfo->secType != kLayr, "Tiff::layers Layr signature not present");

            pData += sizeof(PsTiffLayerInfo);

            for (u16 i = 0; i < pLayerInfo->layerCount; ++i)
            {
                Tiff::LayerInfo li;
                pData = proc_layer_record(&li, pData);
                mLayers.push_back(li);
            }
        }
        else
        {
            ERR("Failed to read 37724 PhotoShop tag from tiff: %s", mPath.c_str());
        }
    }

    return mLayers;
}

UniquePtrFr<Color> Tiff::extractLayerPixels(const Tiff::LayerInfo & li)
{
    UniquePtrFr<Color> pOut((Color*)GALLOC(kMEM_Chef, sizeof(Color) * li.width() * li.height()));

    Color * pSL = mpPixels.get() + li.top * mWidth + li.left;
    Color * pSLD = pOut.get();
    for (u16 line = li.top; line < li.bottom; ++line)
    {
        memcpy(pSLD, pSL, li.width() * sizeof(Color));

        pSL += mWidth;
        pSLD += li.width();
    }

    return pOut;
}

void Tiff::init(const char * path)
{
    PANIC_IF(mpTif != nullptr, "Tiff::init called after already initialized");

    mPath = path;

    mpTif = TIFFOpen(path, "r");
    PANIC_IF(!mpTif, "Tiff::init, TIFFOPen failed on path: %s", path);

    if (mpTif)
    {
        mOrigin = kORIG_TopLeft;
        TIFFGetField(mpTif, TIFFTAG_IMAGEWIDTH, &mWidth);
        TIFFGetField(mpTif, TIFFTAG_IMAGELENGTH, &mHeight);

        mpPixels.reset((Color*)GALLOC(kMEM_Chef, mWidth * mHeight * sizeof(Color)));

        int ret = TIFFReadRGBAImageOriented(mpTif, mWidth, mHeight, (u32*)mpPixels.get(), ORIENTATION_TOPLEFT, 0);
        PANIC_IF(ret != 1, "Tiff::init, TIFFReadRGBAImage failed to read tif data: %s", path);
    }
}

UniquePtr<Tiff> Tiff::read(const char * path)
{
    UniquePtr<Tiff> pTiff(GNEW(kMEM_Chef, Tiff));

    pTiff->init(path);

    return pTiff;
}

ImageInfo Tiff::read_image_info(const char * path)
{
    Tiff tiff;
    tiff.read(path);
    ImageInfo imgInf(tiff.mOrigin, (u16)tiff.width(), (u16)tiff.height());
    return imgInf;
}

} // namespace gaen

