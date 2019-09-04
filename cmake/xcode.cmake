#-------------------------------------------------------------------------------
# xcode.cmake - Xcode specific settings used in CMakeLists.txt files
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2019 Lachlan Orr
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

SET (platform_ext "mm")

MACRO (IDE_SOURCE_PROPERTIES source_path sources)
  STRING (REPLACE "/" "\\\\" source_group_path ${source_path} )
  SOURCE_GROUP (${source_group_path} FILES ${sources})
ENDMACRO (IDE_SOURCE_PROPERTIES NAME sources)

#SET(CMAKE_CXX_FLAGS                "-Wall -std=c++11 -fno-exceptions -framework Cocoa")
SET(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -DIS_BUILD_Debug=1")
#SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -DIS_BUILD_Release=1")

SET (CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
