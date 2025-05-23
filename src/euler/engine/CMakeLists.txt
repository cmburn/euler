add_library(euler_engine STATIC
        engine.cpp
        engine.h
)

target_link_libraries(euler_engine PUBLIC
        euler_util
        euler_renderer
)