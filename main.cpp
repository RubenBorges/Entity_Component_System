//IGPU MAIN
#include "IGpuBackend.hpp"
#include "opengl_backend_adapter.hpp"   // or vulkan/d3d12
#include "ecs_scene_loader.hpp"
#include "ecs_schema_register.hpp"
#include "system_runner_mt.hpp"
#include "gpu_transform_pipeline.hpp"
#include <simdjson.h>

int main() {
    register_ecs_schema();

    simdjson::dom::parser parser;
    simdjson::dom::element scene =
        parser.load("example_scene.json").value(); // <-- fix

    Registry reg;
    load_scene(reg, scene);

    SystemRunnerMT::run(reg);

    // Choose backend
    IGpuBackend* gpu = &OpenGLBackendAPI;

    gpu->init();

    GpuTransformBuffer gpuTransforms{
        .handle = gpu->create_buffer(
            reg.transform_storage().pos_x.size() * 3 * sizeof(float)
        )
    };

    GpuTransformPipeline::build_buffer(reg, gpuTransforms);

    gpu->upload(gpuTransforms.handle,
                gpuTransforms.data.data(),
                gpuTransforms.data.size());

    return 0;
}
/*

#include "ecs_scene_loader.hpp"
#include "ecs_schema_register.hpp"
#include "system_runner_mt.hpp"
#include "gpu_transform_pipeline.hpp"

#include <simdjson.h>
#include <iostream>

int main() {
    // 1. Register all component schemas
    register_ecs_schema();

    // 2. Parse scene JSON using simdjson
    simdjson::dom::parser parser;
    simdjson::dom::element scene = parser.load("example_scene.json");

    // 3. Create ECS registry
    Registry reg;

    // 4. Load scene using schema-driven loader
    load_scene(reg, scene);

    // 5. Run multithreaded systems
    SystemRunnerMT::run(reg);

    // 6. Build GPU transform buffer
    GpuTransformBuffer gpuTransforms{
        .handle = { .id = 0, .generation = 1 }
    };

    GpuTransformPipeline::build_buffer(reg, gpuTransforms);

    // 7. Upload using backend-agnostic callback
    GpuTransformPipeline::upload_to_gpu(
        gpuTransforms,
        [](GpuBufferHandle h, const float* data, std::size_t count) {
            std::cout << "Uploading " << count << " floats to GPU buffer "
                      << h.id << " (gen " << h.generation << ")\n";
        }
    );

    return 0;
} */
/*
#include "ecs_scene_loader.hpp"
#include "system_runner_mt.hpp"
#include "gpu_transform_pipeline.hpp"
#include "opengl_backend.hpp"
#include <fstream>

int main() {
    // 1. Create GL context (platform-specific)
    // 2. Initialize GL loader (glad/glew/etc.)
    register_ecs_schema();

    OpenGLBackend::init();

    Registry reg;

    std::ifstream f("example_scene.json");
    
    // EITHER USE THIS:
    simdjson::dom::parser parser;
    simdjson::dom::element scene = parser.load("example_scene.json");
    
    // OR USE THIS:
    //nlohmann::json scene = nlohmann::json::parse(f);
   
    load_scene(reg, scene);

    SystemRunnerMT::run(reg);

    // Create GPU buffer sized for transforms
    GpuTransformBuffer gpuTransforms{
        .handle = OpenGLBackend::create_buffer(
            reg.transform_storage().pos_x.size() * 3 * sizeof(float)
        )
    };

    GpuTransformPipeline::build_buffer(reg, gpuTransforms);

    GpuTransformPipeline::upload_to_gpu(
        gpuTransforms,
        [](GpuBufferHandle h, const float* data, std::size_t count) {
            OpenGLBackend::upload(h, data, count);
        }
    );

    return 0;
}
    */