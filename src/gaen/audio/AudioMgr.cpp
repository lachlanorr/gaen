//------------------------------------------------------------------------------
// AudioMgr.cpp - Generic renderer include that defines RendererType
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

#include <portaudio.h>

#include "gaen/core/base_defines.h"
#include "gaen/core/logging.h"

#include "gaen/assets/Gaud.h"

#include "gaen/engine/Task.h"
#include "gaen/engine/Asset.h"
#include "gaen/engine/AssetMgr.h"
#include "gaen/engine/Entity.h"
#include "gaen/engine/messages/SoundInstance.h"

#include "gaen/audio/AudioMgr.h"

namespace gaen
{

static const f32 kMaxVolume = 0.90;

static const u32 kPriorityCount = 4;
static const u32 kMaxPriority = kPriorityCount - 1;

typedef std::array<f32, kPriorityCount> Ratios;
typedef std::array<Ratios, 16 /*1 << kPriorityCount*/> RatioArray;

// Each of these Ratios should add to 1.0
// They will get scaled down based on kMaxVolume
static RatioArray kBaseRatios{
    Ratios{ 0.0,  0.0,  0.0,  0.0 }, // 0 0 0 0
    Ratios{ 0.0,  0.0,  0.0,  1.0 }, // 0 0 0 1
    Ratios{ 0.0,  0.0,  1.0,  0.0 }, // 0 0 1 0
    Ratios{ 0.0,  0.0,  0.6,  0.4 }, // 0 0 1 1
    Ratios{ 0.0,  1.0,  0.0,  0.0 }, // 0 1 0 0
    Ratios{ 0.0,  0.8,  0.0,  0.2 }, // 0 1 0 1
    Ratios{ 0.0,  0.7,  0.3,  0.0 }, // 0 1 1 0
    Ratios{ 0.0,  0.7,  0.2,  0.1 }, // 0 1 1 1
    Ratios{ 1.0,  0.0,  0.0,  0.0 }, // 1 0 0 0
    Ratios{ 0.9,  0.0,  0.0,  0.1 }, // 1 0 0 1
    Ratios{ 0.8,  0.0,  0.2,  0.0 }, // 1 0 1 0
    Ratios{ 0.8,  0.0, 0.15, 0.05 }, // 1 0 1 1
    Ratios{ 0.7,  0.3,  0.0,  0.0 }, // 1 1 0 0
    Ratios{ 0.7, 0.25,  0.0, 0.05 }, // 1 1 0 1
    Ratios{ 0.7,  0.2,  0.1,  0.0 }, // 1 1 1 0
    Ratios{ 0.7,  0.2, 0.07, 0.03 }  // 1 1 1 1
};

static bool scale_ratios(const RatioArray & ratioArray)
{
    for (auto & ratio : kBaseRatios)
    {
        for (u32 i = 0; i < ratio.size(); ++i)
        {
            ratio[i] = ratio[i] * kMaxVolume;
        }
    }
    return true;
}

static const bool kAreRatiosValid = scale_ratios(kBaseRatios);

SoundInstance::SoundInstance(task_id owner,
                             ouid uid,
                             u32 priority,
                             const Asset* pAssetGaud)
  : UniqueObject(owner, uid)
  , pAssetGaud(pAssetGaud)
  , priority(priority)
{
    pGaud = pAssetGaud->buffer<Gaud>();

    if (priority > kMaxPriority)
    {
        LOG_ERROR("Invalid sound priority: %d, changing to max %d", priority, kMaxPriority);
        priority = kMaxPriority;
    }

    currSample = 0;
    ratioCounter = 0;
}

enum SoundCommandType
{
    kSCMD_Stop = 0,
    kSCMD_Start = 1,
    kSCMD_Finished = 2,
};

struct SoundCommand
{
    SoundCommandType cmdType;
    SoundInstance * pSoundInstance;
};

typedef SpscRingBuffer<SoundCommand> SoundQueue;
static SoundQueue sCallbackQueue(kMaxSounds*4, kMEM_Audio);
static SoundQueue sEngineQueue(kMaxSounds*4, kMEM_Audio);

static void insert_instance(std::array<SoundInstance*, kMaxSounds> & insts, SoundInstance * pInst)
{
    for (u32 i = 0; i < insts.size(); ++i)
    {
        if (insts[i] == nullptr)
        {
            insts[i] = pInst;
            return;
        }
    }
}

struct PriorityInfo
{
    u32 count;
    f32 localMax;
};

static int pa_stream_callback(const void *pInput,
                              void *pOutput,
                              unsigned long frameCount,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *pUserData)
{
    static std::array<SoundInstance*, kMaxSounds> playing{0};

    // check queue for new commands
    SoundQueue::Accessor acc;
    sCallbackQueue.popBegin(&acc);
    u32 available = acc.available();
    for (u32 i = 0; i < acc.available(); ++i)
    {
        switch (acc[i].cmdType)
        {
        case kSCMD_Start:
        {
            insert_instance(playing, acc[i].pSoundInstance);
            break;
        }
        default:
            LOG_ERROR("AudioMgr callback unknown command type: %d", acc[i].cmdType);
            break;
        }
    }
    sCallbackQueue.popCommit(available);

    // find which priority sounds are available to play and choose ratios
    u32 priorityBitmap = 0;
    Ratios priorityMaxes{0.0};
    for (const auto pInst : playing)
    {
        if (pInst != nullptr)
        {
            priorityBitmap |= (1 << (kMaxPriority - pInst->priority));
            priorityMaxes[pInst->priority] += pInst->pGaud->localMax(pInst->currSample);
        }
    }
    Ratios ratios{0.0};
    f32 budget = 0.0; // remaining budget from previous priorities
    for (u32 i = 0; i < ratios.size(); ++i)
    {
        f32 priorityBudget = kBaseRatios[priorityBitmap][i] + budget;
        if (priorityMaxes[i] == 0)
        {
            // no volume in this priority, give our budget to next priority
            ratios[i] = 0.0;
            budget += kBaseRatios[priorityBitmap][i];
        }
        else if (priorityMaxes[i] >= priorityBudget)
        {
            ratios[i] = priorityBudget / priorityMaxes[i];
            // no remaining budget
        }
        else
        {
            // we're below our budget, so don't compress samples in this priority
            ratios[i] = 1.0;
            // give our remaining budget to next priority
            budget += priorityBudget - priorityMaxes[i];
        }
    }

    // Process playing sounds
    i16 * pLeftOut = (i16*)pOutput;
    i16 * pEnd = pLeftOut + frameCount * 2; // we're always in stereo

    while (pLeftOut < pEnd)
    {
        i16 * pRightOut = pLeftOut+1;

        // initialize to zero
        *pLeftOut = 0;
        *pRightOut = 0;

        for (auto pInst : playing)
        {
            if (pInst != nullptr)
            {
                f32 ratio = ratios[pInst->priority];
                if (pInst->currSample < pInst->pGaud->sampleCount())
                {
                    if (pInst->ratioCounter == 0)
                        pInst->ratioCounter = pInst->pGaud->sampleRatio();
                    pInst->ratioCounter--;

                    *pLeftOut += (i16)(pInst->pGaud->samples()[pInst->currSample] * ratio);
                    if (pInst->pGaud->numChannels() == 2)
                    {
                        *pRightOut += (i16)(pInst->pGaud->samples()[pInst->currSample+1] * ratio);
                    }
                    else
                    {
                        // mono sound duplicate to right channel
                        *pRightOut += (i16)(pInst->pGaud->samples()[pInst->currSample] * ratio);
                    }
                    if (pInst->ratioCounter == 0)
                        pInst->currSample += pInst->pGaud->numChannels();
                }
            }
        }

        pLeftOut += 2; // we're always in stereo
    }

    // count up finished sounds so we can push them all
    u32 finishedSounds = 0;
    for (auto pInst : playing)
    {
        if (pInst != nullptr && pInst->currSample >= pInst->pGaud->sampleCount())
            finishedSounds++;
    }

    if (finishedSounds > 0)
    {
        SoundQueue::Accessor acc;
        sEngineQueue.pushBegin(&acc, finishedSounds);
        u32 pushed = 0;

        for (u32 i = 0; i < playing.size(); ++i)
        {
            if (playing[i] != nullptr && playing[i]->currSample >= playing[i]->pGaud->sampleCount())
            {
                acc[pushed++] = SoundCommand{
                    kSCMD_Finished,
                    playing[i]
                };
                playing[i] = nullptr;
            }
        }
        sEngineQueue.pushCommit(pushed);
    }

    return 0;
}

AudioMgr::AudioMgr()
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        LOG_ERROR("PortAudio Pa_Initialize error: %s", Pa_GetErrorText(err));
        return;
    }

    err = Pa_OpenDefaultStream(&pStream,
                               0,          /* no input channels */
                               2,          /* stereo output */
                               paInt16,  /* 32 bit floating point output */
                               44100, //SAMPLE_RATE,
                               100,        /* frames per buffer, i.e. the number
                                                  of sample frames that PortAudio will
                                                  request from the callback. Many apps
                                                  may want to use
                                                  paFramesPerBufferUnspecified, which
                                                  tells PortAudio to pick the best,
                                                  possibly changing, buffer size.*/
                               pa_stream_callback, /* this is your callback function */
                               nullptr); /*This is a pointer that will be passed to
                                                  your callback*/
    if (err != paNoError)
    {
        LOG_ERROR("PortAudio Pa_OpenDefaultStream error: %s", Pa_GetErrorText(err));
        return;
    }

    err = Pa_StartStream(pStream);
    if (err != paNoError)
    {
        LOG_ERROR("PortAudio Pa_StartStream error: %s", Pa_GetErrorText(err));
        return;
    }
}

