//------------------------------------------------------------------------------
// AudioMgr.h - Generic renderer include that defines RendererType
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

#ifndef GAEN_AUDIO_AUDIO_MGR_H
#define GAEN_AUDIO_AUDIO_MGR_H

#include <array>

#include "gaen/core/SpscRingBuffer.h"

#include "gaen/engine/Message.h"
#include "gaen/engine/Handle.h"
#include "gaen/engine/UniqueObject.h"

typedef void PaStream;

namespace gaen
{

class Gaud;
class Asset;

static const u32 kMaxSounds = 8;

struct SoundInstance : public UniqueObject
{
    SoundInstance(task_id owner,
                  ouid uid,
                  u32 priority,
                  const Asset* pAssetGaud);

    SoundInstance()
      : UniqueObject(0, 0)
    {
        zero();
    }

    void zero()
    {
        UniqueObject::zero();
        pAssetGaud = nullptr;
        pGaud = nullptr;
        currSample = 0;
        ratioCounter = 0;
    }

    const Asset* pAssetGaud;
    const Gaud * pGaud;

    u32 priority;
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
    SoundInstance * findInstance(ouid uid);
    SoundInstance * findEmpty();

    PaStream *pStream = nullptr;

    std::array<SoundInstance, kMaxSounds> mInsts;
};

// Compose API
class Entity;
namespace system_api
{

i32 play_sound(AssetHandleP pAssetHandleGaud,
               i32 priorityHash,
               Entity * pCaller);

} // namespace system_api

} // namespace gaen

#endif // #ifndef GAEN_AUDIO_AUDIO_MGR_H
