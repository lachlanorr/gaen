//------------------------------------------------------------------------------
// sockets.h - Lightweight wrappers for posix and winsock32 sockets
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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

#ifndef GAEN_CORE_SOCKETS_H
#define GAEN_CORE_SOCKETS_H

#include "core/base_defines.h"

namespace gaen
{

extern const u16 kLoggingPort;
//static u16 kConsolePort = 0x6AEC;
//static u16 kProfilePort = 0x6AEF;

#if IS_PLATFORM_WIN32
typedef uintptr_t Sock;
#elif IS_PLATFORM_POSIX // #if IS_PLATFORM_WIN32
typedef int Sock;
#else  // #elif IS_PLATFORM_POSIX
#error Unsupported C++ compiler
#endif // #if IS_PLATFORM_WIN32

bool init_sockets();
bool fin_sockets();

bool sock_create(Sock * pSock);
bool sock_close(Sock sock);
bool sock_bind(Sock sock, u16 port);

bool sock_sendto(Sock sock,
                 const u8 * pData,
                 size_t dataSize,
                 u32 ip,
                 u16 port);

bool sock_recvfrom(Sock sock,
                   u8 * pData,
                   size_t pDataSize,
                   size_t * pRecvSize,
                   u32 * pFromIp,
                   u16 * pFromPort);

bool sock_select_read(Sock sock,
                      u32 timeout_ms);

const char* ip_to_str(u32 ip);
u32 str_to_ip(const char * ipStr);
bool is_valid_ip(const char * ipStr);

} // namespace gaen


#endif // #ifndef GAEN_CORE_SOCKETS_H
