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

include(utils.cmake)

# Adjust external builds and strip out all but the minimal required targets
set(gaen_MINEXT TRUE)

# unit tests
include(googletest.cmake)

# compression libs
include(zlib.cmake)
include(bzip2.cmake)
include(brotli.cmake)

# audio
if (NOT DEFINED IS_HEADLESS)
  include(portaudio.cmake)
endif()

# math
include(glm.cmake)

# rendering
if (NOT DEFINED IS_HEADLESS)
  include(glad.cmake)
  include(glfw.cmake)
  include(imgui.cmake)
  include(nanovg.cmake)
endif()

# physics
include(bullet3.cmake)

# image cooking
include(libtiff.cmake)
include(libpng.cmake)

# fonts
include(freetype.cmake)

# model cooking
include(assimp.cmake)

# json
include(rapidjson.cmake)
