//------------------------------------------------------------------------------
// test_gamevars.cpp - Tests for core/gamevars
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

#include <gtest/gtest.h>

#include "core/base_defines.h"
#include "core/gamevars.h"

GAMEVAR_REF_INT(TestVarInt);
GAMEVAR_REF_INT(TestVarInt1);
GAMEVAR_REF_INT(TestVarInt2);
GAMEVAR_REF_INT(TestVarInt3);

GAMEVAR_REF_INT(TestVarCommandSetInt);
GAMEVAR_REF_FLOAT(TestVarCommandSetFloat);
GAMEVAR_REF_BOOL(TestVarCommandSetBool);
TEST(GameVars, VerifyDeclared)
{
    // Test these vars declared in test_gamevars_aux.cpp:
    // 
    // GAMEVAR_DECL_INT(TestVarInt, 5, 4, 0, 10);
    // GAMEVAR_DECL_INT(TestVarInt1, 6, 3, -100, 10);
    // GAMEVAR_DECL_INT(TestVarInt2, 7, 2, 6, 10);
    // GAMEVAR_DECL_INT(TestVarInt3, 8, 1, 0, 10);

    EXPECT_EQ(TestVarInt, 5);
    gaen::incGameVal(TestVarInt);
    EXPECT_EQ(TestVarInt, 9);
    gaen::incGameVal(TestVarInt);
    EXPECT_EQ(TestVarInt, 9);
    gaen::decGameVal(TestVarInt);
    EXPECT_EQ(TestVarInt, 5);
    gaen::decGameVal(TestVarInt);
    EXPECT_EQ(TestVarInt, 1);
    gaen::decGameVal(TestVarInt);
    EXPECT_EQ(TestVarInt, 1);

    for (gaen::GameVar & gv : gaen::singleton<gaen::GameVarMgr>())
    {
        printf("%s\n", gv.name());
    }
}

TEST(GameVars, SetWithCommand)
{
    // Test these vars declared in test_gamevars_aux.cpp:
    // 
    // GAMEVAR_DECL_INT(TestVarCommandSetInt, 5, 4, -11, 10);
    // GAMEVAR_DECL_FLOAT(TestVarCommandSetFloat, 6.5f, 0.5f, -20.0f, 10.0f);
    // GAMEVAR_DECL_BOOL(TestVarCommandSetBool, false);

    // Null str
    EXPECT_FALSE(gaen::set_gamevar_with_command(nullptr));

    // Empty str
    EXPECT_FALSE(gaen::set_gamevar_with_command(""));

    // Unknown var
    EXPECT_FALSE(gaen::set_gamevar_with_command("ThisIsHopefullyNotDefinedAnyWhere=4"));
    
    // No equal, no val
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt="));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat="));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool="));

    // Out of range vals
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=-12"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=11"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=-22.0"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=11.0"));

    // Wrong types
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=-12.0"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=11.0"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=def"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetInt=true"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=abc"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=false"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=bool"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=1"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=TRUE"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=T"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=0"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=FALSE"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=F"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=fals"));
    EXPECT_FALSE(gaen::set_gamevar_with_command("TestVarCommandSetBool=tru"));

    // Verify no values have changed before we start messing with them
    EXPECT_EQ(TestVarCommandSetInt, 5);
    EXPECT_EQ(TestVarCommandSetFloat, 6.5);
    EXPECT_FALSE(TestVarCommandSetBool);

    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetInt=7"));
    EXPECT_EQ(TestVarCommandSetInt, 7);

    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=8.5"));
    EXPECT_EQ(TestVarCommandSetFloat, 8.5f);
    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetFloat=-20"));
    EXPECT_EQ(TestVarCommandSetFloat, -20.0f);

    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetBool=true"));
    EXPECT_TRUE(TestVarCommandSetBool);
    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetBool=false"));
    EXPECT_FALSE(TestVarCommandSetBool);
    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetBool=t"));
    EXPECT_TRUE(TestVarCommandSetBool);
    EXPECT_TRUE(gaen::set_gamevar_with_command("TestVarCommandSetBool=f"));
    EXPECT_FALSE(TestVarCommandSetBool);
}
