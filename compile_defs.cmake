#-------------------------------------------------------------------------------
# compile_defs.cmake - Compiler flags/definitions
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2022 Lachlan Orr
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

set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set these to avoid any external libs overwriting
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "")
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "")
set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "")

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
  add_compile_definitions(IS_BIG_ENDIAN)
else()
  add_compile_definitions(IS_LITTLE_ENDIAN)
endif()

add_compile_definitions(IS_COMPILER_$<CXX_COMPILER_ID>)
add_compile_definitions(IS_BUILD_$<CONFIG>=1)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(platform_ext "mm")
  if(IOS)
    add_compile_definitions(IS_PLATFORM_IOS)
    set(platform "ios")
  else()
    add_compile_definitions(IS_PLATFORM_OSX)
    set(platform "osx")

    find_library(COCOA_LIBRARY Cocoa)

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

    set(PLATFORM_LINK_LIBS
      ${COCOA_LIBRARY}
      ${OPENGL_LIBRARIES}
      )
  endif()
else()
  set(platform_ext "cpp")
endif()

if(WIN32)
  add_compile_definitions(IS_PLATFORM_WIN32)
  set(platform "win32")
  set(platform_ext "cpp")
  set(net_platform "win32")

  set(PLATFORM_LINK_LIBS
    Ws2_32
	)

  if (NOT DEFINED IS_HEADLESS)
	set(USE_GLFW ON)
	list(APPEND PLATFORM_LINK_LIBS
	  opengl32.lib
	  )
  endif()

  option(USE_MSVC_FAST_FLOATINGPOINT "Use MSVC /fp:fast option" ON)
  if(USE_MSVC_FAST_FLOATINGPOINT)
    add_definitions(/fp:fast)
  endif()
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  add_compile_definitions(_SCL_SECURE_NO_WARNINGS)
  add_compile_definitions(NOMINMAX)

elseif(UNIX)
  add_compile_definitions(IS_PLATFORM_POSIX)
  set(net_platform "posix")
  include(${cmake_dir}/posix.cmake)
endif()
