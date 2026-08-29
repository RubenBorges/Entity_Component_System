#pragma once
#include "types.hpp"
#include "components.hpp"
#include "soa_generic.hpp"
#include <unordered_map>
#include <vector>

class TransformSoA : public IComponentStorage {
public:
    std::unordered_map<EntityID, std::size_t> index;

    std::vector<float> pos_x, pos_y, pos_z;
    std::vector<float> rot_x, rot_y, rot_z;
    std::vector<float> scl_x, scl_y, scl_z;

    bool has(const EntityID& id) const override {
        return index.contains(id);
    }

    Transform get(const EntityID& id) const {
        std::size_t i = index.at(id);
        return Transform{
            {pos_x[i], pos_y[i], pos_z[i]},
            {rot_x[i], rot_y[i], rot_z[i]},
            {scl_x[i], scl_y[i], scl_z[i]}
        };
    }

    Transform& emplace(const EntityID& id, const Transform& t) {
        std::size_t i = pos_x.size();
        index[id] = i;

        pos_x.push_back(t.position.x);
        pos_y.push_back(t.position.y);
        pos_z.push_back(t.position.z);

        rot_x.push_back(t.rotation.x);
        rot_y.push_back(t.rotation.y);
        rot_z.push_back(t.rotation.z);

        scl_x.push_back(t.scale.x);
        scl_y.push_back(t.scale.y);
        scl_z.push_back(t.scale.z);

        // fake reference: reconstruct on demand
        static Transform tmp;
        tmp = t;
        return tmp;
    }

    void remove(const EntityID& id) override {
        std::size_t i = index[id];
        std::size_t last = pos_x.size() - 1;

        auto swap = [&](auto& arr) {
            arr[i] = arr[last];
            arr.pop_back();
        };

        swap(pos_x); swap(pos_y); swap(pos_z);
        swap(rot_x); swap(rot_y); swap(rot_z);
        swap(scl_x); swap(scl_y); swap(scl_z);

        for (auto& [e, idx] : index) {
            if (idx == last) {
                idx = i;
                break;
            }
        }

        index.erase(id);
    }
};