AudioMgr::~AudioMgr()
{
    PaError err;

    if (pStream != nullptr)
    {
        err = Pa_AbortStream(pStream);
        if (err != paNoError)
            LOG_ERROR("PortAudio Pa_AbortStream error: %s", Pa_GetErrorText(err));
    }

    err = Pa_Terminate();
    if (err != paNoError)
    {
        LOG_ERROR("PortAudio Pa_Terminate error: %s", Pa_GetErrorText(err));
        return;
    }

    for (auto & inst : mInsts)
    {
        if (inst.uid() != 0)
        {
            AssetMgr::release_asset(kAssetMgrTaskId, inst.pAssetGaud);
        }
    }
}

void AudioMgr::update(f32 delta)
{
    // check queue for new commands
    SoundQueue::Accessor acc;
    sEngineQueue.popBegin(&acc);
    u32 available = acc.available();
    for (u32 i = 0; i < acc.available(); ++i)
    {
        switch (acc[i].cmdType)
        {
        case kSCMD_Finished:
        {
            AssetMgr::release_asset(kAssetMgrTaskId, acc[i].pSoundInstance->pAssetGaud);
            SoundInstance * pInst = findInstance(acc[i].pSoundInstance->uid());
            if (pInst == nullptr)
            {
                LOG_ERROR("Unknown sound finished: %d", acc[i].pSoundInstance->uid());
            }
            else
            {
                pInst->zero();
            }
            break;
        }
        default:
            LOG_ERROR("AudioMgr update unknown command type: %d", acc[i].cmdType);
            break;
        }
    }
    sEngineQueue.popCommit(available);
}

