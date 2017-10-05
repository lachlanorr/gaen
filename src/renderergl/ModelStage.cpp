//------------------------------------------------------------------------------
// ModelStage.cpp - Grouping of models in a plane, including a cara layer
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

#include "math/common.h"
#include "math/matrices.h"
#include "math/mat4.h"

#include "render_support/Model.h"

#include "renderergl/RendererMesh.h"
#include "renderergl/ModelGL.h"
#include "renderergl/ModelStage.h"

namespace gaen
{

ModelStage::ModelStage(u32 stageHash, RendererMesh * pRenderer)
  : Stage(stageHash,
          pRenderer,
          Camera(kRendererTaskId,
                 stageHash,
                 1.0f,
                 perspective(radians(60.0f),
                             pRenderer->screenWidth() / (f32)pRenderer->screenHeight(),
                             0.1f,
                             100000.0f),
                 mat43(1.0f)))
{}

} // namespace gaen
