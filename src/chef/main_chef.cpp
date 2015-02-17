//------------------------------------------------------------------------------
// main_chef.cpp - Asset cooking console app
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
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
#include "core/mem.h"

#include "chef/Chef.h"

//extern void cook_fnt(const char * platform, const char * raw_file);

namespace gaen
{

extern const bool reg;

void failure_cb(const char * msg)
{
    printf(msg);
    fin_memory_manager();
    exit(EXIT_FAILURE);
}

void print_usage_and_exit(int retcode)
{
    printf("Usage: chef [win|osx|ios] <raw_file>\n");
    fin_memory_manager();
    exit(retcode);
}

} // namespace gaen


int main(int argc, char ** argv)
{
    using namespace gaen;


    // Set up our failure callback so we can use PANIC, ASSERT, etc.
    // and have them print to stdout.
    set_report_failure_cb(failure_cb);

    static const char * kDefaultMemInitStr = "16:100,64:100,256:100,1024:100,4096:100";
    init_memory_manager(kDefaultMemInitStr);

    if (argc != 3)
        print_usage_and_exit(EXIT_FAILURE);

    const char * platform = argv[1];
    const char * raw_file = argv[2];

    if (!Chef::is_valid_platform(platform))
    {
        ERR("Invalid platform: %s", platform);
        print_usage_and_exit(EXIT_FAILURE);
    }

    Chef::cook(platform, raw_file);

    fin_memory_manager();
    return 0;
}