template <typename T>
MessageResult AudioMgr::message(const T & msgAcc)
{
#if HAS(MESSAGE_TRACING)
    LOG_INFO("MSG: Renderer %s(0x%x) -> %s(0x%x): %s(%s)", task_name(msgAcc.message().source), msgAcc.message().source, task_name(msgAcc.message().target), msgAcc.message().target, HASH::reverse_hash(msgAcc.message().msgId), HASH::reverse_hash(msgAcc.message().payload.u));
#endif
    const Message & msg = msgAcc.message();

    switch (msg.msgId)
    {
    case HASH::start_music:
    {
        messages::SoundInstanceR<T> msgr(msgAcc);

        SoundInstance * pInst = findInstance(msgr.uid());
        if (pInst != nullptr)
        {
            LOG_ERROR("Sound already playing: %d", msgr.uid());
            break;
        }

        pInst = findEmpty();
        if (pInst == nullptr)
        {
            LOG_ERROR("Too many sounds playing");
            break;
        }

        u32 priority = 1; // default to low priprity
        switch (msgr.priorityHash())
        {
        case HASH::dialog:
            priority = 0;
            break;
        case HASH::effect:
            priority = 1;
            break;
        case HASH::ambient:
            priority = 2;
            break;
        case HASH::music:
            priority = 3;
            break;
        default:
            LOG_ERROR("Invalid priorityHash: %d, defaulting to %d", msgr.priorityHash(), kMaxPriority);
            priority = kMaxPriority;
        }

        pInst = new(pInst) SoundInstance(msg.source, msgr.uid(), priority, msgr.assetGaud());

        SoundQueue::Accessor acc;
        sCallbackQueue.pushBegin(&acc, 1);
        acc[0] = SoundCommand{
            kSCMD_Start,
            pInst
        };
        sCallbackQueue.pushCommit(1);

        break;
    }

    default:
        LOG_ERROR("Unknown AudioMgr message: %d", msg.msgId);
        break;
    }

    return MessageResult::Consumed;
}

SoundInstance * AudioMgr::findInstance(ouid uid)
{
    for (auto & inst : mInsts)
    {
        if (inst.uid() == uid)
            return &inst;
    }
    return nullptr;
}

SoundInstance * AudioMgr::findEmpty()
{
    for (auto & inst : mInsts)
    {
        if (inst.uid() == 0)
            return &inst;
    }
    return nullptr;
}


// Template decls so we can define message func here in the .cpp
template MessageResult AudioMgr::message<MessageQueueAccessor>(const MessageQueueAccessor & msgAcc);
template MessageResult AudioMgr::message<MessageBlockAccessor>(const MessageBlockAccessor & msgAcc);


namespace system_api
{

i32 play_sound(AssetHandleP pAssetHandleGaud,
               i32 priorityHash,
               Entity * pCaller)
{
    ASSERT(pAssetHandleGaud->typeHash() == HASH::asset);
    const Asset * pAssetGaud = pAssetHandleGaud->data<Asset>();
    AssetMgr::addref_asset(gaen::kAudioMgrTaskId, pAssetGaud);

    ouid uid = UniqueObject::next_uid();
    messages::SoundInstanceBW msgw(HASH::start_music, kMessageFlag_None, pCaller->task().id(), kAudioMgrTaskId, uid);
    msgw.setPriorityHash(priorityHash);
    msgw.setAssetGaud(pAssetGaud);
    send_message(msgw);

    return uid;
}

} // namespace system_api

} // namespace gaen
