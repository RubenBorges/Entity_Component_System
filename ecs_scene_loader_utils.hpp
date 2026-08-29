#pragma once
#include <simdjson.h>
#include "components.hpp"
#include <string>

using simdjson::dom::element;
using simdjson::dom::array;

inline vec3 to_vec3(const element& arr) {
    array a = arr.get_array();
    return vec3{
        float(double(a.at(0))),
        float(double(a.at(1))),
        float(double(a.at(2)))
    };
}

inline PhysicsBody::BodyType to_body_type(const std::string& s) {
    if (s == "static")   return PhysicsBody::BodyType::Static;
    if (s == "dynamic")  return PhysicsBody::BodyType::Dynamic;
    if (s == "kinematic")return PhysicsBody::BodyType::Kinematic;
    throw std::runtime_error("Unknown PhysicsBody::BodyType: " + s);
}

inline PhysicsBody::Collider to_collider(const std::string& s) {
    if (s == "box")     return PhysicsBody::Collider::Box;
    if (s == "sphere")  return PhysicsBody::Collider::Sphere;
    if (s == "capsule") return PhysicsBody::Collider::Capsule;
    if (s == "mesh")    return PhysicsBody::Collider::Mesh;
    throw std::runtime_error("Unknown PhysicsBody::Collider: " + s);
}

inline Light::Type to_light_type(const std::string& s) {
    if (s == "directional") return Light::Type::Directional;
    if (s == "point")       return Light::Type::Point;
    if (s == "spot")        return Light::Type::Spot;
    throw std::runtime_error("Unknown Light::Type: " + s);
}