//------------------------------------------------------------------------------
// gamevars.h - Support for managed global variables used for game settings
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2019 Lachlan Orr
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
//
// Two macros are provided for each type (int, float, and bool).
//
// The DECL macro declares the gamevar, and should be called exactly once
// in one of your .cpp files.
//
// The REF macro creates a reference, and can be placed in any other .cpp file
// to use the variable.
//
// Once properly declared, gamevars act like volatile global variables.
//
// All allocation is static and handled by GameVarMgr, so these can be set/read
// at any stage, including the earliest stages of initialization.
//------------------------------------------------------------------------------

#ifndef GAEN_CORE_GAMEVARS_H
#define GAEN_CORE_GAMEVARS_H

#include <cstdint>
#include <cfloat>
#include <utility>
#include <new>

#include "core/base_defines.h"

namespace gaen
{

// LORRTODO - These all need to be changed to use atomics... will require some class redesign

// Optional params are Step, Min, and Max
#define GAMEVAR_DECL_INT(varName, val, ...) \
volatile gaen::i32 & varName = gaen::singleton<gaen::GameVarMgr>().allocGameVar(#varName, val, ##__VA_ARGS__).intRef();
#define GAMEVAR_REF_INT(varName) \
extern volatile gaen::i32 & varName;

// Optional params are Step, Min, and Max
#define GAMEVAR_DECL_FLOAT(varName, val, ...) \
volatile gaen::f32 & varName = gaen::singleton<gaen::GameVarMgr>().allocGameVar(#varName, val, ##__VA_ARGS__).floatRef();
#define GAMEVAR_REF_FLOAT(varName) \
extern volatile gaen::f32 & varName;

#define GAMEVAR_DECL_BOOL(varName, val) \
volatile bool & varName = gaen::singleton<gaen::GameVarMgr>().allocGameVar(#varName, val).boolRef();
#define GAMEVAR_REF_BOOL(varName) \
extern volatile bool & varName;

enum class GameVarType : u8
{
    Int,
    Float,
    Bool
};

bool set_gamevar_with_command(const char * command);

class GameVar
{
public:
    static const size_t kMaxNameLen = 31;

    GameVar(const char * name, i32 val, i32 step=1,   i32 min=INT32_MIN, i32 max=INT32_MAX);
    GameVar(const char * name, f32 val, f32 step=1.0f, f32 min=-FLT_MAX, f32 max=FLT_MAX);
    GameVar(const char * name, bool val);

    const char * name() const { return mName; }
    GameVarType type() const { return mType; }

    volatile i32 & intRef() volatile;
    volatile f32 & floatRef() volatile;
    volatile bool & boolRef() volatile;

    void setVal(i32 val) volatile;
    void setVal(f32 val) volatile;
    void setVal(bool val) volatile;

    bool isValidVal(i32 val) volatile;
    bool isValidVal(f32 val) volatile;
    bool isValidVal(bool val) volatile;

    void incVal() volatile;
    void decVal() volatile;

private:
    void setName(const char * name);

    // NOTE: Val must be first data member.
    // We cast its address back into the GameVar, so
    // don't every put stuff before this union.
    union valInfo_
    {
        struct numVal_
        {
            union val_
            {
                i32 i;
                f32 f;
            } val;

            union step_
            {
                i32 i;
                f32 f;
            } step;

            union min_
            {
                i32 i;
                f32 f;
            } min;

            union max_
            {
                i32 i;
                f32 f;
            } max;
        } numVal;

        bool boolVal;

    } mValInfo;

    char mName[kMaxNameLen];

    GameVarType mType;
};

// Sanity check the size.
// We're going to declare a whole bunch of these statically in
// GameVarMgr, so just want to know what we're doing space wise and
// not get any suprises.
// NOTE: If this ever changes, keep it a multiple of 16.
static_assert(sizeof(GameVar) == 48, "GameVar not expected size of 48");


class GameVarMgr
{
public:
    class Iterator
    {
        friend class GameVarMgr;
    public:
        Iterator(GameVarMgr & gameVarMgr, size_t initIdx)
        {
            mpGameVar = gameVarMgr.getByIndex(initIdx);
        }

        Iterator& operator++()
        {
            ++mpGameVar;
            return *this;
        }

        bool operator!=(const Iterator & rhs)
        {
            return mpGameVar != rhs.mpGameVar;
        }

        GameVar & operator*()
        {
            return *mpGameVar;
        }
    private:
        GameVar * mpGameVar;
    };
    friend class Iterator;

    GameVarMgr();

    size_t gameVarCount() const { return mGameVarCount; }
    size_t maxGameVarCount() const { return kMaxGameVarCount; }

    template<typename T, class... Args>
    GameVar & allocGameVar(const char * name, T val, Args&&... args)
    {
        ASSERT(!findGameVar(name));
        GameVar * pGameVar = nextGameVar();
        ASSERT(pGameVar);
        pGameVar = new (pGameVar) GameVar(name, val, std::forward<Args>(args)...);
        return *pGameVar;
    }

    template<typename T>
    void incGameVarVal(volatile T & val)
    {
        volatile GameVar * pGV = fromValAddress(val);
        pGV->incVal();
    }

    template<typename T>
    void decGameVarVal(volatile T & val)
    {
        volatile GameVar * pGV = fromValAddress(val);
        pGV->decVal();
    }

    Iterator begin();
    Iterator end();

    GameVar * findGameVar(const char * name);

private:
    GameVar * nextGameVar();

    template<typename T>
    volatile GameVar * fromValAddress(volatile T & val)
    {
        volatile GameVar * pGV = reinterpret_cast<volatile GameVar*>(&val);
        ASSERT(pGV >= begin().mpGameVar && pGV < end().mpGameVar);
        return pGV;
    }

    GameVar * getByIndex(size_t idx);

    static const size_t kMaxGameVarCount = 256;
    u8 mGameVars[kMaxGameVarCount * sizeof(GameVar)];
    size_t mGameVarCount = 0;
};

template<typename T>
void incGameVal(T & val)
{
    singleton<GameVarMgr>().incGameVarVal(val);
}

template<typename T>
void decGameVal(T & val)
{
    singleton<GameVarMgr>().decGameVarVal(val);
}

} // namespace gaen

#endif // #ifndef GAEN_CORE_GAMEVARS_H
