cmake_minimum_required(VERSION 3.14)
project(yarngpt_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Fetch GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Library
add_library(yarngpt src/yarngpt.cpp)
target_include_directories(yarngpt PUBLIC include)
# When we implement GGML, we would link it here
# target_link_libraries(yarngpt ggml)

# Tests
add_executable(test_yarngpt tests/test_yarngpt.cpp)
target_link_libraries(test_yarngpt yarngpt GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_yarngpt)
