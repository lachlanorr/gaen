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

#include "math/mat3.h"

#include "assets/file_utils.h"
#include "assets/Config.h"
#include "assets/Gaim.h"

#include "chef/CookInfo.h"
#include "chef/Chef.h"
#include "chef/cookers/Model.h"
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

static void add_bone_transforms(Gaim::AnimRaw & animRaw,
                                const rapidjson::Value & boneList,
                                const Vector<kMEM_Chef, Bone> & bones)
{
    for (u32 j = 0; j < boneList.Size(); ++j)
    {
        const rapidjson::Value & b = boneList[j];
        PANIC_IF(HASH::hash_func(b["name"].GetString()) != bones[j].nameHash, "Bones out of order between .skl and .ani");
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
                    const Vector<kMEM_Chef, Bone> & bones)
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
    PANIC_IF(boneCount != bones.size(), "Incompatible .skl and .ani files");
    u32 transformCount = animRaw.info.frameCount * boneCount;

    animRaw.transforms.reserve(transformCount);
    for (u32 i = 0; i < transforms.Size(); i++)
    {
        add_bone_transforms(animRaw, transforms[i]["bones"], bones);
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

    ChefString fullSklPath = pCookInfo->chef().getRelativeDependencyRawPath(pCookInfo->rawPath(), aim.get("skeleton"));
    Model::Skeleton skel;
    Model::read_skl(skel, fullSklPath.c_str());

    // Add default pose into gaim (bones transforms from gmdl)
    Gaim::AnimRaw def;
    def.info.nameHash = HASH::default;
    def.info.frameDuration = 1.0 / 60.0;
    def.info.frameCount = 1;
    def.info.framesOffset = 0;
    def.info.isLoopable = 1;
    def.info.totalTime = 0.0;
    def.transforms.reserve(skel.bones.size());
    add_bone_transforms(def, skel.jsonDoc["bones"], skel.bones);
    animsRaw.push_back(def);

    auto itend = aim.keysEnd("animations");
    for (auto it = aim.keysBegin("animations");
         it != itend;
         ++it)
    {
        ChefString aniPath(aim.get("animations", *it));
        pCookInfo->recordDependency(aniPath);
        ChefString fullAniPath = pCookInfo->chef().getRelativeDependencyRawPath(pCookInfo->rawPath(), aniPath);
        animsRaw.push_back(Gaim::AnimRaw());
        read_ani(animsRaw.back(), *it, fullAniPath.c_str(), skel.bones);
    }

    Gaim * pGaim = Gaim::create(animsRaw);

    pCookInfo->setCookedBuffer(pGaim);
}

}
} // namespace gaen


