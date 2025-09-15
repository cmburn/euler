add_library(euler_graphics STATIC
        camera.cpp
        camera.h
        font.cpp
        font.h
        image.cpp
        image.h
        polygon.cpp
        polygon.h
        text.cpp
        text.h
        window.cpp
        window.h
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        euler_vulkan
        glm::glm
)
target_compile_options(euler_graphics PRIVATE ${EULER_COMPILE_OPTIONS})