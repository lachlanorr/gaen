#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2021 Lachlan Orr
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

set(zlib_SOURCES
  zlib/adler32.c
  zlib/compress.c
  zlib/crc32.c
  zlib/crc32.h
  zlib/deflate.c
  zlib/deflate.h
  zlib/gzclose.c
  zlib/gzguts.h
  zlib/gzlib.c
  zlib/gzread.c
  zlib/gzwrite.c
  zlib/infback.c
  zlib/inffast.c
  zlib/inffast.h
  zlib/inffixed.h
  zlib/inflate.c
  zlib/inflate.h
  zlib/inftrees.c
  zlib/inftrees.h
  zlib/trees.c
  zlib/trees.h
  zlib/uncompr.c
  zlib/zlib.h
  zlib/zutil.c
  zlib/zutil.h
  zlib/zconf.h
  )

source_group("" FILES ${zlib_SOURCES})

add_library(zlibstatic STATIC
  ${zlib_SOURCES}
  )

target_include_directories(zlibstatic PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/zlib
  )

set_target_properties(zlibstatic PROPERTIES
  FOLDER external/zlib
  )

set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib CACHE STRING "" FORCE)
set(ZLIB_LIBRARY $<TARGET_LINKER_FILE:zlibstatic> CACHE STRING "" FORCE)
set(ZLIB_LIBRARIES $<TARGET_LINKER_FILE:zlibstatic> CACHE STRING "" FORCE)
set(ZLIB_FOUND TRUE CACHE STRING "" FORCE)
