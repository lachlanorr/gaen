#-------------------------------------------------------------------------------
# win32.cmake - OSX specific settings used in CMakeLists.txt files
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2020 Lachlan Orr
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

SET (platform "win32")
SET (platform_ext "cpp")
SET (net_platform "win32")

SET(PLATFORM_LINK_LIBS
  Ws2_32
  opengl32.lib
  )

SET(CMAKE_CXX_FLAGS_DEBUG  "/MTd /MP /Zi /Ob0 /Od /RTC1 /D_DEBUG /DIS_BUILD_Debug=1")
SET(CMAKE_CXX_FLAGS_RELEASE  "/MT /MP /O2 /Ob2 /DNDEBUG /DIS_BUILD_Release=1")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /MP /O1 /Ob1 /DNDEBUG /DIS_BUILD_Release=1")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /MP /Zi /O2 /Ob1 /DNDEBUG /DIS_BUILD_Release=1")

SET(CMAKE_C_FLAGS_DEBUG  "/MTd /MP /Zi /Ob0 /Od /RTC1 /D_DEBUG /DIS_BUILD_Debug=1")
SET(CMAKE_C_FLAGS_RELEASE  "/MT /MP /O2 /Ob2 /DNDEBUG /DIS_BUILD_Release=1")
SET(CMAKE_C_FLAGS_MINSIZEREL "/MT /MP /O1 /Ob1 /DNDEBUG /DIS_BUILD_Release=1")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "/MT /MP /Zi /O2 /Ob1 /DNDEBUG /DIS_BUILD_Release=1")

SET(USE_GLFW ON)

OPTION(USE_MSVC_FAST_FLOATINGPOINT "Use MSVC /fp:fast option" ON)
IF(USE_MSVC_FAST_FLOATINGPOINT)
  ADD_DEFINITIONS(/fp:fast)
ENDIF(USE_MSVC_FAST_FLOATINGPOINT)
ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(/D _SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(/D NOMINMAX)

SET_PROPERTY (GLOBAL PROPERTY USE_FOLDERS ON)

# Preps MSVC filters
MACRO (IDE_SOURCE_PROPERTIES source_path sources)
IF (MSVC)
  STRING (REPLACE "/" "\\\\" source_group_path ${source_path} )
  SOURCE_GROUP (${source_group_path} FILES ${sources})
ENDIF (MSVC)
ENDMACRO (IDE_SOURCE_PROPERTIES NAME sources)

# Add stdafx MSVC precompiled headers
MACRO (MSVC_PRECOMPILED_HEADER target_name sources)
  IF (MSVC)
    SET (stdafx_h    "${target_name}/stdafx.h")
    SET (stdafx_cpp  "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp")
    SET (pch_file    "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.pch")
    SET (sources_ref ${${sources}})

    SET_SOURCE_FILES_PROPERTIES (${stdafx_cpp}
                                 PROPERTIES COMPILE_FLAGS "/Yc\"${stdafx_h}\" /Fp\"${pch_file}\""
                                            OBJECT_OUTPUTS "${pch_file}")
    SET_SOURCE_FILES_PROPERTIES (${sources_ref}
                                 PROPERTIES COMPILE_FLAGS "/Yu\"${stdafx_h}\" /FI\"${pch_file}\" /Fp\"${pch_file}\""
                                            OBJECT_DEPENDS "${pch_file}")
    # Add precompiled header to sources
    LIST (APPEND ${sources} "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h")
    LIST (APPEND ${sources} ${stdafx_cpp})
  ENDIF (MSVC)
ENDMACRO (MSVC_PRECOMPILED_HEADER source_path sources)
