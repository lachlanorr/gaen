#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
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


IF(IOS)
  FILE(COPY
       ${CMAKE_CURRENT_SOURCE_DIR}/gaenios/gaenios.xcodeproj
       DESTINATION
       ${CMAKE_BINARY_DIR})
  EXECUTE_PROCESS(COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/gaenios/gaenios ${CMAKE_BINARY_DIR}/gaenios)

ELSE(IOS)

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

    ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
      ${gaen_SOURCES}
      )

    # Copy bundle to package dir
    ADD_CUSTOM_COMMAND(
      TARGET ${CMAKE_PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:gaen>/../../../gaen.app ${packaged_dir}/gaen.app
      )

  ELSEIF (WIN32)
    SET (gaen_SOURCES
      ${gaen_SOURCES}
      main_win32.cpp
      )
    ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} WIN32
      ${gaen_SOURCES}
      )
    package ("${CMAKE_PROJECT_NAME}")
  ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IDE_SOURCE_PROPERTIES( "/" "${gaen_SOURCES}")

#--#  ADD_DEPENDENCIES(${CMAKE_PROJECT_NAME}
#--#    bullet
#--#    glfw
#--#    )

  TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME}
    core
    hashes
    math
    assets
    engine
    render_support
    renderergl
    scripts
    ${PROJECT_ASSETS_LIB}
    ${PROJECT_LINK_LIBS}
    ${PLATFORM_LINK_LIBS}
#--#    ${BULLET_LIBS}
#--#    ${GLFW_LIBS}
    )

  # Set debugging params for ALL_BUILD
  create_target_launcher(
    ${CMAKE_PROJECT_NAME}
    ARGS "-l 127.0.0.1"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )

ENDIF(IOS)
