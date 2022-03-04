
set(<<PROJECT_NAME>>_COOKERS_SOURCES
  <<PROJECT_NAME>>_cookers.cpp
  )

msvc_precompiled_header(<<PROJECT_NAME>>_cookers <<PROJECT_NAME>>_COOKERS_SOURCES)
source_group("" FILES ${<<PROJECT_NAME>>_COOKERS_SOURCES})

add_library(<<PROJECT_NAME>>_cookers
  ${<<PROJECT_NAME>>_COOKERS_SOURCES}
  )
