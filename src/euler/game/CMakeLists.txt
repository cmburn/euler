add_library(euler_game STATIC
        ext.cpp
        ext.h
        state.cpp
        state.h
)

target_link_libraries(euler_game PUBLIC
        euler_util
        euler_graphics
        euler_physics

)