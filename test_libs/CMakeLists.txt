cmake_minimum_required(VERSION 3.13)

project(test_libs C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(lctest SHARED
  src/base_type.cpp
  src/std_type.cpp
  src/struct_type.cpp
  src/callback_type.cpp
)

