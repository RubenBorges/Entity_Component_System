// #pragma once
// #include <d3d12.h>
// #include <cstddef>
// #include <vector>
// #include "gpu_transform_pipeline.hpp"

// struct D3D12BufferRecord {
//     ID3D12Resource* resource = nullptr;
//     std::size_t size = 0;
// };

// class D3D12Backend {
// public:
//     // Must be called once with a valid device
//     static void configure(ID3D12Device* device);

//     // Optional init hook (no-op for now)
//     static void init();

//     static GpuBufferHandle create_buffer(std::size_t sizeBytes);
//     static void destroy_buffer(GpuBufferHandle handle);

//     static void upload(GpuBufferHandle handle,
//                        const float* data,
//                        std::size_t count);

//     static void set_debug_name(GpuBufferHandle handle,
//                                const char* name);

//     static ID3D12Resource* get_resource(GpuBufferHandle handle);

// private:
//     static ID3D12Device* s_device;
//     static std::vector<D3D12BufferRecord> s_buffers;
// };