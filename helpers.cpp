#include <nlohmann/json.hpp>
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