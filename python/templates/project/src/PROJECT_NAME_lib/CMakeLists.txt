
set(<<PROJECT_NAME>>_LIB_SOURCES
  <<PROJECT_NAME>>_api.cpp
  <<PROJECT_NAME>>_api.h
  )

msvc_precompiled_header(<<PROJECT_NAME>>_lib <<PROJECT_NAME>>_LIB_SOURCES)
source_group("" FILES ${<<PROJECT_NAME>>_LIB_SOURCES})

add_library(<<PROJECT_NAME>>_lib
  ${<<PROJECT_NAME>>_LIB_SOURCES}
  )
