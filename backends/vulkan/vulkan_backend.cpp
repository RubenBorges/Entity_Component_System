#include "vulkan_backend.hpp"
#include <cstring>
#include <stdexcept>

VkDevice VulkanBackend::s_device = VK_NULL_HANDLE;
VkPhysicalDevice VulkanBackend::s_physicalDevice = VK_NULL_HANDLE;
std::vector<VulkanBufferRecord> VulkanBackend::s_buffers;

void VulkanBackend::configure(VkDevice device, VkPhysicalDevice physicalDevice) {
    s_device = device;
    s_physicalDevice = physicalDevice;
}

void VulkanBackend::init() {
    // No-op for now; keep for symmetry with other backends
}

uint32_t VulkanBackend::find_memory_type(uint32_t typeFilter,
                                         VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(s_physicalDevice, &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
        if ((typeFilter & (1u << i)) &&
            (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable Vulkan memory type");
}

GpuBufferHandle VulkanBackend::create_buffer(std::size_t sizeBytes) {
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = sizeBytes,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };

    VkBuffer buffer = VK_NULL_HANDLE;
    if (vkCreateBuffer(s_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan buffer");
    }

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(s_device, buffer, &memReq);

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memReq.size,
        .memoryTypeIndex = find_memory_type(
            memReq.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        )
    };

    VkDeviceMemory memory = VK_NULL_HANDLE;
    if (vkAllocateMemory(s_device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        vkDestroyBuffer(s_device, buffer, nullptr);
        throw std::runtime_error("Failed to allocate Vulkan buffer memory");
    }

    vkBindBufferMemory(s_device, buffer, memory, 0);

    std::size_t id = s_buffers.size();
    s_buffers.push_back(VulkanBufferRecord{
        .buffer = buffer,
        .memory = memory,
        .size = sizeBytes
    });

    return GpuBufferHandle{ .id = id, .generation = 1 };
}

void VulkanBackend::destroy_buffer(GpuBufferHandle handle) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (rec.buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(s_device, rec.buffer, nullptr);
        rec.buffer = VK_NULL_HANDLE;
    }
    if (rec.memory != VK_NULL_HANDLE) {
        vkFreeMemory(s_device, rec.memory, nullptr);
        rec.memory = VK_NULL_HANDLE;
    }
    rec.size = 0;
}

void VulkanBackend::upload(GpuBufferHandle handle,
                           const float* data,
                           std::size_t count) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (rec.memory == VK_NULL_HANDLE) return;

    void* mapped = nullptr;
    vkMapMemory(s_device, rec.memory, 0, count * sizeof(float), 0, &mapped);
    std::memcpy(mapped, data, count * sizeof(float));
    vkUnmapMemory(s_device, rec.memory);
}

void VulkanBackend::set_debug_name(GpuBufferHandle handle,
                                   const char* name) {
    if (handle.id >= s_buffers.size()) return;

    auto& rec = s_buffers[handle.id];
    if (rec.buffer == VK_NULL_HANDLE) return;

    // Optional: VK_EXT_debug_utils
    auto vkSetDebugUtilsObjectNameEXT =
        reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
            vkGetDeviceProcAddr(s_device, "vkSetDebugUtilsObjectNameEXT"));

    if (!vkSetDebugUtilsObjectNameEXT) return;

    VkDebugUtilsObjectNameInfoEXT info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = nullptr,
        .objectType = VK_OBJECT_TYPE_BUFFER,
        .objectHandle = reinterpret_cast<uint64_t>(rec.buffer),
        .pObjectName = name
    };

    vkSetDebugUtilsObjectNameEXT(s_device, &info);
}

VkBuffer VulkanBackend::get_vk_buffer(GpuBufferHandle handle) {
    if (handle.id >= s_buffers.size()) return VK_NULL_HANDLE;
    return s_buffers[handle.id].buffer;
}