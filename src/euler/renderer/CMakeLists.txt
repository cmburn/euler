add_library(euler_renderer STATIC
        application.cpp
        application.h
        camera.cpp
        camera.h
        font.cpp
        font.h
        gui.cpp
        gui.h
        polygon.cpp
        polygon.h
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
        vma.cpp
        window.cpp
        window.h
)

target_link_libraries(euler_renderer PUBLIC
        fmt::fmt
        Eigen3::Eigen
)

target_link_libraries(euler_renderer PRIVATE
        Vulkan2D
)