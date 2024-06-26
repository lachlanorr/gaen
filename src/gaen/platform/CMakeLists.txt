#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
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

set(gaen_platform_SOURCES
  gaen.cpp
  gaen.h
  GaenPlatform.cpp
  GaenPlatform.h
  )

if(DEFINED IS_HEADLESS)
  list(APPEND gaen_platform_SOURCES GaenPlatform_headless.cpp)
elseif(USE_GLFW)
  list(APPEND gaen_platform_SOURCES GaenPlatform_glfw.cpp)
endif()

source_group("" FILES ${gaen_platform_SOURCES})

add_library(gaen_platform STATIC
  ${gaen_platform_SOURCES}
  )

set_target_properties(gaen_platform PROPERTIES
  FOLDER lib
  )


set(PLATFORM_LIB_LINK_LIBS
  gaen_render_support
  )

if (NOT DEFINED IS_HEADLESS)
  list(APPEND PLATFORM_LIB_LINK_LIBS
	gaen_renderergl
	)
endif()

list(APPEND PLATFORM_LIB_LINK_LIBS
  gaen_assets
  gaen_engine
  )

if (NOT DEFINED IS_HEADLESS)
  list(APPEND PLATFORM_LIB_LINK_LIBS
	gaen_audio
	gaen_cara
	)
endif()

list(APPEND PLATFORM_LIB_LINK_LIBS
  gaen_scripts
  ${PLATFORM_LINK_LIBS}
  )

target_link_libraries(gaen_platform
  ${PLATFORM_LIB_LINK_LIBS}
  )

add_dependencies(gaen_platform
  CODEGEN_MESSAGES
  CODEGEN_SCRIPTS
  )
