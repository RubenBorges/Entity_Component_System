#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <simdjson.h>
#include "ecs_registry.hpp"
#include "components.hpp"
#include "components_parent.hpp"

// Schema entry for one component type
struct ComponentSchema {
    std::string name;
    std::function<void(Registry&, const std::string&, const simdjson::dom::element&)> loader;
};

// Global schema registry
class ECS_Schema {
public:
    static ECS_Schema& instance() {
        static ECS_Schema s;
        return s;
    }

    void register_component(const ComponentSchema& schema) {
        table[schema.name] = schema.loader;
    }

    bool has(const std::string& name) const {
        return table.contains(name);
    }

    auto& get(const std::string& name) const {
        return table.at(name);
    }

private:
    std::unordered_map<std::string,
        std::function<void(Registry&, const std::string&, const simdjson::dom::element&)>> table;
};