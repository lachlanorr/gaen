//------------------------------------------------------------------------------
// Material.h - Materials used with models
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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

#ifndef GAEN_ENGINE_MATERIAL_H
#define GAEN_ENGINE_MATERIAL_H

#include "engine/Color.h"

namespace gaen
{

// Define these in the order of rendering.
// Higher MaterialTypes get rendered later.
// i.e., transparent stuff should be highest.
enum MaterialType
{
    kMAT_Colored = 0,

    // LORRTODO - Add additional shaders

    kMAT_END
};

typedef u32 material_id;

class Material
{
public:
    Material(Color color);

    material_id id() const { return mId; }

    MaterialType type() const { return mType; }
    const Vec4 & color() const { return mColor; }

private:
    material_id mId;
    MaterialType mType;
    Vec4 mColor;
};


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MATERIAL_H
