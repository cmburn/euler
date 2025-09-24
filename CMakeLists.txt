cmake_minimum_required(VERSION 3.17.2)
project(euler)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Ruby REQUIRED)

# TODO: not this
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL/include)

set(EULER_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EULER_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
add_subdirectory(extern)
include_directories(${EULER_ROOT})
add_subdirectory(src)
add_subdirectory(bin)