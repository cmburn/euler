set(SDL_STATIC ON)
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_BUILD_SHARED OFF)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(box2d)
add_subdirectory(eigen)
add_subdirectory(physfs)
add_subdirectory(spdlog)
add_subdirectory(VulkanMemoryAllocator)

file(GLOB VULKAN_2D_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VK2D/*.c)
file(GLOB VULKAN_2D_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VK2D/*.h)

add_library(Vulkan2D STATIC
        ${VULKAN_2D_SOURCES}
        ${VULKAN_2D_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator/src/VmaUsage.cpp
)

target_include_directories(Vulkan2D PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Vulkan2D PUBLIC
        SDL3-static
        GPUOpen::VulkanMemoryAllocator
        Vulkan::Vulkan
)

set(MRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/include
        CACHE PATH "Path to the mruby include directory" FORCE)
add_library(mruby_headers INTERFACE)
target_include_directories(mruby_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mruby/include)
set(MRUBY_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a
        CACHE FILEPATH "Path to the mruby library" FORCE)
add_custom_command(
        COMMAND ${RUBY_EXECUTABLE}
        -r rake
        -e 'Rake.application.init; Rake.application.load_rakefile; Rake::Task["all"].invoke'
        OUTPUT ${MRUBY_LIBRARY}
)

set(MRUBY_PACKAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/)

set(MRUBY_CXXFLAGS PARENT_SCOPE
        -DHAVE_MRUBY_IO_GEM -DMRB_USE_RATIONAL -DMRB_USE_COMPLEX -DMRB_USE_BIGINT -DMRB_USE_DEBUG_HOOK
        -I"${MRUBY_PACKAGE_DIR}/include" -I"${MRUBY_PACKAGE_DIR}/include/mruby/gems/mruby-time/include"
        -I"${MRUBY_PACKAGE_DIR}/include/mruby/gems/mruby-io/include"
)

set(MRUBY_LDFLAGS PARENT_SCOPE -lmruby -lws2_32 -lwsock32 -lws2_32)

set(PHYSFS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physfs/src
        CACHE PATH "Path to the physfs include directory" FORCE
)

