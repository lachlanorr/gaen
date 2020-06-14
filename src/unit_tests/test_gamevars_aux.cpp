//------------------------------------------------------------------------------
// test_gamevars_aux.cpp - Definitions of gamevars to test in test_gamevars
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#include "core/gamevars.h"

// Declare some gamevars here so we can test it from test_gamvars.cpp
GAMEVAR_DECL_INT(TestVarInt, 5, 4, 0, 10);
GAMEVAR_DECL_INT(TestVarInt1, 6, 3, -100, 10);
GAMEVAR_DECL_INT(TestVarInt2, 7, 2, 6, 10);
GAMEVAR_DECL_INT(TestVarInt3, 8, 1, 0, 10);


GAMEVAR_DECL_INT(TestVarCommandSetInt, 5, 4, -11, 10);
GAMEVAR_DECL_FLOAT(TestVarCommandSetFloat, 6.5f, 0.5f, -20.0f, 10.0f);
GAMEVAR_DECL_BOOL(TestVarCommandSetBool, false);
