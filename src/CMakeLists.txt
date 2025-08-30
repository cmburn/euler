include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(EULER_INTERNAL_BUILD=1)

set(EULER_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        # We need to disable -Wclass-memaccess because of how we handle wrapping C++
        # classes in mRuby.
        -Wno-class-memaccess
)

add_subdirectory(euler/game)
add_subdirectory(euler/graphics)
add_subdirectory(euler/gui)
add_subdirectory(euler/physics)
add_subdirectory(euler/util)
add_subdirectory(euler/vulkan)
