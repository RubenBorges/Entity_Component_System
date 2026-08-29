#pragma once
#include "system_transform.hpp"
#include "system_hierarchy.hpp"
#include "system_render.hpp"
#include "system_physics.hpp"
#include "system_light.hpp"
#include <thread>
#include <vector>

class SystemRunnerMT {
public:
    static void run(Registry& reg) {
        // Phase 1: independent updates (can run in parallel)
        std::vector<std::jthread> phase1;
        phase1.emplace_back([&]{ TransformSystem::update(reg); });
        phase1.emplace_back([&]{ PhysicsSystem::update(reg); });

        // jthread auto-joins on destruction

        // Phase 2: hierarchy depends on updated transforms
        HierarchySystem::update(reg);

        // Phase 3: read-only systems (can run in parallel)
        std::vector<std::jthread> phase3;
        phase3.emplace_back([&]{ LightSystem::update(reg); });
        phase3.emplace_back([&]{ RenderSystem::update(reg); });
    }
};