#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2022 Lachlan Orr
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

set(gaen_core_SOURCES
  base_defines.cpp
  base_defines.h
  gamevars.cpp
  gamevars.h
  hashing.cpp
  hashing.h
  HashMap.h
  HashSet.h
  List.h
  log_message.h
  logging.cpp
  logging.h
  LogListener.h
  Map.h
  mem.cpp
  mem.h
  platutils.cpp
  platutils.h
  platutils_${platform}.cpp
  RefCounted.h
  Set.h
  sockets.cpp
  sockets.h
  sockets_${net_platform}.cpp
  SpscRingBuffer.h
  String.h
  threading.cpp
  threading.h
  thread_local.h
  Vector.h
  )

source_group("" FILES ${gaen_core_SOURCES})

add_library(gaen_core STATIC
  ${gaen_core_SOURCES}
  )

set_target_properties(gaen_core PROPERTIES
  FOLDER lib
  )

target_include_directories(gaen_core PUBLIC
  ${GAEN_SRC_INCLUDE}
  )
