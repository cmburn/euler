include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions(EULER_INTERNAL_BUILD=1)

set(EULER_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Werror
        # We need to disable -Wclass-memaccess because of how we handle wrapping C++
        # classes in mRuby.
        -Wno-class-memaccess
        # -Wmissing-field-initializers pitches a fit with Vulkan structs
        -Wno-missing-field-initializers
        -Wno-unused-function
)

add_subdirectory(euler/app)
add_subdirectory(euler/graphics)
add_subdirectory(euler/gui)
add_subdirectory(euler/util)
add_subdirectory(euler/vulkan)
