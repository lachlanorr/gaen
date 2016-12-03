//------------------------------------------------------------------------------
// SpriteStage.h - Grouping of sprites in a plane, including a cara layer
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

#ifndef GAEN_RENDERERGL_SPRITESTAGE_H
#define GAEN_RENDERERGL_SPRITESTAGE_H

#include "core/base_defines.h"

#include "renderergl/SpriteGL.h"
#include "renderergl/FrameGL.h"
#include "renderergl/Stage.h"

namespace gaen
{

class SpriteInstance;

class SpriteStage : public Stage<SpriteGL>
{
public:
    SpriteStage(u32 stageHash, RendererMesh * pRenderer);

    bool animateItem(u32 uid, u32 animHash, u32 animFrameIdx);
}; // class SpriteStage

typedef UniquePtr<SpriteStage> SpriteStageUP;

} // namespace gaen

#endif // #ifndef GAEN_RENDERERGL_SPRITESTAGE_H
