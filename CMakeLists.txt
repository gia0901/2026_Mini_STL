cmake_minimum_required(VERSION 3.16)

project(Mini_STL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GoogleTest
find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Core Library (static)


# Tests
add_subdirectory(tests/)
