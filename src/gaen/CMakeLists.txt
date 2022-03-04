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


if(IOS)
  file(COPY
       ${CMAKE_CURRENT_SOURCE_DIR}/gaenios/gaenios.xcodeproj
       DESTINATION
       ${CMAKE_BINARY_DIR})
  execute_process(COMMAND ln -s ${CMAKE_CURRENT_SOURCE_DIR}/gaenios/gaenios ${CMAKE_BINARY_DIR}/gaenios)

else(IOS)

  set(gaen_SOURCES
    gaen.cpp
    gaen.h
    GaenPlatform.cpp
    GaenPlatform.h
    )

  if(USE_GLFW)
    list(APPEND gaen_SOURCES GaenPlatform_glfw.cpp)
  endif(USE_GLFW)

  if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # OSX
    list(APPEND gaen_SOURCES main_osx.mm)

    set(MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME})

    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
      ${gaen_SOURCES}
      )

    # Copy bundle to package dir
    add_custom_command(
      TARGET ${CMAKE_PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:gaen>/../../../${CMAKE_PROJECT_NAME}.app ${packaged_dir}/${CMAKE_PROJECT_NAME}.app
      )

  elseif(WIN32)
    list(APPEND gaen_SOURCES main_win32.cpp)
    add_executable(${CMAKE_PROJECT_NAME} WIN32
      ${gaen_SOURCES}
      )
    package("${CMAKE_PROJECT_NAME}")
  endif()

  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-l 127.0.0.1")

  source_group("" FILES ${gaen_SOURCES})

  target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${GLAD_INCLUDE}
  )

  target_link_libraries(${CMAKE_PROJECT_NAME}
    core
    hashes
    math
    assets
    engine
    render_support
    renderergl
    audio
    cara
    scripts
    ${PROJECT_ASSETS_LIB}
    ${PROJECT_LINK_LIBS}
    glfw
    ${PLATFORM_LINK_LIBS}
    )

endif()
