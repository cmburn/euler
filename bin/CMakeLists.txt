add_executable(euler_main
        main.cpp
)

set_target_properties(euler_main PROPERTIES
        OUTPUT_NAME "euler"
)

target_link_libraries(euler_main PUBLIC
        euler_engine
        Vulkan2D
)