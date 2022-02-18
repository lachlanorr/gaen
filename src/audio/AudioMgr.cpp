//------------------------------------------------------------------------------
// AudioMgr.cpp - Generic renderer include that defines RendererType
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

#include <portaudio.h>

#include "core/base_defines.h"
#include "core/logging.h"

#include "assets/Gaud.h"

#include "engine/Task.h"
#include "engine/Asset.h"
#include "engine/AssetMgr.h"
#include "engine/Entity.h"
#include "engine/messages/SoundInstance.h"

#include "audio/AudioMgr.h"

namespace gaen
{

SoundInstance::SoundInstance(task_id owner,
                             ouid uid,
                             i32 priorityHash,
                             const Asset* pAssetGaud)
  : UniqueObject(owner, uid)
  , pAssetGaud(pAssetGaud)
{
    pGaud = pAssetGaud->buffer<Gaud>();

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

    u16 * pLeftOut = (u16*)pOutput;
    u16 * pEnd = pLeftOut + frameCount * 2; // we're always in stereo

    while (pLeftOut < pEnd)
    {
        u16 * pRightOut = pLeftOut+1;

        // initialize to zero
        *pLeftOut = 0;
        *pRightOut = 0;

        for (auto pInst : playing)
        {
            if (pInst != nullptr)
            {
                if (pInst->currSample < pInst->pGaud->sampleCount())
                {
                    if (pInst->ratioCounter == 0)
                        pInst->ratioCounter = pInst->pGaud->sampleRatio();
                    pInst->ratioCounter--;

                    *pLeftOut += pInst->pGaud->samples()[pInst->currSample];
                    if (pInst->pGaud->numChannels() == 2)
                    {
                        *pRightOut += pInst->pGaud->samples()[pInst->currSample+1];
                    }
                    else
                    {
                        // mono sound duplicate to right channel
                        *pRightOut += pInst->pGaud->samples()[pInst->currSample];
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
        AssetMgr::release_asset(kAssetMgrTaskId, inst.pAssetGaud);
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

        pInst = new(pInst) SoundInstance(msg.source, msgr.uid(), msgr.priorityHash(), msgr.assetGaud());

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
