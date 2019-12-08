//------------------------------------------------------------------------------
// AnimImage.cpp - Gimg wrapper for storing animation bone transforms
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

#include "core/Vector.h"
#include "hashes/hashes.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gaim.h"

#include "chef/CookInfo.h"
#include "chef/Chef.h"
#include "chef/cookers/AnimImage.h"

namespace gaen
{
namespace cookers
{

AnimImage::AnimImage()
{
    setVersion(1);
    addRawExt(kExtAim);
    addCookedExtExclusive(kExtGaim);
}

static void readRawAni(Gaim::AnimRaw & animRaw, const char * animName, const char * path)
{
    FileReader rdr(path);
    PANIC_IF(!rdr.isOk(), "Unable to load ani file: %s", path);
    Scoped_GFREE<char> jsonStr((char*)GALLOC(kMEM_Chef, rdr.size()+1)); // +1 for null we'll add to end
    rdr.read(jsonStr.get(), rdr.size());
    jsonStr.get()[rdr.size()] = '\0';
    rdr.ifs.close();

    rapidjson::Document d;
    d.Parse(jsonStr.get());

    const rapidjson::Value & transforms = d["transforms"];

    animRaw.info.nameHash = HASH::hash_func(animName);
    animRaw.info.totalTime = transforms[transforms.Size()-1]["time"].GetFloat();
    animRaw.info.frameDuration = d["frameDuration"].GetFloat();
    animRaw.info.frameCount = transforms.Size();
    animRaw.info.framesOffset = 0;
    animRaw.info.isLoopable = d["isLoopable"].GetBool();

    u32 boneCount = d["boneCount"].GetInt();
    u32 transformCount = animRaw.info.frameCount * boneCount;

    animRaw.transforms.reserve(transformCount);
    for (u32 i = 0; i < transforms.Size(); i++)
    {
        const rapidjson::Value & transItem = transforms[i];
        for (u32 j = 0; j < transItem["transforms"].Size(); ++j)
        {
            const rapidjson::Value & trans = transItem["transforms"][j];
            vec3 pos = vec3(trans["pos"][0].GetFloat(),
                            trans["pos"][1].GetFloat(),
                            trans["pos"][2].GetFloat());
            vec3 rot = vec3(trans["rot"][0].GetFloat(),
                            trans["rot"][1].GetFloat(),
                            trans["rot"][2].GetFloat());
            PANIC_IF(animRaw.transforms.capacity() == animRaw.transforms.size(), "About to resize transforms vector, all space should have been reserved.");
            animRaw.transforms.emplace_back(pos, rot);
        }
    }
}

void AnimImage::cook(CookInfo * pCookInfo) const
{
    Gaim::AnimsRaw animsRaw;

    Config<kMEM_Chef> aim;
    {
        FileReader rdr(pCookInfo->rawPath().c_str());
        PANIC_IF(!rdr.isOk(), "Unable to load file: %s", pCookInfo->rawPath().c_str());
        aim.read(rdr.ifs);
    }
    for (auto it = aim.keysBegin();
         it != aim.keysEnd();
         ++it)
    {
        ChefString aniPath(aim.get(*it));
        pCookInfo->recordDependency(aniPath);
        ChefString fullAniPath = pCookInfo->chef().getRelativeDependencyRawPath(pCookInfo->rawPath(), aniPath);
        animsRaw.push_back(Gaim::AnimRaw());
        readRawAni(animsRaw.back(), *it, fullAniPath.c_str());
    }

    Gaim * pGaim = Gaim::create(animsRaw);

    pCookInfo->setCookedBuffer(pGaim);
}

}
} // namespace gaen


