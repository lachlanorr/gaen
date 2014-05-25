//------------------------------------------------------------------------------
// model_viewer.cpp - Load some models and allow mouse based interaction
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include "samples/stdafx.h"

#include "engine/Entity.h"
//#include "engine/Components/ModelComponent.h"
#include "engine/shapes.h"

#include "samples/model_viewer/model_viewer.h"

namespace gaen
{

Entity * model_viewer_init_entity()
{
    //Model * pModel = buildTriModel(20.0f, 20.0f, Color(255, 255, 0, 255));

    // LORRTODO
    //Entity * pEntity = GNEW(kMEM_Engine, Entity, 256);
    //pEntity->addComponent(GNEW(kMEM_Engine, ModelComponent, pModel));

    //return pEntity;

    return nullptr;
}

} // namespace gaen
