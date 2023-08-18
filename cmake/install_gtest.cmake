
# Set the preferred options 
set(INSTALL_GTEST ON CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

# Add googletest to the project
add_subdirectory(external/googletest)