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

SET (cmake_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET (ext_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET (python_dir ${CMAKE_CURRENT_SOURCE_DIR}/python)
SET (src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE (${cmake_dir}/core_defs.cmake)

ADD_SUBDIRECTORY(src)

