#pragma once
#include "types.hpp"
#include <string>

struct Transform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

struct Camera {
    float fov;
    float near;
    float far;
};

struct RenderMesh {
    std::string mesh;
    std::string material;
};

struct PhysicsBody {
    enum class BodyType { Static, Dynamic, Kinematic };
    enum class Collider { Box, Sphere, Capsule, Mesh };

    BodyType bodyType;
    Collider collider;
};

struct Light {
    enum class Type { Directional, Point, Spot };
    Type lightType;
    vec3 color;
    float intensity;
};

struct Parent {
    EntityID parent;
};