//------------------------------------------------------------------------------
// log_message.h - Shared declarations of log message struct
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

#ifndef GAEN_CORE_LOG_MESSAGE_H
#define GAEN_CORE_LOG_MESSAGE_H

#include "gaen/core/base_defines.h"

namespace gaen
{

enum LogSeverity : u8
{
    kLS_Error   = 0,
    kLS_Warning = 1,
    kLS_Info    = 2,

    kLS_COUNT
};

struct LogMessageHeader
{
    f32 time;
    LogSeverity sev;
};

static const size_t kMaxLogPacketSize  = 768; // safe size to udp... we could probably go higher, but this is reasonable for log messages.
static const size_t kMaxLogMessageSize = kMaxLogPacketSize - sizeof(LogMessageHeader);

struct LogMessage
{
    LogMessageHeader header;
    char msg[kMaxLogMessageSize];
};

} // namespace gaen

#endif // #ifndef GAEN_CORE_LOG_MESSAGE_H
