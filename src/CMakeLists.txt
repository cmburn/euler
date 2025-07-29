include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(EULER_INTERNAL_BUILD=1)

add_subdirectory(euler/game)
add_subdirectory(euler/graphics)
add_subdirectory(euler/gui)
add_subdirectory(euler/physics)
add_subdirectory(euler/util)
add_subdirectory(euler/vulkan)
