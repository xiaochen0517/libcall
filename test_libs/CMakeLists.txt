cmake_minimum_required(VERSION 3.13)

project(test_libs C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(base_types SHARED
  base_types/integer_lib.cpp
)

add_library(struct_types SHARED
  struct_types/stdstr_lib.cpp
)

