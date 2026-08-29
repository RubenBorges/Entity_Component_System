#pragma once
#include <entity/ecs_registry.hpp>

#include <simdjson.h>
void load_scene(Registry& reg, const simdjson::dom::element& root);

//#include <nlohmann/json.hpp>
//void load_scene(Registry& reg, const nlohmann::json& root);