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

  # PE3 ECU CAN Bus Messaging Tests
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/frame_formats.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/frame_formats.cpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe1.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe2.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe3.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe4.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe5.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe6.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe7.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe8.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe9.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe10.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe11.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe12.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe13.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe14.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe15.hpp
  ../DAQ_System/Program/Src/Sensor/ECU/PE3/Frames/frame_pe16.hpp
  Src/Sensor/ECU/PE3/frame_parsing_tests.cpp
  Src/Sensor/ECU/PE3/frame_formats_tests.cpp
)

# Linking with `gtest_main` instead of `gtest` allows us to bypass having
#   to write a main() function
target_link_libraries(
  daq-unit-tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(daq-unit-tests)
