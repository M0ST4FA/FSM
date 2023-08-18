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

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")
set(UTILITY_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/utility/include/")

# ADD THE LIBRARY
add_subdirectory("./utility/")	
add_library(fsm "./src/FiniteStateMachine.cpp")
target_include_directories(fsm PUBLIC 
"${UTILITY_INCLUDE_DIR}" 
"${PROJECT_INCLUDE_DIR}"
)
target_link_libraries(fsm utility)

# ADD THE EXAMPLES
file(GLOB EXAMPLE_SRCs "./examples/*.cpp")
add_executable(examples ${EXAMPLE_SRCs})
target_include_directories(fsm PUBLIC 
"${UTILITY_INCLUDE_DIR}" 
"${PROJECT_INCLUDE_DIR}"
)
target_link_libraries(examples fsm)

# ADD THE TESTS
option(BUILD_TESTS "Whether to build tests or not." NO)

if(${BUILD_TESTS})
	include("cmake/install_gtest.cmake")
	add_subdirectory("./tests/")
endif()

# ADD THE DOCUMENTATION
option(BUILD_DOCUMENTATION 
"Whether or not to build the documentation"
NO
)

# BUILD THE DOCUMENTATION
if(${BUILD_DOCUMENTATION})
	add_subdirectory("./docs/")
endif()
