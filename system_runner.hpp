#pragma once
#include "system_transform.hpp"
#include "system_hierarchy.hpp"
#include "system_render.hpp"
#include "system_physics.hpp"
#include "system_light.hpp"

class SystemRunner {
public:
    static void run(Registry& reg) {
        TransformSystem::update(reg);
        HierarchySystem::update(reg);
        PhysicsSystem::update(reg);
        LightSystem::update(reg);
        RenderSystem::update(reg);
    }
};
