#pragma once
#include <entity/ecs_registry.hpp>
#include <component/components.hpp>

class RenderSystem {
public:
    static void update(Registry& reg) {
        auto& t = reg.transform_storage();
        auto& meshes = reg.storage<RenderMesh>();

        for (auto& [entity, idx] : meshes.index) {
            if (!t.has(entity)) continue;

            size_t ti = t.index[entity];
            const RenderMesh& rm = meshes.data[idx];

            // Example: print instead of draw
            // Replace with your renderer
            printf("Draw mesh %s at (%f, %f, %f)\n",
                rm.mesh.c_str(),
                t.pos_x[ti], t.pos_y[ti], t.pos_z[ti]);
        }
    }
};