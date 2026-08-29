#pragma once
#include "ecs_registry.hpp"

class TransformSystem {
public:
    static void update(Registry& reg) {
        auto& t = reg.transform_storage();

        for (size_t i = 0; i < t.pos_x.size(); ++i) {
            // Example: no-op or integrate velocity later
            // This is where you'd apply animation, physics, etc.
        }
    }
};