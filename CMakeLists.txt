# CMakeList.txt : CMake project for myacc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# DECLARE THE PROJECT
project(fsm
VERSION 4.1.0
DESCRIPTION "A library for state machine simulation, written in C++."
LANGUAGES CXX C
)

# SETUP VARIABLES

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(${PROJECT_NAME}_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")

# SETUPT OPTIONS
option(BUILD_TESTING 
"Whether to build tests or not." 
NO)

option(BUILD_DOCUMENTATION 
"Whether or not to build the documentation"
NO
)

option(BUILD_EXAMPLES 
"Whether to build examples or not." 
NO)

# BUILD googletest
if(${BUILD_TESTING})
	include("cmake/install_gtest.cmake")
endif()

# ADD EXTERNAL LIBRARIES
add_subdirectory("${PROJECT_SOURCE_DIR}/external/utility/")

# ADD THE LIBRARY
add_library(${PROJECT_NAME} 
"${PROJECT_SOURCE_DIR}/src/FiniteStateMachine.cpp"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/FiniteStateMachine.h"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/DFA.h"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/NFA.h"
)
target_include_directories(${PROJECT_NAME} PUBLIC 
"${${PROJECT_NAME}_INCLUDE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC utility tabulate::tabulate fmt::fmt)

# ADD THE EXAMPLES
if(${BUILD_EXAMPLES})
	add_subdirectory("./examples/")
endif()

# ADD THE TESTS
if(${BUILD_TESTING})
	enable_testing()
	add_subdirectory("./tests/")
endif()

# BUILD THE DOCUMENTATION
if(${BUILD_DOCUMENTATION})
	add_subdirectory("./docs/")
endif()