#include <loader/scene_loader.hpp>
#include <systems/runner_mt.hpp>
#include <gpu_pipeline/gpu_transform_pipeline.hpp>
#include "d3d12_backend.hpp"
#include <fstream>

int main() {
    // 1. Create Vulkan instance, device, physicalDevice
    VkInstance instance = /* ... */;
    VkDevice device = /* ... */;
    VkPhysicalDevice physicalDevice = /* ... */;

    VulkanBackend::init(device, physicalDevice);

    Registry reg;

    std::ifstream f("example_scene.json");
    nlohmann::json scene = nlohmann::json::parse(f);
    load_scene(reg, scene);

    SystemRunnerMT::run(reg);

    // Create GPU buffer sized for transforms
    GpuTransformBuffer gpuTransforms{
        .handle = D3D12Backend::create_buffer(
            reg.transform_storage().pos_x.size() * 3 * sizeof(float)
        )
    };

    GpuTransformPipeline::build_buffer(reg, gpuTransforms);

    GpuTransformPipeline::upload_to_gpu(
        gpuTransforms,
        [](GpuBufferHandle h, const float* data, std::size_t count) {
            D3D12Backend::upload(h, data, count);
        }
    );

    return 0;
}