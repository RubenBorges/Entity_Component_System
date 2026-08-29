#pragma once
#include <cstddef>
#include <string>
#include "gpu_transform_pipeline.hpp"

struct IGpuBackend {
    const char* name;

    // Backend initialization (OpenGL: context already created)
    void (*init)();

    // Buffer lifecycle
    GpuBufferHandle (*create_buffer)(std::size_t sizeBytes);
    void (*destroy_buffer)(GpuBufferHandle handle);

    // Upload float data
    void (*upload)(GpuBufferHandle handle,
                   const float* data,
                   std::size_t count);

    // Optional debug name
    void (*set_debug_name)(GpuBufferHandle handle,
                           const char* name);
};