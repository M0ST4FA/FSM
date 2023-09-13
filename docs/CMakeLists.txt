

if (CMAKE_BUILD_TYPE MATCHES [Dd]ebug)

	include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/run_doxygen.cmake")
	include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/run_sphinx.cmake")

endif()