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

set(rapidjson_SOURCES
  rapidjson/include/rapidjson/allocators.h
  rapidjson/include/rapidjson/document.h
  rapidjson/include/rapidjson/encodedstream.h
  rapidjson/include/rapidjson/encodings.h
  rapidjson/include/rapidjson/error
  rapidjson/include/rapidjson/error/en.h
  rapidjson/include/rapidjson/error/error.h
  rapidjson/include/rapidjson/filereadstream.h
  rapidjson/include/rapidjson/filewritestream.h
  rapidjson/include/rapidjson/fwd.h
  rapidjson/include/rapidjson/internal
  rapidjson/include/rapidjson/internal/biginteger.h
  rapidjson/include/rapidjson/internal/diyfp.h
  rapidjson/include/rapidjson/internal/dtoa.h
  rapidjson/include/rapidjson/internal/ieee754.h
  rapidjson/include/rapidjson/internal/itoa.h
  rapidjson/include/rapidjson/internal/meta.h
  rapidjson/include/rapidjson/internal/pow10.h
  rapidjson/include/rapidjson/internal/regex.h
  rapidjson/include/rapidjson/internal/stack.h
  rapidjson/include/rapidjson/internal/strfunc.h
  rapidjson/include/rapidjson/internal/strtod.h
  rapidjson/include/rapidjson/internal/swap.h
  rapidjson/include/rapidjson/istreamwrapper.h
  rapidjson/include/rapidjson/memorybuffer.h
  rapidjson/include/rapidjson/memorystream.h
  rapidjson/include/rapidjson/msinttypes
  rapidjson/include/rapidjson/msinttypes/inttypes.h
  rapidjson/include/rapidjson/msinttypes/stdint.h
  rapidjson/include/rapidjson/ostreamwrapper.h
  rapidjson/include/rapidjson/pointer.h
  rapidjson/include/rapidjson/prettywriter.h
  rapidjson/include/rapidjson/rapidjson.h
  rapidjson/include/rapidjson/reader.h
  rapidjson/include/rapidjson/schema.h
  rapidjson/include/rapidjson/stream.h
  rapidjson/include/rapidjson/stringbuffer.h
  rapidjson/include/rapidjson/writer.h
)

source_group("" FILES ${rapidjson_SOURCES})

add_library(rapidjson INTERFACE
  ${rapidjson_SOURCES}
  )

target_include_directories(rapidjson INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include
  )

set_target_properties(rapidjson PROPERTIES
  FOLDER external/rapidjson
  )

configure_source_folders("${rapidjson_SOURCES}" "rapidjson/include/rapidjson/")
