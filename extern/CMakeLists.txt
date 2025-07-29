set(SDL_STATIC ON)
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_BUILD_SHARED OFF)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(glm)
add_subdirectory(physfs)
add_subdirectory(nlohmann-json)
add_subdirectory(VulkanMemoryAllocator)

set(MRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
        CACHE PATH "Path to the mruby include directory" FORCE)
add_library(mruby_headers INTERFACE)
target_include_directories(mruby_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include)
set(MRUBY_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a
        CACHE FILEPATH "Path to the mruby library" FORCE)

set(MRUBY_LDFLAGS PARENT_SCOPE -lmruby -lws2_32 -lwsock32 -lws2_32)

set(PHYSFS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physfs/src
        CACHE PATH "Path to the physfs include directory" FORCE
)

