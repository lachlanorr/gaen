#-------------------------------------------------------------------------------
# CMakeLists.txt - Build script
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2016 Lachlan Orr
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

SET (shaders_dir ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
INCLUDE (${shaders_dir}/codegen.cmake)

SET (renderergl_SOURCES
  gaen_opengl.h
  renderer_api.cpp
  RendererGL.h
  RendererGL.cpp
  RendererGL_${platform}.${platform_ext}
  ShaderRegistry.cpp
  ShaderRegistry.h
  ${shaders_codegen_SOURCES}
  )

IF (WIN32)
  SET(renderergl_SOURCES ${renderergl_SOURCES}
    win32gl.cpp
    win32gl.h
    )
ENDIF (WIN32)

IDE_SOURCE_PROPERTIES( "/" "${renderergl_SOURCES}")

ADD_LIBRARY (renderergl
  ${renderergl_SOURCES}
  )

ADD_DEPENDENCIES(renderergl
  hashes
  )
