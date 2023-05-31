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
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# ADD THE LIBRARY
add_subdirectory("./utility/")
add_library(fsm "./src/FiniteStateMachine.cpp")
target_include_directories(fsm PUBLIC 
"./utility/include/" 
"./include/"
)
target_link_libraries(fsm utility)

# ADD THE EXAMPLES
file(GLOB EXAMPLE_SRCs "./examples/*.cpp")
add_executable(examples ${EXAMPLE_SRCs})
target_include_directories(fsm PUBLIC 
"./utility/include/" 
"./include/"
)
target_link_libraries(examples fsm)