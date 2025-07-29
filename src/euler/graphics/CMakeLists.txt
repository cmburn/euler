add_library(euler_graphics STATIC
        camera.cpp
        camera.h
        color.cpp
        color.h
        ext.cpp
        ext.h
        font.cpp
        font.h
        polygon.cpp
        polygon.h
        text.cpp
        text.h
        window.cpp
        window.h
        ext.cpp
        ext.h
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        euler_vulkan
        glm::glm
)

target_link_libraries(euler_graphics PRIVATE
        Vulkan2D
)