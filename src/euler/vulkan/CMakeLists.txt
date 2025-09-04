add_library(euler_vulkan STATIC
        buffer.cpp
        buffer.h
        camera.cpp
        camera.h
        compute_pipeline.cpp
        compute_pipeline.h
        descriptor_buffer.cpp
        descriptor_buffer.h
        device.cpp
        device.h
        graphics_pipeline.cpp
        graphics_pipeline.h
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
        texture.cpp
        texture.h
)

add_library(euler_vulkan_vma STATIC vma.cpp)

target_compile_definitions(euler_vulkan PUBLIC
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

target_compile_options(euler_vulkan PRIVATE ${EULER_COMPILE_OPTIONS})

target_link_libraries(euler_vulkan_vma PUBLIC
        GPUOpen::VulkanMemoryAllocator
)

target_link_libraries(euler_vulkan PUBLIC
        euler_util
        glm::glm
        euler_vulkan_vma
)
