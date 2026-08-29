#pragma once
#include "gpu_transform_pipeline.hpp"
#include <vulkan/vulkan.h>
#include <cstddef>

struct VulkanBufferRecord {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    std::size_t size = 0;
};

class VulkanBackend {
public:
    // Must be called after Vulkan instance/device creation
    static void init(VkDevice device, VkPhysicalDevice physicalDevice);

    static GpuBufferHandle create_buffer(std::size_t sizeBytes);
    static void destroy_buffer(GpuBufferHandle handle);

    static void upload(GpuBufferHandle handle, const float* data, std::size_t count);

    // Optional: return VkBuffer for binding in pipelines
    static VkBuffer get_vk_buffer(GpuBufferHandle handle);

private:
    static VkDevice s_device;
    static VkPhysicalDevice s_physicalDevice;
    static std::vector<VulkanBufferRecord> s_buffers;

    static uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};