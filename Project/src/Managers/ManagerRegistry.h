//
// Created by chee7 on 9/26/25.
//

#ifndef GEODASH3D_MANAGERREGISTRY_H
#define GEODASH3D_MANAGERREGISTRY_H
// src/Managers/ManagerRegistry.h
#pragma once

#include "IManager.h"           // MUST exist as IManager.h (not "IManager")
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <algorithm>

class ManagerRegistry {
public:
    static ManagerRegistry& Instance() {
        static ManagerRegistry inst;
        return inst;
    }

    template<typename T, typename... Args>
    void Register(Args&&... args) {
        auto idx = std::type_index(typeid(T));
        if (managers.count(idx)) return;
        managers[idx] = std::unique_ptr<IManager>(new T(std::forward<Args>(args)...));
        order.push_back(idx);
    }

    template<typename T>
    T* Get() {
        auto it = managers.find(std::type_index(typeid(T)));
        if (it == managers.end()) return nullptr;
        return static_cast<T*>(it->second.get());
    }

    void InitAll() {
        for (auto &idx : order) managers[idx]->Init();
    }

    void UpdateAll(float dt) {
        for (auto &idx : order) managers[idx]->Update(dt);
    }

    void ShutdownAll() {
        // shutdown in reverse order
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            managers[*it]->Shutdown();
        }
        order.clear();
        managers.clear();
    }

private:
    ManagerRegistry() = default;
    ~ManagerRegistry() = default;

    std::unordered_map<std::type_index, std::unique_ptr<IManager>> managers;
    std::vector<std::type_index> order;
};

#endif //GEODASH3D_MANAGERREGISTRY_H
