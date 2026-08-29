cmake_minimum_required(VERSION 3.20)
project(ECS_Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------
# Backend options
# ---------------------------------------------------------
option(USE_OPENGL "Enable OpenGL backend" OFF)
option(USE_VULKAN "Enable Vulkan backend" OFF)
option(USE_D3D12  "Enable D3D12 backend"  OFF)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------
find_package(simdjson CONFIG REQUIRED)

if (USE_OPENGL)
    find_package(OpenGL REQUIRED)
endif()

if (USE_VULKAN)
    find_package(Vulkan REQUIRED)
endif()

if (USE_D3D12 AND WIN32)
    find_package(directx-headers CONFIG REQUIRED)
endif()

# ---------------------------------------------------------
# Source files
# ---------------------------------------------------------
set(ECS_SOURCES
    ecs_types.hpp
    ecs_registry.hpp
    soa_generic.hpp
    soa_transform.hpp

    components.hpp
    components_parent.hpp

    ecs_scene_loader.hpp
    ecs_scene_loader.cpp
    ecs_scene_loader_utils.hpp

    ecs_schema.hpp
    ecs_schema_register.hpp

    system_transform.hpp
    system_hierarchy.hpp
    system_render.hpp
    system_physics.hpp
    system_light.hpp
    system_runner_mt.hpp

    gpu_transform_pipeline.hpp

    main.cpp
)

# Optional backend sources
if (USE_OPENGL)
    list(APPEND ECS_SOURCES opengl_backend.hpp opengl_backend.cpp)
endif()

if (USE_VULKAN)
    list(APPEND ECS_SOURCES vulkan_backend.hpp vulkan_backend.cpp)
endif()

if (USE_D3D12 AND WIN32)
    list(APPEND ECS_SOURCES d3d12_backend.hpp d3d12_backend.cpp)
endif()

add_executable(ECS_Engine ${ECS_SOURCES})

target_include_directories(ECS_Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------
# Link dependencies
# ---------------------------------------------------------
target_link_libraries(ECS_Engine PRIVATE simdjson::simdjson)

if (USE_OPENGL)
    target_link_libraries(ECS_Engine PRIVATE OpenGL::GL)
endif()

if (USE_VULKAN)
    target_link_libraries(ECS_Engine PRIVATE Vulkan::Vulkan)
endif()

if (USE_D3D12 AND WIN32)
    target_link_libraries(ECS_Engine PRIVATE Microsoft::DirectX-Headers)
endif()

# ---------------------------------------------------------
# Copy example_scene.json
# ---------------------------------------------------------
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/example_scene.json
    ${CMAKE_CURRENT_BINARY_DIR}/example_scene.json
    COPYONLY
)