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

set(BROTLI_DISABLE_TESTS OFF CACHE BOOL "" FORCE)
set(BROTLI_ OFF CACHE BOOL "" FORCE)
add_subdirectory(brotli)
configure_target_folders("brotli" FALSE)

set(BROTLIDEC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/brotli/c/include CACHE STRING "" FORCE)
set(BROTLIDEC_LIBRARIES $<TARGET_FILE:brotlidec-static> CACHE STRING "" FORCE)
