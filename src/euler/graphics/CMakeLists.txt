add_library(euler_graphics STATIC
        camera.cpp
        camera.h
        color.cpp
        color.h
        font.cpp
        font.h
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
