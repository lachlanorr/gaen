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

set(bzip2_SOURCES
  bzip2/blocksort.c
  bzip2/huffman.c
  bzip2/crctable.c
  bzip2/randtable.c
  bzip2/compress.c
  bzip2/decompress.c
  bzip2/bzlib.c
  bzip2/bzlib.h
  bzip2/bzlib_private.h
  )

set(BZ_VERSION "1.0.7")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/bzip2/bz_version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/bzip2/include/bz_version.h
  )

list(APPEND bzip2_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/bzip2/include/bz_version.h")

source_group("" FILES ${bzip2_SOURCES})

add_library(bzip2static STATIC
  ${bzip2_SOURCES}
  )

if(WIN32)
  target_compile_definitions(bzip2static
    PRIVATE BZ_LCCWIN32
    )
else()
  target_compile_definitions(bzip2static
    PRIVATE BZ_UNIX
    )
endif()

target_include_directories(bzip2static PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bzip2
  ${CMAKE_CURRENT_BINARY_DIR}/bzip2/include
  )

target_include_directories(bzip2static SYSTEM INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/bzip2
  ${CMAKE_CURRENT_BINARY_DIR}/bzip2/include
  )

set_target_properties(bzip2static PROPERTIES
  FOLDER external/bzip2
  )
