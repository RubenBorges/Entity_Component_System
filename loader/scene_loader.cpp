#include "scene_loader.hpp"
#include "schema.hpp"
#include "schema_register.hpp"

using simdjson::dom::element;
using simdjson::dom::object;

void load_scene(Registry& reg, const element& root) {
    // Register schemas once
    static bool initialized = false;
    if (!initialized) {
        register_ecs_schema();
        initialized = true;
    }

    // Load entities
    for (auto id : root["entities"].get_array()) {
        reg.create(std::string(id));
    }

    // Load components using schema
    object comps = root["components"].get_object();

    for (auto [compName, compTable] : comps) {
        auto& S = ECS_Schema::instance();

        if (!S.has(std::string(compName))) {
            throw std::runtime_error("Unknown component type in scene: " + std::string(compName));
        }

        auto loader = S.get(std::string(compName));

        for (auto [entity, data] : compTable.get_object()) {
            loader(reg, std::string(entity), data);
        }
    }
}

/*
//-------------------------------_ALTERNATIVE METHOD:--------------------------
#include "ecs_scene_loader.hpp"
#include <stdexcept>
#include <string>
#include "components.hpp"

// ------------------------------
// PhysicsBody::BodyType
// ------------------------------
PhysicsBody::BodyType to_body_type(const std::string& s) {
    if (s == "static")   return PhysicsBody::BodyType::Static;
    if (s == "dynamic")  return PhysicsBody::BodyType::Dynamic;
    if (s == "kinematic")return PhysicsBody::BodyType::Kinematic;
    throw std::runtime_error("Unknown PhysicsBody::BodyType: " + s);
}

// ------------------------------
// PhysicsBody::Collider
// ------------------------------
PhysicsBody::Collider to_collider(const std::string& s) {
    if (s == "box")     return PhysicsBody::Collider::Box;
    if (s == "sphere")  return PhysicsBody::Collider::Sphere;
    if (s == "capsule") return PhysicsBody::Collider::Capsule;
    if (s == "mesh")    return PhysicsBody::Collider::Mesh;
    throw std::runtime_error("Unknown PhysicsBody::Collider: " + s);
}

// ------------------------------
// Light::Type
// ------------------------------
Light::Type to_light_type(const std::string& s) {
    if (s == "directional") return Light::Type::Directional;
    if (s == "point")       return Light::Type::Point;
    if (s == "spot")        return Light::Type::Spot;
    throw std::runtime_error("Unknown Light::Type: " + s);
}

using simdjson::dom::element;
using simdjson::dom::array;
using simdjson::dom::object;

vec3 to_vec3(const element& arr) {
    array a = arr.get_array();
    return vec3{
        float(double(a.at(0))),
        float(double(a.at(1))),
        float(double(a.at(2)))
    };
}

void load_entities(Registry& reg, const element& root) {
    for (auto id : root["entities"].get_array()) {
        reg.create(std::string(id));
    }
}

void load_transforms(Registry& reg, const element& comps) {
    if (!comps["Transform"].error()) {
        for (auto [entity, data] : comps["Transform"].get_object()) {
            Transform t{
                .position = to_vec3(data["position"]),
                .rotation = to_vec3(data["rotation"]),
                .scale    = to_vec3(data["scale"])
            };
            reg.assign_transform(std::string(entity), t);
        }
    }
}

void load_cameras(Registry& reg, const element& comps) {
    if (!comps["Camera"].error()) {
        for (auto [entity, data] : comps["Camera"].get_object()) {
            Camera c{
                .fov  = float(double(data["fov"])),
                .near = float(double(data["near"])),
                .far  = float(double(data["far"]))
            };
            reg.assign<Camera>(std::string(entity), c);
        }
    }
}

void load_render_meshes(Registry& reg, const element& comps) {
    if (!comps["RenderMesh"].error()) {
        for (auto [entity, data] : comps["RenderMesh"].get_object()) {
            RenderMesh rm{
                .mesh     = std::string(data["mesh"]),
                .material = std::string(data["material"])
            };
            reg.assign<RenderMesh>(std::string(entity), rm);
        }
    }
}

void load_physics_bodies(Registry& reg, const element& comps) {
    if (!comps["PhysicsBody"].error()) {
        for (auto [entity, data] : comps["PhysicsBody"].get_object()) {
            PhysicsBody pb{
                .bodyType = to_body_type(std::string(data["bodyType"])),
                .collider = to_collider(std::string(data["collider"]))
            };
            reg.assign<PhysicsBody>(std::string(entity), pb);
        }
    }
}

void load_lights(Registry& reg, const element& comps) {
    if (!comps["Light"].error()) {
        for (auto [entity, data] : comps["Light"].get_object()) {
            Light l{
                .lightType = to_light_type(std::string(data["lightType"])),
                .color     = to_vec3(data["color"]),
                .intensity = float(double(data["intensity"]))
            };
            reg.assign<Light>(std::string(entity), l);
        }
    }
}

void load_scene(Registry& reg, const element& root) {
    load_entities(reg, root);
    element comps = root["components"];
    load_transforms(reg, comps);
    load_cameras(reg, comps);
    load_render_meshes(reg, comps);
    load_physics_bodies(reg, comps);
    load_lights(reg, comps);
}



#include "ecs_scene_loader.hpp"
#include "components.hpp"
using json = nlohmann::json;

vec3 to_vec3(const json& j) {
    return vec3{
        j.at(0).get<float>(),
        j.at(1).get<float>(),
        j.at(2).get<float>()
    };
}

PhysicsBody::BodyType to_body_type(const std::string& s) {
    if (s == "static")   return PhysicsBody::BodyType::Static;
    if (s == "dynamic")  return PhysicsBody::BodyType::Dynamic;
    if (s == "kinematic")return PhysicsBody::BodyType::Kinematic;
    throw std::runtime_error("Unknown bodyType: " + s);
}

PhysicsBody::Collider to_collider(const std::string& s) {
    if (s == "box")     return PhysicsBody::Collider::Box;
    if (s == "sphere")  return PhysicsBody::Collider::Sphere;
    if (s == "capsule") return PhysicsBody::Collider::Capsule;
    if (s == "mesh")    return PhysicsBody::Collider::Mesh;
    throw std::runtime_error("Unknown collider: " + s);
}

Light::Type to_light_type(const std::string& s) {
    if (s == "directional") return Light::Type::Directional;
    if (s == "point")       return Light::Type::Point;
    if (s == "spot")        return Light::Type::Spot;
    throw std::runtime_error("Unknown lightType: " + s);
}

void load_entities(Registry& reg, const json& root) {
    const auto& entities = root.at("entities");
    for (const auto& id : entities) {
        reg.create(id.get<std::string>());
    }
}

void load_transforms(Registry& reg, const json& comps) {
    if (!comps.contains("Transform")) return;
    const auto& table = comps.at("Transform");

    for (auto& [entity, data] : table.items()) {
        Transform t{
            .position = to_vec3(data.at("position")),
            .rotation = to_vec3(data.at("rotation")),
            .scale    = to_vec3(data.at("scale"))
        };
        reg.assign_transform(entity, t);
    }
}

void load_cameras(Registry& reg, const json& comps) {
    if (!comps.contains("Camera")) return;
    const auto& table = comps.at("Camera");

    for (auto& [entity, data] : table.items()) {
        Camera c{
            .fov  = data.at("fov").get<float>(),
            .near = data.at("near").get<float>(),
            .far  = data.at("far").get<float>()
        };
        reg.assign<Camera>(entity, c);
    }
}

void load_render_meshes(Registry& reg, const json& comps) {
    if (!comps.contains("RenderMesh")) return;
    const auto& table = comps.at("RenderMesh");

    for (auto& [entity, data] : table.items()) {
        RenderMesh rm{
            .mesh     = data.at("mesh").get<std::string>(),
            .material = data.at("material").get<std::string>()
        };
        reg.assign<RenderMesh>(entity, rm);
    }
}

void load_physics_bodies(Registry& reg, const json& comps) {
    if (!comps.contains("PhysicsBody")) return;
    const auto& table = comps.at("PhysicsBody");

    for (auto& [entity, data] : table.items()) {
        PhysicsBody pb{
            .bodyType = to_body_type(data.at("bodyType").get<std::string>()),
            .collider = to_collider(data.at("collider").get<std::string>())
        };
        reg.assign<PhysicsBody>(entity, pb);
    }
}

void load_lights(Registry& reg, const json& comps) {
    if (!comps.contains("Light")) return;
    const auto& table = comps.at("Light");

    for (auto& [entity, data] : table.items()) {
        Light l{
            .lightType = to_light_type(data.at("lightType").get<std::string>()),
            .color     = to_vec3(data.at("color")),
            .intensity = data.at("intensity").get<float>()
        };
        reg.assign<Light>(entity, l);
    }
}

void load_scene(Registry& reg, const json& root) {
    load_entities(reg, root);

    const auto& comps = root.at("components");
    load_transforms(reg, comps);
    load_cameras(reg, comps);
    load_render_meshes(reg, comps);
    load_physics_bodies(reg, comps);
    load_lights(reg, comps);
}
*/