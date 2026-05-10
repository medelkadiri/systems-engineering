cmake_minimum_required(VERSION 3.16)
project(fd_wrapper LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library (header + source)
add_library(fd_wrapper src/fd_wrapper.cpp)
target_include_directories(fd_wrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(fd_wrapper PUBLIC cxx_std_17)
target_compile_options(fd_wrapper PRIVATE -Wall -Wextra -Wpedantic)

# Demo executable
add_executable(fd_wrapper_demo src/demo.cpp)
target_link_libraries(fd_wrapper_demo PRIVATE fd_wrapper)

# Tests (always built, registered for ctest)
enable_testing()
add_executable(fd_wrapper_tests tests/unit/test_fd_wrapper.cpp)
target_link_libraries(fd_wrapper_tests PRIVATE fd_wrapper)
add_test(NAME fd_wrapper_tests COMMAND fd_wrapper_tests)
