//------------------------------------------------------------------------------
// system_api.h - C functionas available to Compose scripts
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
#include "engine/stdafx.h"

#include "core/mem.h"
#include "engine/hashes.h"
#include "engine/math.h"
#include "engine/shapes.h"
#include "engine/messages/InsertModelInstance.h"

#include "engine/system_api.h"

namespace gaen
{

//------------------------------------------------------------------------------
// API Defs start here
//------------------------------------------------------------------------------

namespace system_api
{

Handle create_shape_box(const Vec3 & size, const Color & color, const Entity & caller)
{
    Model * pModel = build_box(size, color);

    return Handle(HASH::Model, 0, 0, sizeof(Model), pModel, nullptr);
}

void renderer_insert_model_instance(Handle & modelHandle, const u32 & instanceId, const Mat34 & transform, const Entity & caller)
{
    if (modelHandle.typeHash() != HASH::Model)
        PANIC("Invalid model handle");

    msg::InsertModelInstanceQW msgQW(HASH::renderer_insert_model_instance, kMessageFlag_None, caller.task().id(), kRendererTaskId, instanceId);
    msgQW.setIsAssetManaged(true);
    msgQW.setModel((Model*)modelHandle.data());
    msgQW.setWorldTransform(transform);
}

} // namespace system_api

} // namespace gaen
