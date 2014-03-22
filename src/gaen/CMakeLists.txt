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


SET (gaen_SOURCES
  gaen.cpp
  gaen.h
  )


IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # OSX
  SET (gaen_SOURCES
    ${gaen_SOURCES}
    main_osx.mm
    )
  SET(MACOSX_BUNDLE_BUNDLE_NAME Gaen)
  ADD_EXECUTABLE(gaen MACOSX_BUNDLE
    ${gaen_SOURCES}
    )
  SET_TARGET_PROPERTIES(gaen PROPERTIES MACOSX_BUNDLE TRUE)
  # Copy bundle to package dir
  ADD_CUSTOM_COMMAND(
    TARGET gaen
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/gaen.app ${packaged_dir}/gaen.app
    )

ELSEIF (WIN32)
  SET (gaen_SOURCES
    ${gaen_SOURCES}
    main_win32.cpp
    )
  IDE_SOURCE_PROPERTIES( "/" "${gaen_SOURCES}")
  ADD_EXECUTABLE(gaen WIN32
    ${gaen_SOURCES}
    )
  package (gaen)
ENDIF ()

# LORRTODO - Make samples project only get added with a command switch. Don't add it by default.
TARGET_LINK_LIBRARIES (gaen
  core
  engine
  renderergl
  samples
  ${PLATFORM_LINK_LIBS}
  )

