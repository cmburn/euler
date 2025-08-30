add_library(euler_util STATIC
        logger.cpp
        logger.h
        object.cpp
        object.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        version.cpp
        version.h
        thread.cpp
        thread.h
        config.cpp
        config.h
)

add_library(euler_util_optparse STATIC
        optparse.c
        optparse.h
)

target_link_libraries(euler_util PUBLIC
        mruby_headers
        ${MRUBY_LIBRARIES}
        SDL3-static
)

target_link_libraries(euler_util PRIVATE
        physfs-static
        spdlog::spdlog
        euler_util_optparse
)

target_include_directories(euler_util PUBLIC
        ${MRUBY_INCLUDE_DIR}
        ${EULER_ROOT}
)

target_include_directories(euler_util PRIVATE
        ${PHYSFS_INCLUDE_DIR}
)

target_compile_options(euler_util PRIVATE ${EULER_COMPILE_OPTIONS})