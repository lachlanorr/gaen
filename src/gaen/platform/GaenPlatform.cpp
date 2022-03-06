//------------------------------------------------------------------------------
// GaenPlatform.cpp - Gaen plaform specific app management
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#include "gaen/engine/TaskMaster.h"
#include "gaen/platform/gaen.h"
#include "gaen/platform/GaenPlatform.h"

namespace gaen
{

GaenPlatform::GaenPlatform(int argc, char ** argv)
  : mpContext(nullptr)
{
    init(argc, argv);

    TaskMaster & tm = TaskMaster::primary_task_master();
    Task t = Task::create_updatable(this, HASH::gaen_platform);
    t.setStatus(TaskStatus::Running);
    tm.setPlatformTask(t);
}

GaenPlatform::~GaenPlatform()
{
    fin();
}

void GaenPlatform::start()
{
    start_game_loops();

    shutdown();
}


template <typename T>
MessageResult GaenPlatform::message(const T & msgAcc)
{
    const Message & msg = msgAcc.message();
    PANIC("Message received in GaenPlaform");
    return MessageResult::Propagate;
}

} // namespace gaen
