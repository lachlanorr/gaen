//------------------------------------------------------------------------------
// main_gac.cpp - Gaen console program for logging and sending commands
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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "gaen/core/sockets.h"

#include "gaen/core/platutils.h"
#include "gaen/core/log_message.h"
#include "gaen/core/LogListener.h"

struct LogPrinter
{
    void operator()(const char * time, const char * sev, const char * msg)
    {
        printf("%s %s - %s\n", time, sev, msg);
    }
};

int main(int argc, char ** argv)
{
    using namespace gaen;

    init_sockets();

    bool shouldLogListen = false;
    
    // parse args
    for (int i = 1; i < argc; ++i)
    {
        if (0 == strcmp(argv[i], "-l"))
        {
            shouldLogListen = true;
        }
    }

    if (shouldLogListen)
    {
        LogPrinter logPrinter;
        LogListener<LogPrinter> ll(logPrinter);
        while (1)
        {
            sleep(1000);
        }
    }
}
