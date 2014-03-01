//------------------------------------------------------------------------------
// gaen.cpp - Gaen game app
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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "core/base_defines.h"
#include "core/gamevars.h"
#include "core/platutils.h"
#include "core/logging.h"
#include "core/threading.h"
#include "core/mem.h"

#include "engine/TaskMaster.h"

namespace gaen
{

static const char * kDefaultMemInitStr = "16:100,64:100,256:100,1024:100,4096:100";
static const size_t kMaxMemInitStrLen = 256;
static char sMemInitStr[kMaxMemInitStrLen] = {0};

static thread_id sNumThreads = core_count();

static const size_t kMaxIpLen = 16;
static char sLoggingServerIp[kMaxIpLen] = {0};
static bool sIsLoggingEnabled = false;


static const char * sHelpMsg =
    "Gaen Concurrency Engine"
    "\n"
    "Usage: gaen [options] [initializers]\n"
    "\n"
    "Options:\n"
    "  -h         Display this message\n"
    "  -t num     Set number of engine threads\n"
    "             Min:     1\n"
    "             Max:     num cores (this system has %d)\n"
    "             Default: num cores\n"
    "  -l ip      Enable logging and send logs to this IPV4 address\n"
    "  -m memStr  Initialize per-thread mempools to this string\n"
    "             Default: %s\n"
    "             String is comma separated list of colon separated tuples.\n"
    "             Each tuple has two values, allocation size and count.\n"
    "             Allocation sizes must be a multiple of 16.\n"
    "             Valid allocation sizes are between %d and %d, inclusive.\n"
    "             Valid counts are between %d and %d, inclusive.\n"
    "             E.g.: \"16:100,1024:200\" would declare two mem pools per thread,\n"
    "             a 16 byte with 100 count, and a 1024 byte with 200 count.\n"
    "             Allocation calls are serviced with the smallest pool possible,\n"
    "             and default to malloc/free if no pools are available.\n"
    "             When pool memory is freed, it is load balanced across threads,\n"
    "             applying the freed pool block to hungry threads first.\n"
    "\n"
    "Initializers: Override gamevar default values with the form gamevar=value\n"
    "              E.g.: godMode=true\n"
    "                    lifeCount=8\n"
    "                    regenRate=0.25\n"
    "\n";

    

//------------------------------------------------------------------------------
// Arg Parsing
//------------------------------------------------------------------------------
static void printHelpAndExit()
{
    printf(sHelpMsg,
           core_count(),
           kDefaultMemInitStr,
           kMinPoolAllocSize,
           kMaxPoolAllocSize,
           kMinPoolCount,
           kMaxPoolCount);
    exit(0);
}

static void parse_args(int argc,
                       char ** argv)
{
    strncpy(sMemInitStr, kDefaultMemInitStr, kMaxMemInitStrLen-1);

    // parse args
    for (size_t i = 1; i < static_cast<size_t>(argc); ++i)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'h':
                printHelpAndExit();
            case 't':
            {
                u32 numThreads = strtoul(argv[i+1], nullptr, 10);
                if (numThreads < kMinThreads ||
                    numThreads > core_count())
                {
                    printHelpAndExit();
                }
                sNumThreads = numThreads;
                ++i;
                break;
            }
            case 'l':
            {
                if (!is_valid_ip(argv[i+1]))
                    printHelpAndExit();
                ASSERT(strlen(argv[i+1]) < kMaxIpLen);
                strncpy(sLoggingServerIp, argv[i+1], kMaxIpLen-1);
                sLoggingServerIp[kMaxIpLen-1] = '\0';
                sIsLoggingEnabled = true;
                ++i;
                break;
            }
            case 'm':
            {
                if (!is_mem_init_str_valid(argv[i+1]))
                    printHelpAndExit();
                ASSERT(strlen(argv[i+1]) < kMaxMemInitStrLen);
                strncpy(sMemInitStr, argv[i+1], kMaxMemInitStrLen-1);
                sMemInitStr[kMaxMemInitStrLen-1] = '\0';
                ++i;
                break;
            }
            default:
                printHelpAndExit();
            }
        }
        else if (strchr(argv[i], '='))
        {
            if (!set_gamevar_with_command(argv[i]))
                printHelpAndExit();
        }
        else
        {
            printHelpAndExit();
        }
    }
}
//------------------------------------------------------------------------------
// Arg Parsing (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Systems Initialization
//------------------------------------------------------------------------------
void init_gaen(int argc, char ** argv)
{
    parse_args(argc,
               argv);

    init_time();
    init_sockets();

    if (sIsLoggingEnabled)
        init_logging(sLoggingServerIp);

    LOG_INFO("^^^^^^^^^^^^^^^^^^^^ GAEN STARTED ^^^^^^^^^^^^^^^^^^^^");

    init_threading(sNumThreads);

    init_memory_manager(sMemInitStr);

    init_task_masters<RendererType>();
}

void fin_gaen()
{
    fin_task_masters<RendererType>();

    fin_memory_manager();

    fin_threading();

    LOG_INFO("vvvvvvvvvvvvvvvvvvvv GAEN STOPPED vvvvvvvvvvvvvvvvvvvv");

    if (sIsLoggingEnabled)
        fin_logging();
}
//------------------------------------------------------------------------------
// Systems Initialization (END)
//------------------------------------------------------------------------------

} // namespace gaen


