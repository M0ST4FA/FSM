file(GLOB EXAMPLE_SRCs "./*.cpp")
add_executable(FSMExamples ${EXAMPLE_SRCs})
target_link_libraries(FSMExamples PUBLIC fsm)