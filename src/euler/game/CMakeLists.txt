add_library(euler_game STATIC
        ext.cpp
        ext.h
        game_ext.cpp
        game_ext.h
        graphics_ext.cpp
        graphics_ext.h
        gui_ext.cpp
        gui_ext.h
        physics_ext.cpp
        physics_ext.h
        state.cpp
        state.h
        util_ext.cpp
        util_ext.h
        vulkan_ext.cpp
        vulkan_ext.h
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

target_link_libraries(euler_game PUBLIC
        ${MRUBY_LIBRARIES}
)

target_compile_definitions(euler_game PUBLIC
        MRB_USE_CXX_ABI
        MRB_USE_CXX_EXCEPTION
)

target_compile_options(euler_game PRIVATE ${EULER_COMPILE_OPTIONS})
