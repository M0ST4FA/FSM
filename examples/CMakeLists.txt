file(GLOB EXAMPLE_SRCs "./*.cpp")
add_executable(examples ${EXAMPLE_SRCs})
target_link_libraries(examples PUBLIC fsm)