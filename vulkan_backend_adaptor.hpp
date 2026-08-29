#pragma once
#include "IGpuBackend.hpp"
#include "vulkan_backend.hpp"

inline IGpuBackend VulkanBackendAPI {
    .name = "Vulkan",
    .init = &VulkanBackend::init,
    .create_buffer = &VulkanBackend::create_buffer,
    .destroy_buffer = &VulkanBackend::destroy_buffer,
    .upload = &VulkanBackend::upload,
    .set_debug_name = &VulkanBackend::set_debug_name
};