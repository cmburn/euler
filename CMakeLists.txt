cmake_minimum_required(VERSION 4.0)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

find_package(Vulkan REQUIRED)

set(EULER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(extern)
include_directories(./extern/mruby/include)
include_directories(${EULER_ROOT})
add_subdirectory(src)
add_subdirectory(bin)