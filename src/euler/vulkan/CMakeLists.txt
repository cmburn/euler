add_library(euler_vulkan STATIC
        renderer.cpp
        renderer.h
        window.cpp
        window.h
        vma.cpp
)

target_compile_definitions(euler_vulkan PUBLIC
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

target_link_libraries(euler_vulkan PUBLIC
        euler_util
        glm::glm
        GPUOpen::VulkanMemoryAllocator
)