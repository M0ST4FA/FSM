# CMakeList.txt : CMake project for myacc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(fsm)

# SETUP VARIABLES

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(fsm_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(utility_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utility/include/")

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

# ADD THE LIBRARY
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/utility/")
add_library(fsm 
"${CMAKE_CURRENT_SOURCE_DIR}/src/FiniteStateMachine.cpp"
"include/fsm/FiniteStateMachine.h"
"include/fsm/DFA.h"
"include/fsm/NFA.h"
)
target_include_directories(fsm PUBLIC 
"${utility_INCLUDE_DIR}" 
"${fsm_INCLUDE_DIR}"
)
target_link_libraries(fsm PUBLIC utility)

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