IF (CMAKE_CONFIGURATION_TYPES)
  SET (CMAKE_CONFIGURATION_TYPES "Debug;Release"
    CACHE STRING "Reset the configurations to what we need" FORCE)
ENDIF()


# Include a compiler specific .cmake file.  This weirdness is required
# to distinguish Clang from GCC, as well as to account for build
# wrappers.
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  ADD_DEFINITIONS("-DIS_COMPILER_CLANG=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  INCLUDE (${cmake_dir}/gcc.cmake)
  ADD_DEFINITIONS("-DIS_COMPILER_GCC=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
  ADD_DEFINITIONS("-DIS_COMPILER_INTEL=1")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  INCLUDE (${cmake_dir}/win32.cmake)
  ADD_DEFINITIONS("-DIS_COMPILER_MSVC=1")
ENDIF()


IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  IF (IOS)
    INCLUDE (${cmake_dir}/ios.cmake)
    ADD_DEFINITIONS("-DIS_PLATFORM_IOS=1")
  ELSE()
    INCLUDE (${cmake_dir}/osx.cmake)
    ADD_DEFINITIONS("-DIS_PLATFORM_OSX=1")
  ENDIF()
ENDIF ()
IF (WIN32)
  ADD_DEFINITIONS("-DIS_PLATFORM_WIN32=1")
ENDIF ()
IF (UNIX)
  INCLUDE (${cmake_dir}/posix.cmake)
  ADD_DEFINITIONS("-DIS_PLATFORM_POSIX=1")
ENDIF ()


