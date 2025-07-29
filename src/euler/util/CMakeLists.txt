add_library(euler_util STATIC
        logger.cpp
        logger.h
        object.cpp
        object.h
        optparse.c
        optparse.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        version.cpp
        version.h
        thread.cpp
        thread.h
        ext.cpp
        ext.h
        config.cpp
        config.h
)

target_link_libraries(euler_util PUBLIC
        mruby_headers
        SDL3-static
)

target_link_libraries(euler_util PRIVATE
        physfs-static
)

target_include_directories(euler_util PUBLIC
        ${MRUBY_INCLUDE_DIR}
        ${EULER_ROOT}
)

target_include_directories(euler_util PRIVATE
        ${PHYSFS_INCLUDE_DIR}
)