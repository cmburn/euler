add_library(euler_app STATIC
        ext.cpp
        ext.h
        event.cpp
        event.h
        game_ext.cpp
        game_ext.h
        graphics_ext.cpp
        graphics_ext.h
        gui_ext.cpp
        gui_ext.h
        state.cpp
        state.h
        system.cpp
        system.h
        util_ext.cpp
        util_ext.h
        vulkan_ext.cpp
        vulkan_ext.h
        window.cpp
        window.h
)

target_link_libraries(euler_app PUBLIC
        euler_vulkan
        euler_graphics
        euler_gui
        euler_util
        mruby
)

target_include_directories(euler_app PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/mruby/build/host/include
)

#target_compile_definitions(euler_app PUBLIC
#        MRB_USE_CXX_ABI
#        MRB_USE_CXX_EXCEPTION
#)

target_compile_options(euler_app PRIVATE ${EULER_COMPILE_OPTIONS})
