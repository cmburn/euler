set(SDL_STATIC ON)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(glm)
add_subdirectory(physfs)
add_subdirectory(nlohmann-json)
add_subdirectory(libevent)
add_subdirectory(VulkanMemoryAllocator)

target_sources(physfs-static PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.h
)

target_include_directories(physfs-static INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras>
        $<INSTALL_INTERFACE:include/>
)

set(MRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
        CACHE PATH "Path to the mruby include directory" FORCE)
add_library(mruby_headers INTERFACE)
target_include_directories(mruby_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include)
set(MRUBY_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a
        ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby_core.a
        CACHE FILEPATH "Path to the mruby library" FORCE)

set(MRUBY_LDFLAGS PARENT_SCOPE -lmruby -lws2_32 -lwsock32 -lws2_32)

set(PHYSFS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physfs/src
        CACHE PATH "Path to the physfs include directory" FORCE
)

target_include_directories(VulkanMemoryAllocator INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/src>
)
