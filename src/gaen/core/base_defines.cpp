//------------------------------------------------------------------------------
// base_defines.cpp - Commonly used definitions used throughout Gaen
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

#include "gaen/core/stdafx.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <csignal>
#include <cstdlib>

#include "gaen/core/base_defines.h"
#include "gaen/core/thread_local.h"

namespace gaen
{

// max size for formatted failure message
static const size_t kMaxMessageSize = 512;
// max total size of failure message
static const size_t kMaxFailureMessageSize = kMaxMessageSize + 256;

// If none is set, we'll just printf
static ReportFailureCB sReportFailureCB = nullptr;

static DebugBreakFunc sDebugBreakOverride = nullptr;
static AbortFunc sAbortOverride = nullptr;

void debug_break_gaen()
{
    if (!sDebugBreakOverride)
    {
#if IS_PLATFORM_POSIX
        raise(SIGTRAP);
#elif IS_COMPILER_MSVC // #if IS_PLATFORM_POSIX
        __debugbreak();
#endif // #elif IS_COMPILER_MSVC
    }
    else
        sDebugBreakOverride();
}

void abort_gaen()
{
    if (!sAbortOverride)
        abort();
    else
        sAbortOverride();
}

void override_failure_funcs(DebugBreakFunc debugBreakFunc, AbortFunc abortFunc)
{
    // LORRTODO - to support unit tests, implement this so we don't abort on asserts/panics
}

void set_report_failure_cb(ReportFailureCB reportFailureCB)
{
    sReportFailureCB = reportFailureCB;
}

// strip off all the '..\..\..\' stuff at the start of
// filenames.
const char * strip_dotdots(const char * file)
{
    if (!file)
        return file;
    const char * p = file;
    while (*p == '.' || *p == '\\' || *p == '/')
        ++p;
    return p;
}

void report_failure(const char * condition,
                    const char * file,
                    int line,
                    const char * format,
                    ...)
{
    file = strip_dotdots(file);

// Asserts not working nicely on iOS... pending investigation,
// but for now just print. Feels like college.
#if IS_PLATFORM_IOS
    printf("FAILURE: %s(%d) - %s\n", file, line, condition);
#endif

    // Thread storage to print our message.
    TLARRAY(char, tMessage, kMaxMessageSize);
    TLARRAY(char, tFailureMessage, kMaxFailureMessageSize);

    va_list argptr;
    va_start(argptr, format);

    if (format && format[0] != '\0')
    {
#if !IS_PLATFORM_IOS
        int ret = vsnprintf(tMessage, kMaxMessageSize-1, format, argptr);
#else
        // LORRTODO - Implement replacement for vsnprintf on ios, or wait for Apple to catch up to C++11
        int ret = std::vsprintf(tMessage, format, argptr);
#endif

        if (ret <= 0)
        {
            std::strncpy(tMessage, "report_failure was unable to format message", kMaxMessageSize-1);
        }
    }
    else
        tMessage[0] = '\0';

    va_end(argptr);

    // Ensure that we're null terminated in cases where someone sent
    // us a stupidly long message.
    tMessage[kMaxMessageSize-1] = '\0';


    if (tMessage[0] != '\0')
    {
        snprintf(tFailureMessage,
                 kMaxFailureMessageSize-1,
                 "%s: %s:%d - %s",
                 condition,
                 file,
                 line,
                 (const char*)tMessage);
    }
    else
    {
        snprintf(tFailureMessage,
                 kMaxFailureMessageSize-1,
                 "%s: %s:%d",
                 condition,
                 file,
                 line);
    }
    tFailureMessage[kMaxFailureMessageSize-1] = '\0';

    if (sReportFailureCB)
        sReportFailureCB(tFailureMessage);
    else
        puts(tFailureMessage);

}


} // namespace gaen
