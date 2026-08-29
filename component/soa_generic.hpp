#pragma once
#include "types.hpp"
#include <vector>
#include <unordered_map>
#include <cassert>

struct IComponentStorage {
    virtual ~IComponentStorage() = default;
    virtual void remove(const EntityID&) = 0;
    virtual bool has(const EntityID&) const = 0;
};

template <typename T>
class SoAStorage : public IComponentStorage {
public:
    std::unordered_map<EntityID, std::size_t> index;
    std::vector<T> data;

    bool has(const EntityID& id) const override {
        return index.contains(id);
    }

    T* get(const EntityID& id) {
        if (!index.contains(id)) return nullptr;
        return &data[index[id]];
    }

    T& emplace(const EntityID& id, const T& value) {
        assert(!index.contains(id));
        std::size_t i = data.size();
        index[id] = i;
        data.push_back(value);
        return data.back();
    }

    void remove(const EntityID& id) override {
        if (!index.contains(id)) return;

        std::size_t i = index[id];
        std::size_t last = data.size() - 1;

        data[i] = data[last];

        for (auto& [e, idx] : index) {
            if (idx == last) {
                idx = i;
                break;
            }
        }

        data.pop_back();
        index.erase(id);
    }
};