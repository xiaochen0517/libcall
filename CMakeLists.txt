cmake_minimum_required(VERSION 3.13)

project(libcall
    VERSION 0.0.1
    DESCRIPTION "Use JSON to call C functions dynamically."
)

configure_file(include/conf.hpp.in "${CMAKE_BINARY_DIR}/include/conf.hpp")

message([DEBUG] "${CMAKE_BINARY_DIR}")

# 设置默认构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# 设置编译器标志
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Debug builds." FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Release builds." FORCE)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp include/*.cpp include/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include)

find_package(Catch2 3 REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(unofficial-libffi CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 主程序可执行文件
add_executable(libcall ${SOURCE_FILES})

target_link_libraries(libcall PRIVATE
    CLI11::CLI11
    unofficial::libffi::libffi
    nlohmann_json::nlohmann_json
)

# 启用测试
enable_testing()

# 收集测试源文件（排除 src/main.cpp）
file(GLOB_RECURSE TEST_SOURCE_FILES tests/*.cpp)
file(GLOB_RECURSE LIB_SOURCE_FILES
    src/util/*.cpp
    src/adapter/*.cpp
    include/*.hpp
)

# 创建测试可执行文件
add_executable(libcall-tests ${TEST_SOURCE_FILES} ${LIB_SOURCE_FILES})

target_link_libraries(libcall-tests PRIVATE
    Catch2::Catch2WithMain
    CLI11::CLI11
    unofficial::libffi::libffi
)

# 自动发现并注册 Catch2 测试
include(CTest)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(libcall-tests)


option(BUILD_TEST_LIBS "Build test libraries for dynamic loading tests" ON)

if(BUILD_TEST_LIBS)
  add_subdirectory(test_libs)
endif()

