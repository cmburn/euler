add_library(euler_util STATIC
        config.cpp
        config.h
        logger.cpp
        logger.h
        majordomo.cpp
        majordomo.h
        object.cpp
        object.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        thread.cpp
        thread.h
        version.cpp
        version.h
        worker.cpp
        worker.h
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