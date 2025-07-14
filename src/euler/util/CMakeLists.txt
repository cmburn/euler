add_library(euler_util STATIC
        defer.cpp
        defer.h
        logger.cpp
        logger.h
        object.cpp
        object.h
        opaque.cpp
        opaque.h
        optparse.c
        optparse.h
        sdl.cpp
        sdl.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        thread.cpp
        thread.h
        value.cpp
        value.h
        version.cpp
        version.h
)

target_link_libraries(euler_util PUBLIC
        mruby_headers
        SDL3-static
)

target_link_libraries(euler_util PRIVATE
        spdlog::spdlog
        physfs-static
)

target_include_directories(euler_util PUBLIC
        ${MRUBY_INCLUDE_DIR}
        ${EULER_ROOT}
)

target_include_directories(euler_util PRIVATE
        ${PHYSFS_INCLUDE_DIR}
)