add_library(euler_util STATIC
        logger.cpp
        logger.h
        object.cpp
        object.h
        state.cpp
        state.h
        defer.cpp
        defer.h
        thread.cpp
        thread.h
        version.cpp
        version.h
)

target_link_libraries(euler_util PUBLIC
        fmt::fmt
        mruby_headers
        SDL3-static
)

target_link_libraries(euler_util PRIVATE
        spdlog::spdlog
)

target_include_directories(euler_util PUBLIC
        ${MRUBY_INCLUDE_DIR}
        ${EULER_ROOT}
)