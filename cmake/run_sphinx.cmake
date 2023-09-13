
# Add the cmake folder so the FindSphinx module is found
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" 
${CMAKE_MODULE_PATH})

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Sphinx)
set(SPHINX_BUILD ${CMAKE_BINARY_DIR}/docs/sphinx)

add_custom_target(Sphinx ALL
			  COMMAND
			  ${SPHINX_EXECUTABLE} -b html
			  ${SPHINX_SOURCE} ${SPHINX_BUILD}
			  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			  COMMENT "Generating documentation with Sphinx"
			  )