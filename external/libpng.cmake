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

option(PNG_SHARED "" OFF)
option(PNG_TESTS "" OFF)
set(SKIP_INSTALL_ALL TRUE)

set(ZLIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/zlib CACHE STRING "" FORCE)
set(ZLIB_USE_STATIC_LIBS TRUE CACHE STRING "" FORCE)

add_subdirectory(libpng)
configure_target_folders("libpng" FALSE)

target_include_directories(png_static INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/libpng
  ${CMAKE_CURRENT_BINARY_DIR}/libpng
  )
