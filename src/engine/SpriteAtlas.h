//------------------------------------------------------------------------------
// SpriteAtlas.h - Combination of a gatl alognwith quads to draw the glyphs
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_ENGINE_SPRITEATLAS_H
#define GAEN_ENGINE_SPRITEATLAS_H

#include "core/base_defines.h"

namespace gaen
{

class Asset;

struct SpriteVert
{
    glm::vec3 position;
    f32 u;
    f32 v;
};
static_assert(sizeof(SpriteVert) == 20, "SpriteVert unexpected size");

struct SpriteTri
{
    u16 p0;
    u16 p1;
    u16 p2;
};
static_assert(sizeof(SpriteTri) == 6, "SpriteTri unexpected size");


class SpriteAtlas
{
public:
    SpriteAtlas(Asset * pGatlAtlas);
    ~SpriteAtlas();
    
private:
    Asset * mpGatlAtlas;
    SpriteVert * pVerts;
    SpriteTri * pTris;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITEATLAS_H
