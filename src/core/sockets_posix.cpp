//------------------------------------------------------------------------------
// sockets_posix.cpp - Lightweight wrappers for posix sockets
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

#include <errno.h>
#include <sys/un.h>
#include <unistd.h>

#include "core/sockets.h"

namespace gaen
{

bool init_sockets()
{
    // Unecessary to initialize, as it is on Win32
    return true;
}

bool sock_create(Sock * pSock)
{
    *pSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*pSock == -1)
    {
        ERR("Unable to create socket, errno: %d, 0x%x", errno, errno);
        return false;
    }
    return true;
}

bool sock_close(Sock sock)
{
    int retval = close(sock);

    if (retval == -1)
    {
        ERR("Unable to bind socket, errno: %d, 0x%x", errno, errno);
        return false;
    }
    return true;
}

static inline void prep_sock_addr(struct sockaddr_in * pAddr, u32 ip, u16 port)
{
    pAddr->sin_family = AF_INET;
    pAddr->sin_port = port;
    pAddr->sin_addr.s_addr = ip;

    uint64_t * pZero = reinterpret_cast<uint64_t*>(&pAddr->sin_zero[0]);
    *pZero = 0;
}

bool sock_bind(Sock sock, u16 port)
{
    sockaddr_in winAddr;
    prep_sock_addr(&winAddr, INADDR_ANY, port);
    
    int retval = bind(sock, (sockaddr*)&winAddr, sizeof(winAddr));

    if (retval == -1)
    {
        ERR("Unable to bind socket, errno: %d, 0x%x", errno, errno);
        return false;
    }
    return true;
}

bool sock_sendto(Sock sock,
                 const u8 * pData,
                 size_t dataSize,
                 u32 ip,
                 u16 port)
{
    sockaddr_in winAddr;
    prep_sock_addr(&winAddr, ip, port);
    
    int sendSize = (int)sendto(sock,
                               reinterpret_cast<const char*>(pData),
                               dataSize,
                               0,
                               reinterpret_cast<sockaddr*>(&winAddr),
                               sizeof(sockaddr_in));
    
    if (sendSize == -1)
    {
        ERR("Unable to send on socket, errno: %d, %x", errno, errno);
        return false;
    }
    if (static_cast<size_t>(sendSize) != dataSize)
    {
        ERR("Unable to send entire packet on socket, sent/attempted = %d/%d", sendSize, dataSize);
        return false;
    }
    return true;
}

bool sock_recvfrom(Sock sock,
                   u8 * pData,
                   size_t dataSize,
                   size_t * pRecvSize,
                   u32 * pFromIp,
                   u16 * pFromPort)
{
    sockaddr_in sockAddr;
    socklen_t sockAddrSize = sizeof(sockaddr_in);

    int recvSize = (int)recvfrom(sock,
                                 reinterpret_cast<char*>(pData),
                                 dataSize,
                                 0,
                                 reinterpret_cast<sockaddr*>(&sockAddr),
                                 &sockAddrSize);

    if (recvSize == -1)
    {
        ERR("Failure during recv, errno: %d, %x", errno, errno);
        return false;
    }

    *pRecvSize = static_cast<size_t>(recvSize);
    *pFromIp = sockAddr.sin_addr.s_addr;
    *pFromPort = sockAddr.sin_port;

    return true;
}

} // namespace gaen
