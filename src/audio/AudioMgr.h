//------------------------------------------------------------------------------
// AudioMgr.h - Generic renderer include that defines RendererType
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_AUDIO_AUDIO_MGR_H
#define GAEN_AUDIO_AUDIO_MGR_H

#include "core/SpscRingBuffer.h"

#include "engine/Message.h"
#include "engine/Handle.h"
#include "engine/UniqueObject.h"

typedef void PaStream;

namespace gaen
{

class Gaud;
class Asset;

struct SoundInstance : public UniqueObject
{
    SoundInstance(task_id owner,
                  const Asset* pAssetGaud);

    const Asset* pAssetGaud;
    const Gaud * pGaud;

    u32 currSample = 0;
    u32 ratioCounter = 0;
};

class AudioMgr
{
public:
    AudioMgr();
    ~AudioMgr();

    void update(f32 delta);

    template <typename T>
    MessageResult message(const T& msgAcc);

private:
    PaStream *pStream = nullptr;
};

// Compose API
class Entity;
namespace system_api
{

i32 start_music(AssetHandleP pAssetHandleGaud,
                Entity * pCaller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_AUDIO_AUDIO_MGR_H
