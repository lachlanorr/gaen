#-------------------------------------------------------------------------------
# codegen.cmake - Autogenerated cmake containing generated classes${license_text}
#-------------------------------------------------------------------------------

set(scripts_dir ${scripts_dir})

set(gaen_scripts_codegen_SOURCES
  $${CMAKE_CURRENT_BINARY_DIR}/script_includes.h
  $${CMAKE_CURRENT_BINARY_DIR}/registration.cpp
${files}
)

source_group("" FILES $${CMAKE_CURRENT_BINARY_DIR}/script_includes.h)
source_group("" FILES $${CMAKE_CURRENT_BINARY_DIR}/registration.cpp)
${ide_source_props}
