add_library(euler_vulkan STATIC
        renderer.cpp
        renderer.h
        window.cpp
        window.h
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
)

target_link_libraries(euler_vulkan PRIVATE
        euler_vulkan_vma
)