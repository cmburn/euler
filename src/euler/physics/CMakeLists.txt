add_library(euler_physics STATIC
        dynamic_body.cpp
        dynamic_body.h
        body.cpp
        body.h
        ext.cpp
        ext.h
        fluid.cpp
        fluid.h
        material.cpp
        material.h
        static_body.cpp
        static_body.h
        world.cpp
        world.h
)

target_link_libraries(euler_physics PUBLIC
        glm::glm
        euler_util
)