#pragma once
#include "ecs_registry.hpp"
#include <vector>
#include <cstddef>

// Backend-agnostic GPU buffer handle.
// Your renderer owns the actual GPU resource and uses this as an ID/token.
struct GpuBufferHandle {
    std::size_t id;
    std::size_t generation;
};

struct GpuTransformBuffer {
    GpuBufferHandle handle;
    std::vector<float> data; // packed positions (or whatever layout you choose)

    void resize(std::size_t floats) {
        data.resize(floats);
    }
};

class GpuTransformPipeline {
public:
    // Build a packed position buffer from TransformSoA
    static void build_buffer(Registry& reg, GpuTransformBuffer& out) {
        auto& t = reg.transform_storage();
        const std::size_t count = t.pos_x.size();

        // 3 floats per position (x, y, z)
        const std::size_t floats = count * 3;
        out.resize(floats);

        std::size_t idx = 0;
        for (std::size_t i = 0; i < count; ++i) {
            out.data[idx++] = t.pos_x[i];
            out.data[idx++] = t.pos_y[i];
            out.data[idx++] = t.pos_z[i];
        }
    }

    // Backend-agnostic upload hook:
    // You implement this in your renderer, using the handle + data.
    template <typename UploadFn>
    static void upload_to_gpu(const GpuTransformBuffer& buf, UploadFn&& upload) {
        // UploadFn signature:
        // void upload(GpuBufferHandle handle, const float* data, std::size_t count);
        upload(buf.handle, buf.data.data(), buf.data.size());
    }
};