//------------------------------------------------------------------------------
// ModelComponent.h - Component that encapsulate a static model
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

#ifndef GAEN_ENGINE_MODEL_COMPONENT_H
#define GAEN_ENGINE_MODEL_COMPONENT_H

#include "engine/Model.h"
#include "engine/Component.h"

namespace gaen
{

// Base component that handles some generic stuff all components must
// abide by

class ModelComponent : public Component
{
public:
    ModelComponent(Model * pModel)
      : mpModel(pModel) {}
    
    void update(f32 deltaSecs);
    MessageResult message(const MessageQueue::MessageAccessor& msgAcc);

private:
    Model * mpModel;

};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODEL_COMPONENT_H
