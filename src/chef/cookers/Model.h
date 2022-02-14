//------------------------------------------------------------------------------
// Model.h - Model cooker
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

#ifndef GAEN_CHEF_COOKERS_MODEL_H
#define GAEN_CHEF_COOKERS_MODEL_H

#include "assets/Gmdl.h"
#include "chef/Cooker.h"

namespace gaen
{
namespace cookers
{

static const char * kExtObj = "obj";
static const char * kExtPly = "ply";
static const char * kExtOgex = "ogex";
static const char * kExtGltf = "gltf";
static const char * kExtGmdl = "gmdl";

class Model : public Cooker
{
public:
    struct Skeleton
    {
        bool hasCenter = false;
        vec3 center = vec3(0.0f);
        bool hasHalfExtents = false;
        vec3 halfExtents = vec3(0.0f);
        Vector<kMEM_Chef, Bone> bones;
        Vector<kMEM_Chef, Bone> hardpoints;
        rapidjson::Document jsonDoc;
    };

    Model();
    virtual void cook(CookInfo * pCookInfo) const;

    static void read_skl(Skeleton & skel, const char * path);
    static u32 bone_id(const Vector<kMEM_Chef, Bone> & bones, const char * name);
};

}
} // namespace gaen

#endif // #ifndef GAEN_CHEF_COOKERS_MODEL_H
