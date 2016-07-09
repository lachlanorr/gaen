IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/codegen.cmake")
  INCLUDE ("${CMAKE_CURRENT_BINARY_DIR}/codegen.cmake")
ENDIF ()

SET (scripts_SOURCES
  ${scripts_codegen_SOURCES}
  )

ADD_LIBRARY (scripts
  DUMMY.cpp
  ${scripts_SOURCES}
  )

IDE_SOURCE_PROPERTIES( "/" "DUMMY.cpp" )

ADD_CUSTOM_TARGET (
  CODEGEN_SCRIPTS ALL
  python ${python_dir}/codegen.py "${CMAKE_CURRENT_BINARY_DIR}"
  # re-run update_hashes.py since generated .cpp files will likely
  # have new hashes to consider.
  COMMAND python ${python_dir}/update_hashes.py "${CMAKE_BINARY_DIR}/src/hashes"
  DEPENDS cmpc
  COMMENT "Generating scripts"
  )

ADD_DEPENDENCIES(scripts
  hashes
  engine
  CODEGEN_SCRIPTS
  )
