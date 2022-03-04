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

set(nanovg_SOURCES
  nanovg/src/fontstash.h
  nanovg/src/nanovg.c
  nanovg/src/nanovg_gl.h
  nanovg/src/nanovg_gl_utils.h
  nanovg/src/nanovg.h
  nanovg/src/stb_image.h
  nanovg/src/stb_truetype.h
)

source_group("" FILES ${nanovg_SOURCES})

add_library(nanovg STATIC
  ${nanovg_SOURCES}
  )

target_include_directories(nanovg PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/nanovg/src
  )

set_target_properties(nanovg PROPERTIES
  FOLDER external/nanovg
  )

configure_source_folders("${nanovg_SOURCES}" "nanovg/src/")
