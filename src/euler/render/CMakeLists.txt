add_library(euler_render STATIC
        camera.cpp
        camera.h
        renderer.cpp
        renderer.h
        shader.cpp
        shader.h
        shadow_environment.cpp
        shadow_environment.h
        shadow_object.cpp
        shadow_object.h
        texture.cpp
        texture.h
)

add_library(euler_render_vma STATIC
        vma.cpp
)

target_link_libraries(euler_render PRIVATE euler_render_vma)
