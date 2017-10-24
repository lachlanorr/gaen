#!/usr/bin/env python

#-------------------------------------------------------------------------------
# udptest.py - Send a udp message to specified ip and port
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2017 Lachlan Orr
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#   1. The origin of this software must not be misrepresented; you must not
#   claim that you wrote the original software. If you use this software
#   in a product, an acknowledgment in the product documentation would be
#   appreciated but is not required.
#
#   2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.
#
#   3. This notice may not be removed or altered from any source
#   distribution.
#-------------------------------------------------------------------------------

import sys
import socket

UDP_IP = sys.argv[1]
UDP_PORT = eval(sys.argv[2])
MESSAGE = sys.argv[3]

print "Sending '%s' to %s:%d" % (MESSAGE, UDP_IP, UDP_PORT)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

