#pragma once
#include "ecs_schema.hpp"
#include "ecs_scene_loader_utils.hpp"

inline void register_ecs_schema() {
    auto& S = ECS_Schema::instance();

    // ------------------------------
    // Transform
    // ------------------------------
    S.register_component({
        "Transform",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            Transform t{
                .position = to_vec3(data["position"]),
                .rotation = to_vec3(data["rotation"]),
                .scale    = to_vec3(data["scale"])
            };
            reg.assign_transform(entity, t);
        }
    });

    // ------------------------------
    // Camera
    // ------------------------------
    S.register_component({
        "Camera",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            Camera c{
                .fov  = float(double(data["fov"])),
                .near = float(double(data["near"])),
                .far  = float(double(data["far"]))
            };
            reg.assign<Camera>(entity, c);
        }
    });

    // ------------------------------
    // RenderMesh
    // ------------------------------
    S.register_component({
        "RenderMesh",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            RenderMesh rm{
                .mesh     = std::string(data["mesh"]),
                .material = std::string(data["material"])
            };
            reg.assign<RenderMesh>(entity, rm);
        }
    });

    // ------------------------------
    // PhysicsBody
    // ------------------------------
    S.register_component({
        "PhysicsBody",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            PhysicsBody pb{
                .bodyType = to_body_type(std::string(data["bodyType"])),
                .collider = to_collider(std::string(data["collider"]))
            };
            reg.assign<PhysicsBody>(entity, pb);
        }
    });

    // ------------------------------
    // Light
    // ------------------------------
    S.register_component({
        "Light",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            Light l{
                .lightType = to_light_type(std::string(data["lightType"])),
                .color     = to_vec3(data["color"]),
                .intensity = float(double(data["intensity"]))
            };
            reg.assign<Light>(entity, l);
        }
    });

    // ------------------------------
    // Parent (Hierarchy)
    // ------------------------------
    S.register_component({
        "Parent",
        [](Registry& reg, const std::string& entity, const simdjson::dom::element& data) {
            Parent p{
                .parent = std::string(data["parent"])
            };
            reg.assign<Parent>(entity, p);
        }
    });
}