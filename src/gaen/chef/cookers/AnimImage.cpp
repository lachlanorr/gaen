//------------------------------------------------------------------------------
// AnimImage.cpp - Gimg wrapper for storing animation bone transforms
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

#include "gaen/core/Vector.h"
#include "gaen/core/hashing.h"
#include "gaen/hashes/hashes.h"

#include "gaen/math/mat3.h"

#include "gaen/assets/file_utils.h"
#include "gaen/assets/Config.h"
#include "gaen/assets/Gaim.h"

#include "gaen/chef/CookInfo.h"
#include "gaen/chef/Chef.h"
#include "gaen/chef/cookers/Model.h"
#include "gaen/chef/cookers/Image.h"
#include "gaen/chef/cookers/AnimImage.h"

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

static void add_bone_transforms(Gaim::AnimRaw & animRaw,
                                const rapidjson::Value & boneList)
{
    for (u32 j = 0; j < boneList.Size(); ++j)
    {
        const rapidjson::Value & b = boneList[j];
        mat43 transform(vec3(b["transform"][0][0].GetFloat(), b["transform"][0][1].GetFloat(), b["transform"][0][2].GetFloat()),
                        vec3(b["transform"][1][0].GetFloat(), b["transform"][1][1].GetFloat(), b["transform"][1][2].GetFloat()),
                        vec3(b["transform"][2][0].GetFloat(), b["transform"][2][1].GetFloat(), b["transform"][2][2].GetFloat()),
                        vec3(b["transform"][3][0].GetFloat(), b["transform"][3][1].GetFloat(), b["transform"][3][2].GetFloat()));
        PANIC_IF(animRaw.transforms.capacity() == animRaw.transforms.size(), "About to resize transforms vector, all space should have been reserved.");
        animRaw.transforms.push_back(transform);
    }
}

static void read_ani(Gaim::AnimRaw & animRaw,
                     const char * animName,
                     const char * path,
                     u32 & boneCount)
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

    boneCount = d["boneCount"].GetInt();
    u32 transformCount = animRaw.info.frameCount * boneCount;

    animRaw.transforms.reserve(transformCount);
    for (u32 i = 0; i < transforms.Size(); i++)
    {
        add_bone_transforms(animRaw, transforms[i]["bones"]);
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

    bool hasDefault = false;
    u32 boneCount = 0;
    auto itend = aim.keysEnd("animations");
    for (auto it = aim.keysBegin("animations");
         it != itend;
         ++it)
    {
        ChefString aniPath(aim.get("animations", *it));
        pCookInfo->recordDependency(aniPath);
        ChefString fullAniPath = pCookInfo->chef().getRelativeDependencyRawPath(pCookInfo->rawPath(), aniPath);
        animsRaw.push_back(Gaim::AnimRaw());

        u32 aniBoneCount = 0;
        read_ani(animsRaw.back(), *it, fullAniPath.c_str(), aniBoneCount);

        if (boneCount == 0)
            boneCount = aniBoneCount;
        else
            PANIC_IF(boneCount != aniBoneCount, "ani with mismatched bone count");

        if (strcmp(*it, "default") == 0)
            hasDefault = true;
    }

    PANIC_IF(!hasDefault, "No default ani specified");
    PANIC_IF(animsRaw.size() == 0, "No ani's specificed");

    Gaim * pGaim = Gaim::create(animsRaw, Image::reference_path_hash(pCookInfo));

    pCookInfo->setCookedBuffer(pGaim);
}

}
} // namespace gaen


