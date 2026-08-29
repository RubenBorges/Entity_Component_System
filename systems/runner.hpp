#pragma once
#include "transform.hpp"
#include "hierarchy.hpp"
#include "render.hpp"
#include "physics.hpp"
#include "light.hpp"

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
