set(SDL_STATIC ON)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(glm)
add_subdirectory(physfs)
add_subdirectory(nlohmann-json)
add_subdirectory(VulkanMemoryAllocator)

target_sources(physfs-static PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.h
)

target_include_directories(physfs-static INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SDL/include>
        $<INSTALL_INTERFACE:include/>
)

set(MRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
        CACHE PATH "Path to the mruby include directory" FORCE)

file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/mruby-headers.txt MRUBY_HEADERS)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/mruby-sources.txt MRUBY_SOURCES)

add_library(mruby INTERFACE ${MRUBY_HEADERS} ${MRUBY_SOURCES})

set(MRUBY_LIBRARIES
        ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a
        ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby_core.a
)

target_include_directories(mruby INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
)

target_compile_definitions(mruby INTERFACE
        MRB_USE_CXX_ABI
        MRB_USE_CXX_EXCEPTION
)

target_link_libraries(mruby INTERFACE ${MRUBY_LIBRARIES})

if (WIN32)
    target_link_libraries(mruby INTERFACE ws2_32 wsock32)
endif ()

set(PHYSFS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physfs/src
        CACHE PATH "Path to the physfs include directory" FORCE
)

target_include_directories(VulkanMemoryAllocator INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/src>
)
