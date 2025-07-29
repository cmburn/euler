add_library(euler_vulkan STATIC
        buffer.cpp
        buffer.h
        camera.cpp
        camera.h
        descriptor_buffer.cpp
        descriptor_buffer.h
        descriptor_control.cpp
        descriptor_control.h
        instance.cpp
        instance.h
        renderer.cpp
        renderer.h
        surface.cpp
        surface.h
        swapchain.cpp
        swapchain.h
        vma.cpp
)

target_link_libraries(euler_vulkan PUBLIC
        euler_util
        glm::glm
        GPUOpen::VulkanMemoryAllocator
)