//------------------------------------------------------------------------------
// Program.h - Program class for compiled composed scripts
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

#ifndef GAEN_COMPOSE_PROGRAM_H
#define GAEN_COMPOSE_PROGRAM_H

#include <string>
#include <vector>

#include "corelib/base_types.h"
#include "corelib/cell_types.h"
#include "engine/component/Property.h"

namespace gaen
{

struct ProgramProperty
{
    std::string name;
    ePropertyType type;
    cell defaultValue[48];

    ProgramProperty(const char * name, ePropertyType type, const cell * defaultValue)
      : name(name)
      , type(type)
    {
        // LORRTODO - memcpy default value into defaultValue
    }
};


struct ProgramMessageHandler
{
    std::string name;
    u64 signature;
    cell * address;

    ProgramMessageHandler(const char * name, u64 signature, cell * address)
      : name(name)
      , signature(signature)
      , address(address) {}
};


class Program
{
public:
    // serialize program into buffer
    u8 * serialize(u8 * buffer, size_t size);
    static Program * deserialize(u8 * buffer, size_t size);

    void setUpdateHandler(cell * address);

    const ProgramProperty * addProperty(const char * name, ePropertyType type, const cell * defaultValue);
    const ProgramMessageHandler * addMessageHandler(const char * name, u64 signature, cell * address);

private:
    std::vector<cell> code_;
};


}


#endif

