#pragma once
#include <component/types.hpp>
#include <component/components.hpp>
#include <component/soa_generic.hpp>
#include <component/soa_transform.hpp>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include <string>

using EntityID = std::string;

class Registry {
public:
    EntityID create(const EntityID& id) {
        entities.insert(id);
        return id;
    }

    void destroy(const EntityID& id) {
        entities.erase(id);
        for (auto& [_, storage] : components)
            storage->remove(id);
    }

    // SoA Transform
    Transform assign_transform(const EntityID& id, const Transform& t) {
        auto& s = get_or_create_transform();
        s.emplace(id, t);
        return s.get(id);
    }

    Transform get_transform(const EntityID& id) {
        return get_or_create_transform().get(id);
    }

    TransformSoA& transform_storage() {
        return get_or_create_transform();
    }

    // Generic components (AoS in SoAStorage<T>)
    template <typename T>
    T& assign(const EntityID& id, const T& value) {
        auto& s = get_or_create_generic<T>();
        return s.emplace(id, value);
    }

    template <typename T>
    T* get(const EntityID& id) {
        auto it = components.find(typeid(T));
        if (it == components.end()) return nullptr;
        return static_cast<SoAStorage<T>*>(it->second.get())->get(id);
    }

    template <typename T>
    SoAStorage<T>& storage() {
        return get_or_create_generic<T>();
    }

private:
    std::unordered_set<EntityID> entities;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> components;

    TransformSoA& get_or_create_transform() {
        auto key = std::type_index(typeid(Transform));
        if (!components.contains(key)) {
            components[key] = std::make_unique<TransformSoA>();
        }
        return *static_cast<TransformSoA*>(components[key].get());
    }

    template <typename T>
    SoAStorage<T>& get_or_create_generic() {
        auto key = std::type_index(typeid(T));
        if (!components.contains(key)) {
            components[key] = std::make_unique<SoAStorage<T>>();
        }
        return *static_cast<SoAStorage<T>*>(components[key].get());
    }
};