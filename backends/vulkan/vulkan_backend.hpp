#pragma once
#include <vulkan/vulkan.h>
#include <cstddef>
#include <vector>
#include <gpu_pipeline/gpu_transform_pipeline.hpp>

struct VulkanBufferRecord {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    std::size_t size = 0;
};

class VulkanBackend {
public:
    // Configure device/physical device before use
    static void configure(VkDevice device, VkPhysicalDevice physicalDevice);

    // Optional init hook (no-op for now)
    static void init();

    static GpuBufferHandle create_buffer(std::size_t sizeBytes);
    static void destroy_buffer(GpuBufferHandle handle);

    static void upload(GpuBufferHandle handle,
                       const float* data,
                       std::size_t count);

    static void set_debug_name(GpuBufferHandle handle,
                               const char* name);

    static VkBuffer get_vk_buffer(GpuBufferHandle handle);

private:
    static VkDevice s_device;
    static VkPhysicalDevice s_physicalDevice;
    static std::vector<VulkanBufferRecord> s_buffers;

    static uint32_t find_memory_type(uint32_t typeFilter,
                                     VkMemoryPropertyFlags properties);
};