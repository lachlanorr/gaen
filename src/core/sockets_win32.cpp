//------------------------------------------------------------------------------
// sockets_win32.cpp - Lightweight wrappers for Winsock sockets
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

#include "core/stdafx.h"

#include <winsock2.h>

#include "core/sockets.h"

namespace gaen
{

bool init_sockets()
{
    //-----------------------------------------------
    // Initialize Winsock
    WSADATA wsaData;
    i32 result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        ERR("WSAStartup failed with error %d\n", result);
        return false;
    }
    return true;
}

bool fin_sockets()
{
    i32 result = WSACleanup();
    if (result == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        ERR("WSACleanup failed, WSAGetLastError: %d, 0x%x", err, err);
        return false;
    }
    return true;
}

bool sock_create(Sock * pSock)
{
    *pSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*pSock == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        ERR("Unable to create socket, WSAGetLastError: %d, 0x%x", err, err);
        return false;
    }
    return true;
}

bool sock_close(Sock sock)
{
    int retval = closesocket(sock);

    if (retval == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        ERR("Unable to bind socket, WSAGetLastError: %d, 0x%x", err, err);
        return false;
    }
    return true;
}

static inline void prepSockAddr(sockaddr_in * pWinAddr, u32 ip, u16 port)
{
    pWinAddr->sin_family = AF_INET;
    pWinAddr->sin_port = port;
    pWinAddr->sin_addr.s_addr = ip;

    uint64_t * pZero = reinterpret_cast<uint64_t*>(&pWinAddr->sin_zero[0]);
    *pZero = 0;
}

bool sock_bind(Sock sock, u16 port)
{
    sockaddr_in winAddr;
    prepSockAddr(&winAddr, INADDR_ANY, port);
    
    int retval = bind(sock, (sockaddr*)&winAddr, sizeof(winAddr));

    if (retval == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        ERR("Unable to bind socket, WSAGetLastError: %d, 0x%x", err, err);
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
    prepSockAddr(&winAddr, ip, port);
    
    int sendSize = sendto(sock,
                          reinterpret_cast<const char*>(pData),
                          static_cast<int>(dataSize),
                          0,
                          reinterpret_cast<sockaddr*>(&winAddr),
                          sizeof(sockaddr_in));
    
    if (sendSize == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        // LORRNOTE: Don't log (ERR/ASSERT) here since our logging
        // depends on this code so errors can lead to infinite loops.
        return false;
    }
    if (sendSize != dataSize)
    {
        // LORRNOTE: Don't log (ERR/ASSERT) here since our logging
        // depends on this code so errors can lead to infinite loops.
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
    sockaddr_in winAddr;
    int winAddrSize = sizeof(sockaddr_in);

    *pRecvSize = recvfrom(sock,
                          reinterpret_cast<char*>(pData),
                          static_cast<int>(dataSize),
                          0,
                          reinterpret_cast<sockaddr*>(&winAddr),
                          &winAddrSize);

    if (*pRecvSize == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        // LORRNOTE: Don't log (ERR/ASSERT) here since our logging
        // depends on this code so errors can lead to infinite loops.
        return false;
    }
    *pFromIp = winAddr.sin_addr.s_addr;
    *pFromPort = winAddr.sin_port;

    return true;
}

} // namespace gaen
