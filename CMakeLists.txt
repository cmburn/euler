cmake_minimum_required(VERSION 3.25)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)

set(EULER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(extern)
include_directories(${EULER_ROOT})
add_subdirectory(src)
add_subdirectory(bin)