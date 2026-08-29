#pragma once
#include "ecs_registry.hpp"
#include "components_parent.hpp"

class HierarchySystem {
public:
    static void update(Registry& reg) {
        auto& parents = reg.storage<Parent>();
        auto& t = reg.transform_storage();

        for (auto& [child, idx] : parents.index) {
            const Parent& p = parents.data[idx];

            if (!t.has(child)) continue;
            if (!t.has(p.parent)) continue;

            size_t ci = t.index[child];
            size_t pi = t.index[p.parent];

            // Position
            t.pos_x[ci] += t.pos_x[pi];
            t.pos_y[ci] += t.pos_y[pi];
            t.pos_z[ci] += t.pos_z[pi];

            // Rotation
            t.rot_x[ci] += t.rot_x[pi];
            t.rot_y[ci] += t.rot_y[pi];
            t.rot_z[ci] += t.rot_z[pi];

            // Scale
            t.scl_x[ci] *= t.scl_x[pi];
            t.scl_y[ci] *= t.scl_y[pi];
            t.scl_z[ci] *= t.scl_z[pi];
        }
    }
};