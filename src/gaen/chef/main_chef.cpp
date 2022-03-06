//------------------------------------------------------------------------------
// main_chef.cpp - Asset cooking console app
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

#include "gaen/chef/stdafx.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"
#include "gaen/core/Vector.h"
#include "gaen/core/threading.h"
#include "gaen/core/SpscRingBuffer.h"

#include "gaen/assets/file_utils.h"

#include "gaen/chef/Chef.h"
#include "gaen/chef/CookerRegistry.h"
#include "gaen/chef/cookers.h"

namespace gaen
{

typedef List<kMEM_Chef, UniquePtr<CookInfo>> CookList;

struct RecurseContext
{
    Chef & chef;
    CookList & cookList;
    bool force;

    RecurseContext(Chef & chef, CookList & cookList, bool force)
      : chef(chef)
      , cookList(cookList)
      , force(force)
    {}
};

void failure_cb(const char * msg)
{
    printf("%s\n", msg);
}

void print_usage_and_exit(int retcode = 1)
{
    printf("Usage: chef [-f] [-p win|osx|ios] [-t threads] [path]\n");
    fin_memory_manager();
    exit(retcode);
}

void chef_thread(SpscRingBuffer<CookInfo*> & spsc, const char * platform, const char * assetsDir, bool force)
{
    Chef chef(active_thread_id(), platform, assetsDir);
}

void recurse_dir_cb(const char * path, void * context)
{
    RecurseContext * pRc = static_cast<RecurseContext*>(context);

    UniquePtr<CookInfo> pCi = pRc->chef.prepCookInfo(path, pRc->force);
    if (pCi.get() && pRc->chef.shouldCook(*pCi))
    {
        pRc->cookList.push_back(std::move(pCi));
    }
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

    // find assets dir based on where our process is running out of
    char assetsDir[kMaxPath+1];
    find_assets_cooking_dir(assetsDir);

    char assetsRawDir[kMaxPath+1];
    assets_raw_dir(assetsRawDir, assetsDir);

    // ----------------
    // parse arguments

    // default path is root of raw assets
    char path[kMaxPath+1];
    strcpy(path, assetsRawDir);

    // assume active platform based on what we're running on
    char platform[kMaxPath+1];
    strcpy(platform, default_platform());

    bool force = false;
    u32 threadCount = platform_core_count();
    u32 maxThreadCount = platform_core_count() * 4;

    bool hasInputPath = false;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'f':
                force = true;
                break;
            case 'p':
                if (i == argc-1)
                    print_usage_and_exit();
                if (!is_valid_platform(argv[i+1]))
                {
                    ERR("Invalid platform: %s", argv[i+1]);
                    print_usage_and_exit();
                }
                strcpy(platform, argv[i+1]);
                i++;
                break;
            case 't':
                if (i == argc-1)
                    print_usage_and_exit();
                threadCount = strtoul(argv[i+1], nullptr, 10);
                if (threadCount == 0)
                    print_usage_and_exit();
                if (threadCount > maxThreadCount)
                {
                    ERR("Overriding '-t %u' option and only using %u threads\n", threadCount, maxThreadCount);
                    threadCount = maxThreadCount;
                }
                i++;
                break;
            default:
                break;
            }
        }
        else if (!hasInputPath)
        {
            full_path(path, argv[i]);
            if (strstr(path, assetsRawDir) != path)
            {
                // prepend our raw directory
                strcpy(path, assetsRawDir);
                append_path(path, argv[i]);
                normalize_path(path);
            }
            hasInputPath = true;
        }
        else
        {
            ERR("Invalid option: %s", argv[i]);
            print_usage_and_exit();
        }
    }
    // parse arguments (END)
    // -----------------

    // Register any cookers defined in gaen
    register_cookers();

    Chef chef(1, platform, assetsDir);

    if (dir_exists(path))
    {
        CookList cookList;
        RecurseContext rc(chef, cookList, force);

        // if it's a directory, cook all recursively
        recurse_dir(path, &rc, recurse_dir_cb);

        if (rc.cookList.size() > 0)
        {
            printf("Cooking %u files...\n", (u32)rc.cookList.size());

            for (const UniquePtr<CookInfo> & pCi : cookList)
            {
                // Already checked that these need cooking when recursing directory
                chef.forceCookAndWrite(pCi.get());
            }
        }
    }
    else if (file_exists(path))
    {
        chef.cook(path, force);
    }
    else
    {
        ERR("Cook path not found: %s", path);
        print_usage_and_exit();
    }

    fin_memory_manager();
    return 0;
}
