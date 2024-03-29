//------------------------------------------------------------------------------
// test_task.cpp - Tests for engine/Task class
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include <cstdio>
#include <vector>

#include <gtest/gtest.h>

#include "gaen/engine/Task.h"
#include "gaen/engine/MessageQueue.h"

class Entity1
{
    friend struct gaen::Task;
public:
    void update(gaen::f32 delta)
    {
        printf("Entity1::update, delta = %f\n", delta);
    }

    template <typename T>
    gaen::MessageResult message(const T & msgAcc)
    {
        printf("Entity1::message\n");
        return gaen::MessageResult::Propagate;
    }

private:
    void setTaskId(gaen::task_id taskId) { mTaskId = taskId; }

    gaen::task_id mTaskId;
};

class Entity2
{
    friend struct gaen::Task;
public:
    void update(gaen::f32 delta)
    {
        printf("Entity2::update, delta = %f\n", delta);
    }

    template <typename T>
    gaen::MessageResult message(const T & msgAcc)
    {
        printf("Entity2::message\n");
        return gaen::MessageResult::Propagate;
    }

    
private:
    void setTaskId(gaen::task_id taskId) { mTaskId = taskId; }

    gaen::task_id mTaskId;
};


TEST(TaskTest, CreateTasks)
{
    Entity1 e1;
    Entity2 e2;

    std::vector<gaen::Task> vec(2);

    vec[0] = gaen::Task::create_updatable(&e1, 0);
    vec[1] = gaen::Task::create_updatable(&e2, 0);

    gaen::MessageQueueAccessor msgAcc;

    for(gaen::Task & t : vec)
    {
        t.update(1.23);
        t.message(msgAcc);
    }
}

