//------------------------------------------------------------------------------
// system_api.cpp - C functions available to Compose scripts
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
#include "math/common.h"
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

} // namespace system_api

} // namespace gaen
