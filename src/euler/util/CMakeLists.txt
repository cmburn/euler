add_library(euler_util STATIC
        color.cpp
        color.h
        config.cpp
        config.h
        logger.cpp
        logger.h
        object.cpp
        object.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        thread.cpp
        thread.h
        mruby_exception.cpp
        mruby_exception.h
        version.cpp
        version.h
)

add_library(euler_util_optparse STATIC
        optparse.c
        optparse.h
)

target_link_libraries(euler_util PUBLIC
        mruby_headers
        SDL3-static
        ${MRUBY_LIBRARIES}
)

target_link_libraries(euler_util PRIVATE
        physfs-static
        euler_util_optparse
        ${MRUBY_LIBRARIES}
)

target_include_directories(euler_util PUBLIC
        ${MRUBY_INCLUDE_DIR}
        ${EULER_ROOT}
)

target_include_directories(euler_util PRIVATE
        ${PHYSFS_INCLUDE_DIR}
)

target_compile_options(euler_util PRIVATE ${EULER_COMPILE_OPTIONS})