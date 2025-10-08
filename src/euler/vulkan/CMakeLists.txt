add_library(euler_vulkan STATIC
        camera.cpp
        camera.h
        error.cpp
        error.h
        renderer.cpp
        renderer.h
        shader.cpp
        shader.h
        surface.cpp
        surface.h
        texture.cpp
        texture.h
)


target_link_libraries(euler_vulkan PUBLIC
        euler_util
        glm::glm
)

target_link_libraries(euler_vulkan PRIVATE
        VK2D
)
