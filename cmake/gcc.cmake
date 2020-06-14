#-------------------------------------------------------------------------------
# gcc.cmake - GCC specific settings used in CMakeLists.txt files
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2020 Lachlan Orr
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

# Initialize CXXFLAGS.
SET(CMAKE_CXX_FLAGS                "-Wall -std=c++11 -fno-exceptions -framework Cocoa")
SET(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -gdwarf-3 -fno-inline-small-functions")
#SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -gdwarf-3")

# These are some definitions that will be available to the code to
# check for platform and build type.
ADD_DEFINITIONS("-DIS_BUILD_${CMAKE_BUILD_TYPE}=1")

# Compiler-specific C++11 activation.
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    EXECUTE_PROCESS(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    IF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        MESSAGE (FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    ENDIF ()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
ELSE ()
    MESSAGE (FATAL_ERROR "Your C++ compiler does not support C++11.")
ENDIF ()

