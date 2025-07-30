add_library(euler_game STATIC
        ext.cpp
        ext.h
        state.cpp
        state.h
)

target_link_libraries(euler_game PUBLIC
        euler_graphics
        euler_gui
        euler_physics
        euler_util
        euler_vulkan
)

target_include_directories(euler_game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/mruby/build/host/include
)
