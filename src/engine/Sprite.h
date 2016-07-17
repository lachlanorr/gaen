//------------------------------------------------------------------------------
// Sprite.h - 2d graphics object
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

#ifndef GAEN_ENGINE_SPRITE_H
#define GAEN_ENGINE_SPRITE_H

#include <glm/mat4x3.hpp>

namespace gaen
{

class Asset;

class Sprite
{
public:
    Sprite(task_id owner, Asset* pGsprAsset, const glm::mat4x3 & transform);
    ~Sprite();

    const glm::mat4x3 & transform() { return mTransform; }
    void setTransform(const glm::mat4x3 & transform);
private:
    task_id mOwner;
    Asset * mpGsprAsset;
    glm::mat4x3 mTransform;
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_SPRITE_H
