//------------------------------------------------------------------------------
// logging.h - Support for logging over udp to console listener
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_CORE_LOGGING_H
#define GAEN_CORE_LOGGING_H

#include "core/base_defines.h"
#include "core/sockets.h"
#include "core/log_message.h"

namespace gaen
{

#define LOGGING WHEN(HAS(DEV_BUILD))

#define LOG_ERROR(format, ...)   LOG(kLS_Error,    format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG(kLS_Warning,  format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)    LOG(kLS_Info,     format, ##__VA_ARGS__)

#if HAS(DEV_BUILD)
#define LOG(severity, format, ...) gaen::logf(severity, format, ##__VA_ARGS__)
#else  // #if HAS(DEV_BUILD)
#define LOG(severity, format, ...) do {} while(0)
#endif // #if HAS(DEV_BUILD)

void init_logging(const char * serverIp);
void fin_logging();


#if HAS(LOGGING)    
class Logger
{
public:
    void init(u32 serverIp);
    void fin();
    void log(LogSeverity severity, const char * message);

    bool isInit() { return mIsInit; }

private:
    Sock mSock;
    u32 mServerIp;
    bool mIsInit = false;
};

void logf(LogSeverity severity, const char * format, ...);
#endif // #if HAS(LOGGING)


} // namespace gaen

#endif // #ifndef GAEN_CORE_LOGGING_H
