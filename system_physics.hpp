#pragma once
#include "ecs_registry.hpp"
#include "components.hpp"

class PhysicsSystem {
public:
    static void update(Registry& reg) {
        auto& t = reg.transform_storage();
        auto& bodies = reg.storage<PhysicsBody>();

        for (auto& [entity, idx] : bodies.index) {
            if (!t.has(entity)) continue;

            size_t ti = t.index[entity];
            const PhysicsBody& pb = bodies.data[idx];

            // Example: simple gravity
            if (pb.bodyType == PhysicsBody::BodyType::Dynamic) {
                t.pos_y[ti] -= 0.016f * 9.81f; // frame gravity
            }
        }
    }
};