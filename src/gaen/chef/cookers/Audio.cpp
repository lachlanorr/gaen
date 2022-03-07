//------------------------------------------------------------------------------
// Audio.cpp - Audio cooker
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

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Gaud.h"

#include "gaen/chef/CookInfo.h"
#include "gaen/chef/cookers/Audio.h"

namespace gaen
{
namespace cookers
{

Audio::Audio()
{
    setVersion(1);
    addRawExt(kExtWav);
    addCookedExtExclusive(kExtGaud);
}

void Audio::cook(CookInfo * pCookInfo) const
{
    const char * ext = get_ext(pCookInfo->rawPath().c_str());

    if (0 == strcmp(ext, kExtWav))
    {
        cookWav(pCookInfo);
    }
    else
    {
        PANIC("Unable to cook audio: %s", pCookInfo->rawPath().c_str());
    }
}

#pragma pack(push, 1)
struct RiffDesc
{
    u32 RIFF4cc;
    u32 chunkSize;
    u32 WAVE4cc;
};

struct SubChunkDesc
{
    u32 sc4cc;
    u32 scSize;
};

struct WavFormat
{
    u16 audioFormat;     // Should be 1 for PCM, 3 for IEEE Float
    u16 numChannels;     // Number of channels 1=Mono 2=Sterio
    u32 sampleRate;      // Sampling Frequency in Hz
    u32 byteRate;        // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    u16 sampleAlignment; // numChannels * Bytes Per Sample
    u16 bitDepth;        // Number of bits per sample
};
#pragma pack(pop)

void Audio::cookWav(CookInfo * pCookInfo) const
{
    FileReader rdr(pCookInfo->rawPath().c_str());
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());

    RiffDesc riffDesc;
    rdr.read(&riffDesc);
    PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(RiffDesc), "Invalid RiffDesc: %s", pCookInfo->rawPath().c_str());
    PANIC_IF(riffDesc.RIFF4cc != FOURCC("RIFF"), "Bad RIFF 4cc: %s", pCookInfo->rawPath().c_str());
    PANIC_IF(riffDesc.WAVE4cc != FOURCC("WAVE"), "Bad RIFF 4cc: %s", pCookInfo->rawPath().c_str());

    WavFormat fmt;
    Vector<kMEM_Chef, u8> data;

    // skip ahead until we find the fmt chunk
    while (1)
    {
        SubChunkDesc scdesc;
        rdr.read(&scdesc);
        PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(SubChunkDesc), "Invalid SubChunkDesc: %s", pCookInfo->rawPath().c_str());
        if (scdesc.sc4cc == FOURCC("fmt "))
        {
            PANIC_IF(scdesc.scSize < sizeof(WavFormat), "Invalid SubChunk size for WaveFormat: %s", pCookInfo->rawPath().c_str());

            rdr.read(&fmt);
            PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(WavFormat), "Invalid WavFormat: %s", pCookInfo->rawPath().c_str());
            if (scdesc.scSize > sizeof(WavFormat))
                rdr.advance(scdesc.scSize - sizeof(WavFormat));
            break;
        }
        else
        {
            rdr.advance(scdesc.scSize);
        }
    }

    while (1)
    {
        SubChunkDesc scdesc;
        rdr.read(&scdesc);
        PANIC_IF(!rdr.ifs.good() || rdr.ifs.gcount() != sizeof(SubChunkDesc), "Invalid SubChunkDesc: %s", pCookInfo->rawPath().c_str());
        if (scdesc.sc4cc == FOURCC("data"))
        {
            data.resize(scdesc.scSize);
            rdr.read(data.data(), scdesc.scSize);
            break;
        }
        else
        {
            rdr.advance(scdesc.scSize);
        }
    }

    PANIC_IF(fmt.numChannels != 1 && fmt.numChannels != 2, "Invalid numChannels(%d): %s", fmt.numChannels, pCookInfo->rawPath().c_str());
    PANIC_IF(fmt.bitDepth % 8 != 0, "Bitdepth(%d) not a multiple of 8: %s", fmt.bitDepth, pCookInfo->rawPath().c_str());
    u32 bytesPerSample = fmt.bitDepth / 8;

    PANIC_IF(data.size() % bytesPerSample != 0, "Data size not a multiple of bytes per sample: %s", pCookInfo->rawPath().c_str());
    u32 sampleCount = (u32)(data.size() / bytesPerSample);

    Gaud * pGaud = Gaud::create(fmt.sampleRate, fmt.numChannels, sampleCount);

    const u8 * pData = data.data();
    const u8 * pDataEnd = pData + data.size();
    i16 * pSamples = pGaud->samples();
    const i16 * pSamplesEnd = pSamples + pGaud->sampleCount();
    f64 sample;
    u32 sampleIdx = 0;

    while (pData < pDataEnd)
    {
        if (bytesPerSample == 2)
        {
            i32 isample = *(i16*)pData;
            sample = (f64)isample / 32768.0;
        }
        else if (bytesPerSample == 3)
        {
            i32 isample = (pData[2] << 24) | (pData[1] << 16) | (pData[0] << 8);
            sample = (f64)isample / 2147483648.0;
        }
        else if (bytesPerSample == 4)
        {
            i32 isample = *(i32*)pData;
            sample = (f64)isample / 2147483648.0;
        }
        else
        {
            PANIC("Invalid bitdepth(%d): %s", fmt.bitDepth, pCookInfo->rawPath().c_str());
        }
        pGaud->localMaxes()[pGaud->localMaxIndex(sampleIdx)] = max(pGaud->localMaxes()[pGaud->localMaxIndex(sampleIdx)], (f32)abs(sample));
        *pSamples = (i16)(sample * 32768.0);

        pData += bytesPerSample;
        pSamples++;
        sampleIdx++;
    }

    PANIC_IF(pData != pDataEnd, "Unexpected end of data: %s", pCookInfo->rawPath().c_str());
    PANIC_IF(pSamples != pSamplesEnd, "Unexpected end of samples: %s", pCookInfo->rawPath().c_str());

    pCookInfo->setCookedBuffer(pGaud);
}

} // namespace cookers
} // namespace gaen
