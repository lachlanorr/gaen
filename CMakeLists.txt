#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2016 Lachlan Orr
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

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

PROJECT (gaen)

CMAKE_POLICY(SET CMP0054 NEW)
CMAKE_POLICY(SET CMP0026 OLD)

SET (cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET (ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET (python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)
SET (src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE (${cmake_dir}/core_defs.cmake)

#-------------------------------------------------------------------------------
# External dependencies
#-------------------------------------------------------------------------------
INCLUDE (ExternalProject)

# FreeType
SET (FREETYPE_VER "freetype-2.6.3")
SET (FREETYPE_PATH "${CMAKE_BINARY_DIR}/external/${FREETYPE_VER}")
ExternalProject_Add(freetype
  URL "http://download.savannah.gnu.org/releases/freetype/${FREETYPE_VER}.tar.gz"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/freetype
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_PATH}
)
INCLUDE_DIRECTORIES (${FREETYPE_PATH}/include)
SET (FREETYPE_LIB "${FREETYPE_PATH}/lib/freetype.lib")
SET (FREETYPE_LIBD "${FREETYPE_PATH}/lib/freetyped.lib")
#-------------------------------------------------------------------------------
# External dependencies (END)
#-------------------------------------------------------------------------------

ADD_SUBDIRECTORY(src)

