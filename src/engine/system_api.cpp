//------------------------------------------------------------------------------
// system_api.cpp - C functionas available to Compose scripts
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
#include "engine/stdafx.h"

#include "core/mem.h"
#include "core/logging.h"
#include "hashes/hashes.h"
#include "engine/glm_ext.h"
#include "engine/InputMgr.h"
#include "engine/MessageWriter.h"

#include "engine/system_api.h"

namespace gaen
{

//------------------------------------------------------------------------------
// API Defs start here
//------------------------------------------------------------------------------

namespace system_api
{

void print(CmpString str, Entity & caller)
{
    LOG_INFO(str.c_str());
}

CmpString hashstr(i32 hash, Entity & caller)
{
    return caller.blockMemory().stringAlloc(HASH::reverse_hash(hash));
}

void print_asset_info(AssetHandleP asset, Entity & caller)
{
    LOG_INFO("print_asset_info");
}

//void activate_entity(i32 id, Entity & caller)
//{
//    caller.activateEntity(id);
//}

f32 radians(f32 degrees, Entity & caller)
{
    return glm::radians(degrees);
}

f32 degrees(f32 radians, Entity & caller)
{
    return glm::degrees(radians);
}

glm::mat4x3 transform(const glm::vec3 & pos, glm::vec3 & angles, Entity & caller)
{
    return glm::mat43_transform(pos, angles);
}

glm::mat4x3 position(const glm::vec3 & pos, Entity & caller)
{
    return glm::mat43_position(pos);
}

glm::mat4x3 rotation(const glm::vec3 & angles, Entity & caller)
{
    return glm::mat43_rotation(angles);
}

glm::mat3 rotation_mat3(const glm::vec3 & angles, Entity & caller)
{
    return glm::mat3_rotation(angles);
}

glm::quat quat_from_axis_angle(const glm::vec3 & dir, f32 angle, Entity & caller)
{
    return glm::quat(angle, dir);
}

glm::quat quat_normalize(const glm::quat & quat, Entity & caller)
{
    glm::quat q = quat;
    glm::normalize(q);
    return q;
}

} // namespace system_api

} // namespace gaen
