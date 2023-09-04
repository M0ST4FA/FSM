

if (CMAKE_BUILD_TYPE MATCHES [Dd]ebug)

	# Require dot, treat the other components as optional
	find_package(Doxygen
				REQUIRED dot
				OPTIONAL_COMPONENTS mscgen dia)

	if (${DOXYGEN_FOUND})
		
		# This directory is relative to the binary directory
		set( DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs/" )
		set( DOXYGEN_COLLABORATION_GRAPH YES )
		set( DOXYGEN_EXTRACT_ALL YES )
		set( DOXYGEN_CLASS_DIAGRAMS YES )
		set( DOXYGEN_HIDE_UNDOC_RELATIONS NO )
		set( DOXYGEN_HAVE_DOT YES )
		set( DOXYGEN_CLASS_GRAPH YES )
		set( DOXYGEN_CALL_GRAPH YES )
		set( DOXYGEN_CALLER_GRAPH YES )
		set( DOXYGEN_COLLABORATION_GRAPH YES )
		set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
		set( DOXYGEN_EXTRACT_PRIVATE YES )
		set( DOXYGEN_EXTRACT_PACKAGE YES )
		set( DOXYGEN_EXTRACT_STATIC YES )
		set( DOXYGEN_EXTRACT_LOCALMETHODS YES )
		set( DOXYGEN_UML_LOOK YES )
		set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
		set( DOXYGEN_TEMPLATE_RELATIONS YES )
		set( DOXYGEN_DOT_GRAPH_MAX_NODES 100 )
		set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
		set( DOXYGEN_DOT_TRANSPARENT YES )
		set( DOXYGEN_GENERATE_XML YES)
		set( DOXYGEN_EXCLUDE_PATTERNS 
		"*/docs/*" 
		"*/tests/*" 
		"*/utility/*"
		"*/out/*"
		"*/examples/*"
		"*/external/*"
		)

		doxygen_add_docs(docs ${PROJECT_SOURCE_DIR})
		execute_process(
		COMMAND	"./${DOXYGEN_EXECUTABLE}" 
			"${DOXYGEN_OUTPUT_DIRECTORY}/Doxyfile.docs"
		)

		configure_file(
			"./Sphinx/conf.py.in" 
			"${CMAKE_SOURCE_DIR}/docs/Sphinx/conf.py"
		)

		set(DOXYGEN_OUT "${PROJECT_BINARY_DIR}/docs/doxyfile.docs")
		add_custom_target(Doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			)
	endif()

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


endif()