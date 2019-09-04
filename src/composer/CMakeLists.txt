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

SET (composer_SOURCES
  main_composer.cpp
  BaseWindow.cpp
  BaseWindow.h
  MenuBar.cpp
  MenuBar.h
  Messages.cpp
  Messages.h
  )

MSVC_PRECOMPILED_HEADER (composer composer_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${composer_SOURCES}")

ADD_DEFINITIONS("-DIS_BUILD_Tool=1")

ADD_EXECUTABLE (composer WIN32
  ${composer_SOURCES}
  )

ADD_DEPENDENCIES(composer
  zlib
  libpng
  )

TARGET_LINK_LIBRARIES (composer
  core
  nana
  optimized ${LIBPNG_LIB}
  debug     ${LIBPNG_LIBD}
  optimized ${ZLIB_LIB}
  debug     ${ZLIB_LIBD}
  ${PLATFORM_LINK_LIBS}
  )

package (composer)
