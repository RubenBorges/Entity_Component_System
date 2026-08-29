#pragma once
#include <entity/ecs_registry.hpp>
#include <component/components.hpp>

class LightSystem {
public:
    static void update(Registry& reg) {
        auto& t = reg.transform_storage();
        auto& lights = reg.storage<Light>();

        for (auto& [entity, idx] : lights.index) {
            if (!t.has(entity)) continue;

            size_t ti = t.index[entity];
            const Light& l = lights.data[idx];

            printf("Light (%f,%f,%f) intensity %f\n",
                t.pos_x[ti], t.pos_y[ti], t.pos_z[ti],
                l.intensity);
        }
    }
};