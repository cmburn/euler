cmake_policy(SET CMP0171 NEW)

file(GLOB EULER_SHADERS
        ${EULER_SHADER_DIR}/*.vert
        ${EULER_SHADER_DIR}/*.frag
        ${EULER_SHADER_DIR}/*.comp
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/renderer_blobs.cpp
        COMMAND ${Ruby_EXECUTABLE}
                ${EULER_SHADER_DIR}/generate_blobs.rb
                ${CMAKE_CURRENT_BINARY_DIR}/renderer_blobs.cpp
                ${EULER_SHADER_DIR}
        DEPENDS ${EULER_SHADER_DIR}/generate_blobs.rb ${EULER_SHADERS}
        CODEGEN
)

add_library(euler_vulkan STATIC
        buffer.cpp
        buffer.h
        camera.cpp
        camera.h
        command_buffer.cpp
        command_buffer.h
        compute_pipeline.cpp
        compute_pipeline.h
        descriptor_buffer.cpp
        descriptor_buffer.h
        device.cpp
        device.h
        graphics_pipeline.cpp
        graphics_pipeline.h
        image.cpp
        image.h
        physical_device.cpp
        physical_device.h
        pipeline.cpp
        pipeline.h
        renderer.cpp
        renderer.h
        shader.cpp
        shader.h
        shadow_environment.cpp
        shadow_environment.h
        surface.cpp
        surface.h
        swapchain.cpp
        swapchain.h
        texture.cpp
        texture.h
        ${CMAKE_CURRENT_BINARY_DIR}/renderer_blobs.cpp
)

add_library(euler_vulkan_vma STATIC vma.cpp)

target_compile_definitions(euler_vulkan PUBLIC
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

target_compile_options(euler_vulkan PRIVATE ${EULER_COMPILE_OPTIONS})

target_link_libraries(euler_vulkan_vma PUBLIC
        GPUOpen::VulkanMemoryAllocator
        ${Vulkan_LIBRARIES}
)

target_link_libraries(euler_vulkan PUBLIC
        ${Vulkan_LIBRARIES}
        euler_util
        glm::glm
        euler_vulkan_vma
)
