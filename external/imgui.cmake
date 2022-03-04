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

set(imgui_SOURCES
  imgui/imconfig.h
  imgui/imgui.cpp
  imgui/imgui.h
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_internal.h
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imstb_rectpack.h
  imgui/imstb_textedit.h
  imgui/imstb_truetype.h
  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_glfw.h
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_opengl3.h
)

source_group("" FILES ${imgui_SOURCES})

add_library(imgui STATIC
  ${imgui_SOURCES}
  )

target_include_directories(imgui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  )

target_link_libraries(imgui PUBLIC
  glfw
  )

set_target_properties(imgui PROPERTIES
  FOLDER external/imgui
  )

configure_source_folders("${imgui_SOURCES}" "imgui/")
