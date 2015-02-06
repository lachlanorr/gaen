//------------------------------------------------------------------------------
// main_gac.cpp - Gaen console program for logging and sending commands
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

#include "core/sockets.h"

#include "core/platutils.h"
#include "core/log_message.h"

namespace gaen
{

static const char * sLogSeverityText[] = {"ERR ",
                                          "WARN",
                                          "INFO"};

static_assert(sizeof(sLogSeverityText) / sizeof(char*) == kLS_COUNT,
              "sLogSeverityText should have the same number of entries as LogSeverity enum");


void log_listen_and_print()
{
    // LORRTODO - Make this multithreaded.  Recv thread should be as lightweight as possible and
    // put inbounds on an spsc queue.  Consider using the MemMgr here as well.  Logging must never
    // become a bottleneck, nor do we want to drop logs.

    init_sockets();

    Sock sock;

    if (!sock_create(&sock))
        PANIC("Unable to create socket");

    if (!sock_bind(sock, kLoggingPort))
        PANIC("Unable to bind socket");


    u8 buff[kMaxLogPacketSize];

    size_t recvSize;
    u32 fromIp;
    u16 fromPort;

    while(1)
    {
        if (!sock_recvfrom(sock, buff, kMaxLogPacketSize-1, &recvSize, &fromIp, &fromPort))
        {
            puts("LOGERR: Failed to recv");
            exit(-1);
        }

        LogMessage * pLogMessage = reinterpret_cast<LogMessage*>(buff);
        pLogMessage->msg[kMaxLogMessageSize-1] = '\0'; // just in case we were sent junk

        char timeStr[32];
        if (!time_to_str(timeStr, 32, pLogMessage->header.time))
        {
            strcpy(timeStr, "????");
        }

        printf("%s %s - %s\n",
               timeStr,
               sLogSeverityText[pLogMessage->header.sev],
               pLogMessage->msg);
    }
}

} // namespace gaen

int main(int argc, char ** argv)
{
    using namespace gaen;
    
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
        log_listen_and_print();
}
