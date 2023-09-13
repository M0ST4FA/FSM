
# VARIABLES ------------------------------
set(GTEST_INCLUDE_DIR 
"${CMAKE_SOURCE_DIR}/external/googletest/googletest/include/gtest/"
)

set(PROJECT_INCLUDE_DIR
"${CMAKE_SOURCE_DIR}/include/"
)

set(PROJECT_SRC_DIR
"${CMAKE_SOURCE_DIR}/src/"
)

include(GoogleTest)

# TESTS ----------------------------------

# DFATests
add_executable(DFATests "dfa.cpp")
target_include_directories(DFATests PRIVATE
"${GTEST_INCLUDE_DIR}/../"
"${PROJECT_INCLUDE_DIR}"
)
target_link_libraries(DFATests PUBLIC fsm gtest_main)
gtest_discover_tests(DFATests
TEST_PREFIX FSMTests
)

# NFATests
add_executable(NFATests "nfa.cpp")
target_include_directories(NFATests PRIVATE
"${GTEST_INCLUDE_DIR}/../"
"${PROJECT_INCLUDE_DIR}"
)
target_link_libraries(NFATests PUBLIC fsm gtest_main)
gtest_discover_tests(NFATests
TEST_PREFIX FSMTests
)
