//------------------------------------------------------------------------------
// gamevars.cpp - Support for managed global variables used for game settings
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

//------------------------------------------------------------------------------
// Game Vars are global variables that are script settable and support
// runtime evaluation of which ones exist and their current values.
//------------------------------------------------------------------------------

#include "core/stdafx.h"

#include <cstring>

#include "core/base_defines.h"
#include "core/gamevars.h"

namespace gaen
{

bool set_gamevar_with_command(const char * command)
{
    if (!command)
        return false;
    
    size_t commandLen = strlen(command);
    const char * commandEnd = command + commandLen;

    const char * equalPos = strchr(command, '=');

    if (!equalPos ||
        equalPos == commandEnd)
    {
        return false;
    }

    size_t nameLen = equalPos - command;
    if (nameLen > GameVar::kMaxNameLen-1)
        return false;

    char name[GameVar::kMaxNameLen];
    strncpy(name, command, nameLen);
    name[nameLen] = '\0';

    GameVar * pGameVar = singleton<GameVarMgr>().findGameVar(name);
    if (!pGameVar)
        return false;

    const char * val = equalPos+1;

    if (val == commandEnd)
        return false;

    char * endNum;

    switch (pGameVar->type())
    {
    case GameVarType::Int:
    {
        i32 ival = (i32)strtol(val, &endNum, 10);
        if (endNum != commandEnd ||
            !pGameVar->isValidVal(ival))
            return false;
        pGameVar->setVal(ival);
        break;
    }
    case GameVarType::Float:
    {
        f32 fval = strtof(val, &endNum);
        if (endNum != commandEnd ||
            !pGameVar->isValidVal(fval))
            return false;
        pGameVar->setVal(fval);
        break;
    }
    case GameVarType::Bool:
    {
        if (0 == strcmp("true", val) ||
            0 == strcmp("t", val))
        {
            pGameVar->setVal(true);
        }
        else if (0 == strcmp("false", val) ||
                 0 == strcmp("f", val))
        {
            pGameVar->setVal(false);
        }
        else
            return false;
        break;
    }
    default:
        return false;
    }
    return true;    
}

void GameVar::setName(const char * name)
{
    ASSERT(name);
    ASSERT(strlen(name) < kMaxNameLen-1);
    ASSERT_MSG(strlen(name) < kMaxNameLen-1, "GameVar name too long: %s", name);
    strncpy(mName, name, kMaxNameLen-1);
    mName[kMaxNameLen-1] = '\0';
}

GameVar::GameVar(const char * name, i32 val, i32 step, i32 min, i32 max)
{
    ASSERT(val >= min && val <= max);
    ASSERT(min <= max);
    setName(name);
    mType = GameVarType::Int;

    mValInfo.numVal.val.i = val;
    mValInfo.numVal.step.i = step;
    mValInfo.numVal.min.i = min;
    mValInfo.numVal.max.i = max;
}

GameVar::GameVar(const char * name, f32 val, f32 step, f32 min, f32 max)
{
    ASSERT(val >= min && val <= max);
    ASSERT(min <= max);
    setName(name);
    mType = GameVarType::Float;

    mValInfo.numVal.val.f = val;
    mValInfo.numVal.step.f = step;
    mValInfo.numVal.min.f = min;
    mValInfo.numVal.max.f = max;
}

GameVar::GameVar(const char * name, bool val)
{
    setName(name);
    mType = GameVarType::Bool;

    mValInfo.boolVal = val;
}

volatile i32 & GameVar::intRef() volatile
{
    ASSERT(mType == GameVarType::Int);
    return mValInfo.numVal.val.i;
}

volatile f32 & GameVar::floatRef() volatile
{
    ASSERT(mType == GameVarType::Float);
    return mValInfo.numVal.val.f;
}

volatile bool & GameVar::boolRef() volatile
{
    ASSERT(mType == GameVarType::Bool);
    return mValInfo.boolVal;
}

void GameVar::setVal(i32 val) volatile
{
    ASSERT(isValidVal(val));
    mValInfo.numVal.val.i = val;
}

void GameVar::setVal(f32 val) volatile
{
    ASSERT(isValidVal(val));
    mValInfo.numVal.val.f = val;
}

void GameVar::setVal(bool val) volatile
{
    ASSERT(isValidVal(val));
    mValInfo.boolVal = val;
}

bool GameVar::isValidVal(i32 val) volatile
{
    return (mType == GameVarType::Int &&
            val >= mValInfo.numVal.min.i &&
            val <= mValInfo.numVal.max.i);
}

bool GameVar::isValidVal(f32 val) volatile
{
    return (mType == GameVarType::Float &&
            val >= mValInfo.numVal.min.f &&
            val <= mValInfo.numVal.max.f);
}

bool GameVar::isValidVal(bool val) volatile
{
    return mType == GameVarType::Bool;
}

void GameVar::incVal() volatile
{
    switch (mType)
    {
    case GameVarType::Int:
    {
        int newVal = mValInfo.numVal.val.i + mValInfo.numVal.step.i;
        if (newVal <= mValInfo.numVal.max.i)
            mValInfo.numVal.val.i = newVal;
        break;
    }
    case GameVarType::Float:
    {
        float newVal = mValInfo.numVal.val.f + mValInfo.numVal.step.f;
        if (newVal <= mValInfo.numVal.max.f)
            mValInfo.numVal.val.f = newVal;
        break;
    }
    case GameVarType::Bool:
        mValInfo.boolVal = !mValInfo.boolVal;
        break;
    default:
        PANIC("Invalid GameVarType: %d", mType);
        break;
    }
}

void GameVar::decVal() volatile
{
    switch (mType)
    {
    case GameVarType::Int:
    {
        int newVal = mValInfo.numVal.val.i - mValInfo.numVal.step.i;
        if (newVal >= mValInfo.numVal.min.i)
            mValInfo.numVal.val.i = newVal;
        break;
    }
    case GameVarType::Float:
    {
        float newVal = mValInfo.numVal.val.f - mValInfo.numVal.step.f;
        if (newVal >= mValInfo.numVal.min.f)
            mValInfo.numVal.val.f = newVal;
        break;
    }
    case GameVarType::Bool:
        mValInfo.boolVal = !mValInfo.boolVal;
        break;
    default:
        PANIC("Invalid GameVarType: %d", mType);
        break;
    }
}




GameVarMgr::GameVarMgr()
{
    memset(mGameVars, 0, sizeof(GameVar) * kMaxGameVarCount);
}

GameVar * GameVarMgr::getByIndex(size_t idx)
{
    return reinterpret_cast<GameVar*>(&mGameVars[idx * sizeof(GameVar)]);
}

GameVarMgr::Iterator GameVarMgr::begin()
{
    return GameVarMgr::Iterator(*this, 0);
}

GameVarMgr::Iterator GameVarMgr::end()
{
    return GameVarMgr::Iterator(*this, mGameVarCount);
}

GameVar * GameVarMgr::nextGameVar()
{
    ASSERT(mGameVarCount < kMaxGameVarCount);
    GameVar * pGameVar = end().mpGameVar;
    mGameVarCount++;
    return pGameVar;
}

GameVar * GameVarMgr::findGameVar(const char * name)
{
    ASSERT(name);
    for (GameVar & gv : *this)
    {
        if (0 == strncmp(name, gv.name(), GameVar::kMaxNameLen-1))
            return &gv;
    }
    return nullptr;
}



} // namespace gaen
