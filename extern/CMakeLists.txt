set(SDL_STATIC ON)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(glm)
add_subdirectory(physfs)
add_subdirectory(nlohmann-json)
add_subdirectory(box2d)

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

file(GLOB VK2D_SOURCES
        Vulkan2D/VK2D/*.c
        Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp
)

file(GLOB VK2D_HEADERS
        Vulkan2D/VK2D/*.h
)

add_library(VK2D STATIC
        ${VK2D_SOURCES}
        ${VK2D_HEADERS}
)

target_compile_options(VK2D PUBLIC
        -Wno-c23-extensions
)

target_include_directories(VK2D PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D
        ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VulkanMemoryAllocator/include
)

target_link_libraries(VK2D PRIVATE
        SDL3-static
        ${Vulkan_LIBRARIES}
)
