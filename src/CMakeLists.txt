#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014 Lachlan Orr
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



# Standard include paths
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  INCLUDE_DIRECTORIES (${ext_dir}/opengl-4.3)
ENDIF()


# Packaging
SET (packaged_dir ${CMAKE_BINARY_DIR}/packaged)

EXECUTE_PROCESS (COMMAND ${CMAKE_COMMAND} -E make_directory ${packaged_dir})

MACRO (package target)
  GET_PROPERTY(target_location TARGET ${target} PROPERTY LOCATION)
  ADD_CUSTOM_COMMAND(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${target_location} ${packaged_dir}/
    )
ENDMACRO (package)


# Subdirs

#libs
ADD_SUBDIRECTORY (core)
ADD_SUBDIRECTORY (engine)
ADD_SUBDIRECTORY (renderergl)
ADD_SUBDIRECTORY (compose)
ADD_SUBDIRECTORY (gaen)

# If building gaen standalone (not part of a project),
# include our scripts. Otherwise, we assume project specific
# scripts are added by project's root CMakeLists.txt.
IF ("${CMAKE_SOURCE_DIR}/src" MATCHES "${CMAKE_CURRENT_SOURCE_DIR}")
  ADD_SUBDIRECTORY (scripts)
ENDIF()

#exes
IF (NOT IOS)
  ADD_SUBDIRECTORY (cmpc)
  ADD_SUBDIRECTORY (gac)
  ADD_SUBDIRECTORY (unit_tests)
ENDIF()
