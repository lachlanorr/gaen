//------------------------------------------------------------------------------
// sockets.cpp - Platform generic sockets functions
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

#include "core/stdafx.h"

#if IS_PLATFORM_WIN32
// LORRTODO: Need to work out the IP6 solution with inet_notoa nad inet_addr below.
// For now, pretty low priority.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#elif IS_PLATFORM_POSIX // #if IS_PLATFORM_WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else  // #elif IS_PLATFORM_POSIX
#error Unsupported C++ compiler
#endif // #if IS_PLATFORM_WIN32

#include "core/sockets.h"

namespace gaen
{

const u16 kLoggingPort = htons(0x6AE1);

const char* ip_to_str(u32 ip)
{
    in_addr & addr = *reinterpret_cast<in_addr*>(&ip);
    return inet_ntoa(addr);
}

u32 str_to_ip(const char * ipStr)
{
    return inet_addr(ipStr);
}

bool is_valid_ip(const char * ipStr)
{
    return inet_addr(ipStr) != (u32)-1;
}

} // namespace gaen
