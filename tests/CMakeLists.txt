
option(USE_GOOGLE_TEST "Whether to use google test or not." NO)

if (USE_GOOGLE_TEST)
	enable_testing()
	
	# SET THE CXX STANDARD
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	# Fetch THE GOOGLETEST LIBRARY
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY https://github.com/google/googletest.git
	  GIT_TAG        "b796f7d44681514f58a683a3a71ff17c94edb0c1" # release-1.13.x
	  FIND_PACKAGE_ARGS NAMES GTest
	)

	# INTRODUCE THE LIBRARY INTO THE PROJECT
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	# ADD THE TESTS
	file(GLOB TEST_SRCs "./*.cpp")
	add_executable(tests ${TEST_SRCs})

	target_link_libraries(
	  tests
	  utility
	  GTest::gtest_main
	)
	target_include_directories(tests PUBLIC 
	"." 
	"../include/" 
	"../utility/include"
	)

	# DISCOVER AND RUN THE TESTS BY CTEST
	include(GoogleTest)
	gtest_discover_tests(tests)

endif()