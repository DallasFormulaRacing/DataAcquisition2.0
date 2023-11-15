cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  # The hash-ID at the end of this URL should be updated occassionally
  URL https://github.com/google/googletest/archive/f8d7d77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  daq-unit-tests
  Src/hello_test.cc
)

# Linking with `gtest_main` instead of `gtest` allows us to bypass having
#   to write a main() function
target_link_libraries(
  daq-unit-tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(daq-unit-tests)