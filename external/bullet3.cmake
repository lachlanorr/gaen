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

option(BUILD_CPU_DEMOS "" OFF)
option(USE_GLUT "" OFF)
option(BUILD_PYBULLET "" OFF)
option(BUILD_EGL "" OFF)
option(BUILD_OPENGL3_DEMOS "" OFF)
option(BUILD_ENET "" OFF)
option(BUILD_CLSOCKET "" OFF)
option(BUILD_BULLET2_DEMOS "" OFF)
option(BUILD_EXTRAS "" OFF)
option(BUILD_UNIT_TESTS "" OFF)
option(INSTALL_LIBS "" OFF)
option(INSTALL_CMAKE_FILES "" OFF)
add_subdirectory(bullet3)
configure_target_folders("bullet3" FALSE)

target_include_directories(BulletCollision INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src
  )

target_include_directories(BulletDynamics INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src
  )

target_include_directories(LinearMath INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src
  )
