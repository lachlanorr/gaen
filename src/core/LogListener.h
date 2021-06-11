//------------------------------------------------------------------------------
// LogListener.h - Convenience class to listen to UDP log messages
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

#ifndef GAEN_CORE_LOG_LISTENER_H
#define GAEN_CORE_LOG_LISTENER_H

#include <thread>

#include "core/platutils.h"

namespace gaen
{

typedef void (*LogListenerCB)(const char * time, const char * sev, const char * msg);

template <class CB>
class LogListener
{
public:

    LogListener(CB & cb)
      : mCb(cb)
      , mIsRunning(true)
      , mThread(&LogListener::listenProc, this)
    {
    }

    ~LogListener()
    {
        mIsRunning = false;
        mThread.join();
    }

private:
    void listenProc()
    {
        init_time();

        static const char * sLogSeverityText[] = {"ERR ",
                                                  "WARN",
                                                  "INFO"};

        static_assert(sizeof(sLogSeverityText) / sizeof(char*) == kLS_COUNT,
                      "sLogSeverityText should have the same number of entries as LogSeverity enum");

        Sock sock;

        if (!sock_create(&sock))
            PANIC("Unable to create socket");

        if (!sock_bind(sock, kLoggingPort))
            PANIC("Unable to bind socket");

        u8 buff[kMaxLogPacketSize];

        size_t recvSize;
        u32 fromIp;
        u16 fromPort;

        while(mIsRunning)
        {
            if (sock_select_read(sock, 250))
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

                mCb(timeStr, sLogSeverityText[pLogMessage->header.sev], pLogMessage->msg);
            }
        }
    }
    
    CB & mCb;
    bool mIsRunning;
    std::thread mThread;
    
}; // class LogListener


} // namespace gaen

#endif // #ifndef GAEN_CORE_LOG_LISTENER_H
