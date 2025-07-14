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
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        Eigen3::Eigen
)

target_link_libraries(euler_graphics PRIVATE
        Vulkan2D
)