
SET (<<PROJECT_NAME>>_LIB_SOURCES
  <<PROJECT_NAME>>_api.cpp
  <<PROJECT_NAME>>_api.h
  )

MSVC_PRECOMPILED_HEADER (<<PROJECT_NAME>>_lib <<PROJECT_NAME>>_LIB_SOURCES)
IDE_SOURCE_PROPERTIES( "/" "${<<PROJECT_NAME>>_LIB_SOURCES}")

ADD_LIBRARY (<<PROJECT_NAME>>_lib
  ${<<PROJECT_NAME>>_LIB_SOURCES}
  )
