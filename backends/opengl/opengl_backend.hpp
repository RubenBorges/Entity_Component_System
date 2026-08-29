#pragma once
#include <gpu_pipeline/gpu_transform_pipeline.hpp>
#include <cstddef>
#include <string>

// Forward-declare GL types; include your actual GL loader in the .cpp
using GLBuffer = unsigned int;

struct GpuBufferRecord {
    GLBuffer glBuffer = 0;
    std::size_t size = 0;
};

class OpenGLBackend {
public:
    static void init();
    static GpuBufferHandle create_buffer(std::size_t sizeBytes);
    static void destroy_buffer(GpuBufferHandle handle);
    static void upload(GpuBufferHandle handle, const float* data, std::size_t count);
    static void set_debug_name(GpuBufferHandle handle, const std::string& name);
    //static void set_debug_name(GpuBufferHandle handle, const char* name);
    static void bind_as_array_buffer(GpuBufferHandle handle);
};